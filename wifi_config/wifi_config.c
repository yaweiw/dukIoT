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

#include "wifi_config.h"
#include "micokit_ext.h"

#define os_wifi_config_log(format, ...)  custom_log("wifi_config", format, ##__VA_ARGS__)

void iot_config_wifi(mico_Context_t* context, char* ssid, char* pass)
{
  int ssid_len = strlen(ssid);
  os_wifi_config_log("SSID %s, length %d.", ssid, ssid_len);
  if (ssid_len == 0 || ssid_len > maxSsidLen - 1)
  {
    os_wifi_config_log("Skip configuring WiFi due to invalid SSID.");
    return;
  }
  strncpy(context->flashContentInRam.micoSystemConfig.ssid, ssid, ssid_len + 1);

  int pass_len = strlen(pass);
  strncpy(context->flashContentInRam.micoSystemConfig.key, pass, pass_len);
  strncpy(context->flashContentInRam.micoSystemConfig.user_key, pass, pass_len);
  context->flashContentInRam.micoSystemConfig.keyLength = strlen(context->flashContentInRam.micoSystemConfig.key);
  context->flashContentInRam.micoSystemConfig.user_keyLength = strlen(context->flashContentInRam.micoSystemConfig.key);

  context->flashContentInRam.micoSystemConfig.channel = 0;
  memset(context->flashContentInRam.micoSystemConfig.bssid, 0x0, 6);
  context->flashContentInRam.micoSystemConfig.security = SECURITY_TYPE_AUTO;
  context->flashContentInRam.micoSystemConfig.dhcpEnable = true;

  context->flashContentInRam.micoSystemConfig.configured = allConfigured;
  mico_system_context_update(context);
}

void iot_show_ip_address(const char* ip)
{
  char oled_show_line[OLED_DISPLAY_MAX_CHAR_PER_ROW+1] = {'\0'};   // max char each line
  // Display IP in serial log
  os_wifi_config_log("Device IP: %s", ip);
  // Display IP at OLED
  snprintf(oled_show_line, OLED_DISPLAY_MAX_CHAR_PER_ROW+1, "@%s", ip);
  OLED_ShowString(OLED_DISPLAY_COLUMN_START, OLED_DISPLAY_ROW_4, oled_show_line);
}

void iot_blink_rgb_led(u_int8_t r, u_int8_t g, u_int8_t b, u_int8_t times)
{
  rgb_led_init();
  u_int8_t i = 0;
  while (i < times)
  {
    i++;
    rgb_led_open(r, g, b);
    mico_thread_msleep(500);
    rgb_led_open(0, 0, 0);
    mico_thread_msleep(500);
  }
}

void iot_connect_fail_handler(OSStatus err, mico_Context_t * const context)
{
  os_wifi_config_log("Wlan Connection Err %d", err);
  os_wifi_config_log("Erase WiFi setting and reset device.");
  context->flashContentInRam.micoSystemConfig.configured = unConfigured;
  mico_system_context_update(context);

  OLED_ShowString(OLED_DISPLAY_COLUMN_START, OLED_DISPLAY_ROW_4, "WiFi Connect ERR");
  iot_blink_rgb_led(255, 0, 0, 3); // Blink red LED

  mico_system_power_perform(context, eState_Software_Reset);
}

void iot_wifi_status_handler(WiFiEvent status, mico_Context_t * const context)
{
  switch(status)
  {
    case NOTIFY_STATION_UP:
      iot_show_ip_address(context->micoStatus.localIp);
      iot_blink_rgb_led(0, 255, 0, 3);  // Blink green LED
      break;
    case NOTIFY_AP_UP:
      iot_show_ip_address("10.10.10.1");
      iot_blink_rgb_led(0, 0, 255, 3);  // Blink blue LED
      break;
    case NOTIFY_STATION_DOWN:
      OLED_ShowString(OLED_DISPLAY_COLUMN_START, OLED_DISPLAY_ROW_4, "Station Down...");
      iot_blink_rgb_led(255, 0, 0, 3); // Blink red LED
      break;
    case NOTIFY_AP_DOWN:
      OLED_ShowString(OLED_DISPLAY_COLUMN_START, OLED_DISPLAY_ROW_4, "AP Down...");
      iot_blink_rgb_led(255, 0, 0, 3); // Blink red LED
      break;
  }
}
