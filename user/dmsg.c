/**
  ******************************************************************************
  * @file    dmsg.c
  * @author  Baoshi <mail(at)ba0sh1(dot)com>
  * @version 0.1
  * @date    Dec 16, 2014
  * @brief   Some TX only functions for debug output,
  *          meant for use with ESP8266 UART1.
  *
  ******************************************************************************
  * @copyright
  *
  * Copyright (c) 2015, Baoshi Zhu. All rights reserved.
  * Use of this source code is governed by a BSD-style license that can be
  * found in the LICENSE.txt file.
  *
  * THIS SOFTWARE IS PROVIDED 'AS-IS', WITHOUT ANY EXPRESS OR IMPLIED
  * WARRANTY.  IN NO EVENT WILL THE AUTHOR(S) BE HELD LIABLE FOR ANY DAMAGES
  * ARISING FROM THE USE OF THIS SOFTWARE.
  *
  */

#include <stdarg.h>
#include "esp_common.h"
#include "uart.h"
#include "xmit.h"
#include "dmsg.h"


/** @brief   Output callback for zprintf.c */
LOCAL int ICACHE_FLASH_ATTR _z_out_cb(void *param, char c)
{
    uart1_putchar(c);
    return 1;
}


void ICACHE_FLASH_ATTR dmsg_init(void)
{
    uart1_init();
}


void ICACHE_FLASH_ATTR dmsg_putchar(char c)
{
    xfputc(_z_out_cb, 0, c);
}


void ICACHE_FLASH_ATTR dmsg_puts(const char *s)
{
    xfputs(_z_out_cb, 0, s);
}


void ICACHE_FLASH_ATTR dmsg_printf(const char *fmt, ...)
{
    va_list args;
    va_start(args, fmt);
    xvfprintf(_z_out_cb, 0, fmt, args);
    va_end(args);
}


