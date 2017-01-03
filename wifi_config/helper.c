/**
 ******************************************************************************
 *
 *  The MIT License
 *  Copyright (c) 2016 Microsoft Corp.
 *
 *  Permission is hereby granted, free of charge, to any person obtaining a copy
 *  of this software and associated documentation files (the "Software"), to deal
 *  in the Software without restriction, including without limitation the rights
 *  to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 *  copies of the Software, and to permit persons to whom the Software is furnished
 *  to do so, subject to the following conditions:
 *
 *  The above copyright notice and this permission notice shall be included in
 *  all copies or substantial portions of the Software.
 *
 *  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 *  IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 *  FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 *  AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
 *  WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR
 *  IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 *
 ******************************************************************************
 */

#include "mico.h"
#include "helper.h"
#include "ff_gen_drv.h"
#include "sflash_diskio.h"

#define helper_log(M, ...) custom_log("helper", M, ##__VA_ARGS__)
#define check_int(L, S, M)   \
if(S > L)                    \
{                            \
    custom_log("helper", M); \
    return kParamErr;        \
}
#define safe_search_str(S, T, O)              \
O = strstr(S, T);                             \
if(O == NULL)                                 \
{                                             \
    custom_log("helper", "Not found: %s", T); \
    return kNotFoundErr;                      \
}
#define check_file_result(E, M, ...)        \
if(E != FR_OK)                              \
{                                           \
    custom_log("helper", M, ##__VA_ARGS__); \
    return E;                               \
}
#define ensure_fs_mounted()                              \
if(!is_fs_mounted)                                       \
{                                                        \
    FRESULT err = mount_fs();                            \
    if (err != FR_OK)                                    \
    {                                                    \
      custom_log("helper", "Fail to mount filesystem."); \
      return err;                                        \
    }                                                    \
}

static bool is_fs_mounted = false;
static FATFS SFLASHFatFs;       /* File system object for flash disk logical drive */
static FIL SFLASHFile;          /* File object */
static char SFLASHPath[4];      /* SFLASH logical drive path */

int mount_fs()
{
  FRESULT err;
  filesystem_info fatfs_info;
  FATFS_LinkDriver(&SFLASHDISK_Driver, SFLASHPath);

  err = f_mount(&SFLASHFatFs, (TCHAR const *)SFLASHPath, 0);
  check_file_result(err, "Fail to mount filesystem.");

  fatfs_info = fatfs_get_info((uint8_t *)SFLASHPath);
  if (fatfs_info.total_space == 0)
  {
    helper_log("Filesystem total space is %d, need format", fatfs_info.total_space);
    helper_log("Start formatting filesystem...");
    err = f_mkfs((TCHAR const *)SFLASHPath, 0, _MAX_SS);
    check_file_result(err, "Fail to format filesystem.");
    fatfs_info = fatfs_get_info((uint8_t *)SFLASHPath);
  }
  helper_log("filesystem total space is %dKB, free space is %dKB", fatfs_info.total_space, fatfs_info.free_space);
  is_fs_mounted = true;
  return FR_OK;
}

int unmount_fs()
{
  FATFS_UnLinkDriver(SFLASHPath);
  is_fs_mounted = false;
  return FR_OK;
}

int save_file(char *filename, const char *input)
{
  FRESULT err;
  uint32_t byteswritten;

  ensure_fs_mounted();

  err = f_open(&SFLASHFile, filename, FA_CREATE_ALWAYS | FA_WRITE);
  check_file_result(err, "Fail to open file: %s, err: %d", filename, err);

  err = f_write(&SFLASHFile, input, strlen(input), (void *)&byteswritten);
  if ((byteswritten == 0) || (err != FR_OK))
  {
    helper_log("Fail to write file, error %d", err);
    f_close(&SFLASHFile);
    return err;
  }
  helper_log("fatfs write file, name:%s, data:%s", filename, input);

  err = f_close(&SFLASHFile);
  check_file_result(err, "Fail to close file: %s", filename);
  return err;
}

int read_file(char *filename, char *output, size_t output_len)
{
  FRESULT err;
  uint32_t bytesread;

  ensure_fs_mounted();

  err = f_open(&SFLASHFile, filename, FA_READ);
  check_file_result(err, "Fail to open file: %s", filename);

  err = f_read(&SFLASHFile, output, output_len, (void *)&bytesread);
  if ((bytesread == 0) || (err != FR_OK))
  {
    helper_log("fatfs read error %d", err);
    f_close(&SFLASHFile);
    return err;
  }
  helper_log("fatfs read file, name:%s, data:%s", filename, output);

  err = f_close(&SFLASHFile);
  check_file_result(err, "Fail to close file: %s", filename);
  return err;
}

int httpd_get_tag_from_multipart_form(char *inbuf, char *boundary, const char *tag, char *val, unsigned val_len)
{
  check_int(strlen(boundary), 1, "boundray is empty.");
  check_int(val_len, 1, "val_len should be positive integer.");
  check_int(24, strlen(tag), "tag name should be less than 24 characters.");

  char tag_start_label[64];
  sprintf(tag_start_label, "Content-Disposition: form-data; name=\"%s\"", tag);

  char *tag_start, *tag_content_start, *tag_end;
  safe_search_str(inbuf, tag_start_label, tag_start);
  safe_search_str(tag_start, "\r\n\r\n", tag_content_start);
  safe_search_str(tag_start, boundary, tag_end);

  // exclude "\r\n\r\n" from content start
  tag_content_start += 4;
  // exclude "\r\n--" from content end
  tag_end -= 4;

  int tag_content_len = tag_end - tag_content_start;
  if (tag_content_len < 0)
  {
    helper_log("No content for tag: %s", tag);
    return kNotFoundErr;
  }
  if (tag_content_len >= val_len)
  {
    helper_log("Target content length is larger than destination. Target content length: %d, destination length: %d", tag_content_len, val_len);
    return kNoSpaceErr;
  }

  if (tag_content_len > 0)
  {
    strncpy(val, tag_content_start, tag_content_len);
  }
  val[tag_content_len] = '\0';
  return kNoErr;
}
