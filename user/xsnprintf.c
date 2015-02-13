/**
  ******************************************************************************
  * @file    xsnprintf.c
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

#include <stdarg.h>
#include "esp_common.h"
#include "xsnprintf.h"


int ICACHE_FLASH_ATTR xsnprintf(char *str, int len, const char *fmt, ...)
{
    unsigned int r, i, j, w, f;
    unsigned long v;
    char s[16], c, d, *p;
    va_list arp;
    int pos = 0;

    va_start(arp, fmt);

    for (;;)
    {
        c = *fmt++;                 /* Get a char */
        if (!c) break;              /* End of format? */
        if (c != '%')               /* Pass through it if not a % sequence */
        {
            // xputc(c)
            if (pos < len - 1)
            {
                str[pos] = c;
                ++pos;
            }
            else
            {
                goto snprintf_exit;
            }
            continue;
        }
        f = 0;
        c = *fmt++;                 /* Get first char of the sequence */
        if (c == '0')               /* Flag: '0' padded */
        {
            f = 1; c = *fmt++;
        }
        else
        {
            if (c == '-')           /* Flag: left justified */
            {
                f = 2; c = *fmt++;
            }
        }
        for (w = 0; c >= '0' && c <= '9'; c = *fmt++)   /* Minimum width */
            w = w * 10 + c - '0';
        if (c == 'l' || c == 'L')   /* Prefix: Size is long int */
        {
            f |= 4; c = *fmt++;
        }
        if (!c) break;              /* End of format? */
        d = c;
        if (d >= 'a') d -= 0x20;
        switch (d)                  /* Type is... */
        {
        case 'S' :                  /* String */
            p = va_arg(arp, char*);
            for (j = 0; p[j]; j++) ;
            while (!(f & 2) && j++ < w)
            {
                // putc(' ');
                if (pos < len - 1)
                {
                    str[pos] = ' ';
                    ++pos;
                }
                else
                {
                    goto snprintf_exit;
                }
            }
            // xputs(p)
            while (*p)
            {
                if (pos < len - 1)
                {
                    str[pos] = *p++;
                    ++pos;
                }
                else
                {
                    goto snprintf_exit;
                }
            }
            while (j++ < w)
            {
                // xputc(' ');
                if (pos < len - 1)
                {
                    str[pos] = ' ';
                    ++pos;
                }
                else
                {
                    goto snprintf_exit;
                }
            }
            continue;
        case 'C' :                  /* Character */
            // xputc((char)va_arg(arp, int));
            if (pos < len - 1)
            {
                str[pos] = (char)va_arg(arp, int);
                ++pos;
            }
            else
            {
                goto snprintf_exit;
            }
            continue;
        case 'B' :                  /* Binary */
            r = 2; break;
        case 'O' :                  /* Octal */
            r = 8; break;
        case 'D' :                  /* Signed decimal */
        case 'U' :                  /* Unsigned decimal */
            r = 10; break;
        case 'X' :                  /* Hexdecimal */
            r = 16; break;
        default:                    /* Unknown type (passthrough) */
            // xputc(c);
            if (pos < len - 1)
            {
                str[pos] = c;
                ++pos;
            }
            else
            {
                goto snprintf_exit;
            }
            continue;
        }

        /* Get an argument and put it in numeral */
        v = (f & 4) ? va_arg(arp, long) : ((d == 'D') ? (long)va_arg(arp, int) : (long)va_arg(arp, unsigned int));
        if (d == 'D' && (v & 0x80000000))
        {
            v = 0 - v;
            f |= 8;
        }
        i = 0;
        do
        {
            d = (char)(v % r); v /= r;
            if (d > 9) d += (c == 'x') ? 0x27 : 0x07;
            s[i++] = d + '0';
        } while (v && i < sizeof(s));
        if (f & 8) s[i++] = '-';
        j = i; d = (f & 1) ? '0' : ' ';
        while (!(f & 2) && j++ < w)
        {
            // xputc(d);
            if (pos < len - 1)
            {
                str[pos] = d;
                ++pos;
            }
            else
            {
                goto snprintf_exit;
            }
        }
        do
        {
            // xputc(s[--i]);
            if (pos < len - 1)
            {
                str[pos] = s[--i];
                ++pos;
            }
            else
            {
                goto snprintf_exit;
            }
        } while(i);

        while (j++ < w)
        {
            // xputc(' ');
            if (pos < len - 1)
            {
                str[pos] = ' ';
                ++pos;
            }
            else
            {
                goto snprintf_exit;
            }
        }
    }
snprintf_exit:
    va_end(arp);
    str[pos] = '\0';
    return pos;
}

