/**
 ******************************************************************************
 * @file    hello_world.c
 * @author  William Xu
 * @version V1.0.0
 * @date    21-May-2015
 * @brief   First MiCO application to say hello world!
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

#include "mico.h"
#include "duktapert/duktape.h"
#include "wifi_config/app_httpd.h"
#include "wifi_config/wifi_config.h"

#define os_helloworld_log(format, ...)  custom_log("helloworld", format, ##__VA_ARGS__)

void iot_custom_init(mico_Context_t* context)
{
  // Uncomment below line to hard code the WiFi you want to connect. Then the device won't go into AP mode.
  //iot_config_wifi(context, "MSFTLAB", "");

  OSStatus err = mico_system_notify_register(mico_notify_WIFI_CONNECT_FAILED, (void *)iot_connect_fail_handler, context);
  if (err != kNoErr)
  {
    os_helloworld_log("Fail to register WIFI connection fail handler.");
  }
}

void iot_run_duktape()
{
  system_log("A -- Free memory %d bytes", MicoGetMemoryInfo()->free_memory); 
  duk_context *ctx = duk_create_heap_default();
  system_log("B -- Free memory %d bytes", MicoGetMemoryInfo()->free_memory); 
  os_helloworld_log( "DUK CONTEXT CREATED!" );

  duk_eval_string(ctx, "var a = 10 + 20; if (123) { if (123) {if (123) {if (123) {if (123) {if (123) {}}}}}}");
  os_helloworld_log( "EVAL STRING CALLED" );
  duk_destroy_heap(ctx);
  os_helloworld_log( "HEAP DESTROYED");
  system_log("C -- Free memory %d bytes", MicoGetMemoryInfo()->free_memory);
}

int application_start( void )
{
  /* Start MiCO system functions according to mico_config.h*/
  mico_Context_t* mico_context = (mico_Context_t*) mico_system_context_init( 0 );

  iot_custom_init(mico_context);

  mico_system_init(mico_context);

  // start http server thread for WiFi configuration update
  app_httpd_start();

  /* Output on debug serial port */
  os_helloworld_log( "Hello world! XXXX" );

  iot_run_duktape();

  /* Trigger MiCO system led available on most MiCOKit */
  while(1)
  {
      MicoGpioOutputTrigger( MICO_SYS_LED );
      mico_thread_sleep(1);
  }
}
