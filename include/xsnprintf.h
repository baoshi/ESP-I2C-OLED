/**
  ******************************************************************************
  * @file    xsnprintf.h
  * @author  Baoshi <mail(at)ba0sh1(dot)com>
  * @version 0.1
  * @date    Dec 16, 2014
  * @brief   Implementation of snprintf which is missing from Espressif SDK
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

#ifndef XSNPRINTF_H_
#define XSNPRINTF_H_

/**
 * @brief Implementing snprintf
 * @param str   String buffer
 * @param len   Lens of str
 * @param fmt   Format string
 * @return      Output string length
 */
int xsnprintf(char *str, int len, const char *fmt, ...);

#endif /* XSNPRINTF_H_ */
