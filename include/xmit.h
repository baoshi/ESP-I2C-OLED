/**
  ******************************************************************************
  * @file    xmit.h
  * @author  Baoshi <mail(at)ba0sh1(dot)com>
  * @version 0.1
  * @date    Dec 16, 2014
  * @brief   Implementation of several callback-enabled transmit functions in
  *          standard C library fashion.
  *
  ******************************************************************************
  * @copyright
  *
  * Copyright (c) 2015, Baoshi Zhu. All rights reserved.
  * Use of this source code is governed by a BSD-style license that can be
  * found in the LICENSE.txt file.
  *
  * Many of the implementation here are based on Chan's excellent
  * <a href="http://elm-chan.org/fsw/strf/xprintf.html">xprintf</a> library
  *
  * THIS SOFTWARE IS PROVIDED 'AS-IS', WITHOUT ANY EXPRESS OR IMPLIED
  * WARRANTY.  IN NO EVENT WILL THE AUTHOR(S) BE HELD LIABLE FOR ANY DAMAGES
  * ARISING FROM THE USE OF THIS SOFTWARE.
  *
  */

#ifndef _XMIT_H
#define _XMIT_H_

#include <stdarg.h>

/**
 * @brief   fputc alike output function
 * @param   output_cb Output callback
 * @param   param     Parameter for output_cb
 * @param   c         Character to output
 * @return  Number of characters sent
 */
int xfputc(int (*output_cb)(void *, char), void *param, char c);

/**
 * @brief   fputs alike output function
 * @param   output_cb Output callback
 * @param   param     Parameter for output_cb
 * @param   str       String to output
 * @return  Number of characters sent
 */
int xfputs(int (*output_cb)(void *, char), void *param, const char* str);

/**
 * @brief   vfprintf alike output function
 * @param   output_cb Output callback
 * @param   param     Parameter for output_cb
 * @param   fmt       Format string
 * @param   arg       Variable arguments
 * @return  Number of characters sent
 */
int xvfprintf(int (*output_cb)(void *, char), void *param, const char* fmt, va_list arg);

/**
 * @brief   fprintf alike output function
 * @param   output_cb Output callback
 * @param   param     Parameter for output_cb
 * @param   fmt       Format string
 * @return  Number of characters sent
 */
int xfprintf(int (*output_cb)(void *, char), void *param, const char* fmt, ...);


#endif /* _XMIT_H_ */
