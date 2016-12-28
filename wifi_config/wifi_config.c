/**
 ******************************************************************************
 *
 *  The MIT License
 *  Copyright (c) 2016 MXCHIP Inc.
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

void iot_connect_fail_handler(OSStatus err, mico_Context_t * const context)
{
  os_wifi_config_log("Wlan Connection Err %d", err);
  os_wifi_config_log("Erase WiFi setting and reset device.");
  context->flashContentInRam.micoSystemConfig.configured = unConfigured;
  mico_system_context_update(context);
  mico_system_power_perform(context, eState_Software_Reset);
}