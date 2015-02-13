/**
  ******************************************************************************
  * @file    dmsg.h
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

#ifndef DMSG_H_
#define DMSG_H_

/**
 * @brief   Initialize
 */
void dmsg_init(void);

/**
 * @brief   Output one character
 * @param   c Character to output
 */
void dmsg_putchar(char c);

/**
 * @brief   Output one string
 * @param   s String to output
 */
void dmsg_puts(const char *s);

/**
 * @brief   printf style output
 * @param   fmt printf format string
 */
void dmsg_printf(const char *fmt, ...);

/** @brief   Helper function, output "(I) xxxxx \r\n" */
#define dmsg_info_puts(x) { dmsg_puts("(I) "); dmsg_puts(x); dmsg_puts("\r\n"); }
/** @brief   Helper function, output "(W) xxxxx\r\n" */
#define dmsg_warn_puts(x) { dmsg_puts("(W) "); dmsg_puts(x); dmsg_puts("\r\n"); }
/** @brief   Helper function, output "(E) xxxxx\r\n" */
#define dmsg_err_puts(x)  { dmsg_puts("(E) "); dmsg_puts(x); dmsg_puts("\r\n"); }
/** @brief   Helper function, output "(I) xxxxx\r\n" in variable arguments style */
#define dmsg_info_printf(...) { dmsg_puts("(I) "); dmsg_printf(__VA_ARGS__); dmsg_puts("\r\n"); }
/** @brief   Helper function, output "(W) xxxxx\r\n" in variable arguments style */
#define dmsg_err_printf(...) { dmsg_puts("(W) "); dmsg_printf(__VA_ARGS__); dmsg_puts("\r\n"); }
/** @brief   Helper function, output "(E) xxxxx\r\n" in variable arguments style */
#define dmsg_warn_printf(...) { dmsg_puts("(E) "); dmsg_printf(__VA_ARGS__); dmsg_puts("\r\n"); }

#endif /* DMSG_H_ */
