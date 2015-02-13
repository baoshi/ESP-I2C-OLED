/**
  ******************************************************************************
  * @file    xmit.c
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
  * THIS SOFTWARE IS PROVIDED 'AS-IS', WITHOUT ANY EXPRESS OR IMPLIED
  * WARRANTY.  IN NO EVENT WILL THE AUTHOR(S) BE HELD LIABLE FOR ANY DAMAGES
  * ARISING FROM THE USE OF THIS SOFTWARE.
  *
  */

#include "esp_common.h"
#include "xmit.h"


int ICACHE_FLASH_ATTR xfputc(int (*output_cb)(void *, char), void *param, char c)
{
    return output_cb(param, c);
}


int ICACHE_FLASH_ATTR xfputs(int (*output_cb)(void *, char), void *param, const char* str)
{
    int ret  = 0, iret;
    while (*str)
    {
        if ((iret = output_cb(param, *str++)) > 0)
            ret += iret;
        else
        {
            // If there are output, we return number of output chars
            // Otherwise return error code
            // Caller can check return code to find partial output situation.
            ret = (ret == 0) ? iret : ret;
            break;
        }
    }
    return ret;
}


int ICACHE_FLASH_ATTR xvfprintf(int (*output_cb)(void *, char), void *param, const char* fmt, va_list arg)
{
    unsigned int r, i, j, w, f;
    unsigned long v;
    char s[16], c, d, *p;
    int ret = 0, iret;

    for (;;)
    {
        c = *fmt++;                 /* Get a char */
        if (!c) break;              /* End of format? */
        if (c != '%')               /* Pass through it if not a % sequence */
        {
            // xputc(c)
            if ((iret = output_cb(param, c)) > 0)
            {
                ret += iret;
            }
            else
            {
                ret = (ret == 0) ? iret : ret;
                goto vfprintf_exit;
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
            p = va_arg(arg, char*);
            for (j = 0; p[j]; j++) ;
            while (!(f & 2) && j++ < w)
            {
                // putc(' ');
                if ((iret = output_cb(param, ' ')) > 0)
                {
                    ret += iret;
                }
                else
                {
                    ret = (ret == 0) ? iret : ret;
                    goto vfprintf_exit;
                }
            }
            // xputs(p)
            while (*p)
            {
                if ((iret = output_cb(param, *p++)) > 0)
                    ret += iret;
                else
                {
                    ret = (ret == 0) ? iret : ret;
                    goto vfprintf_exit;
                }
            }
            while (j++ < w)
            {
                // xputc(' ');
                if ((iret = output_cb(param, ' ')) > 0)
                {
                    ret += iret;
                }
                else
                {
                    ret = (ret == 0) ? iret : ret;
                    goto vfprintf_exit;
                }
            }
            continue;
        case 'C' :                  /* Character */
            // xputc((char)va_arg(arp, int));
            if ((iret = output_cb(param, (char)va_arg(arg, int))) > 0)
            {
                ret += iret;
            }
            else
            {
                ret = (ret == 0) ? iret : ret;
                goto vfprintf_exit;
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
            if ((iret = output_cb(param, c)) > 0)
            {
                ret += iret;
            }
            else
            {
                ret = (ret == 0) ? iret : ret;
                goto vfprintf_exit;
            }
            continue;
        }

        /* Get an argument and put it in numeral */
        v = (f & 4) ? va_arg(arg, long) : ((d == 'D') ? (long)va_arg(arg, int) : (long)va_arg(arg, unsigned int));
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
            if ((iret = output_cb(param, d)) > 0)
            {
                ret += iret;
            }
            else
            {
                ret = (ret == 0) ? iret : ret;
                goto vfprintf_exit;
            }
        }
        do
        {
            // xputc(s[--i]);
            if ((iret = output_cb(param, s[--i])) > 0)
            {
                ret += iret;
            }
            else
            {
                ret = (ret == 0) ? iret : ret;
                goto vfprintf_exit;
            }
        } while(i);

        while (j++ < w)
        {
            // xputc(' ');
            if ((iret = output_cb(param, ' ')) > 0)
            {
                ret += iret;
            }
            else
            {
                ret = (ret == 0) ? iret : ret;
                goto vfprintf_exit;
            }
        }
    }
vfprintf_exit:
    return ret;
}


/*----------------------------------------------*/
/* Formatted string output                      */
/*----------------------------------------------*/
/*  zfprintf(..., "%d", 1234);            "1234"
    zfprintf(..., "%6d,%3d%%", -200, 5);  "  -200,  5%"
    zfprintf(..., "%-6u", 100);           "100   "
    zfprintf(..., "%ld", 12345678L);      "12345678"
    zfprintf(..., "%04x", 0xA3);          "00a3"
    zfprintf(..., "%08LX", 0x123ABC);     "00123ABC"
    zfprintf(..., "%016b", 0x550F);       "0101010100001111"
    zfprintf(..., "%s", "String");        "String"
    zfprintf(..., "%-4s", "abc");         "abc "
    zfprintf(..., "%4s", "abc");          " abc"
    zfprintf(..., "%c", 'a');             "a"
*/
int ICACHE_FLASH_ATTR xfprintf(int (*output_cb)(void *, char), void *param, const char* fmt, ...)
{
    int ret;
    va_list args;
    va_start(args, fmt);
    ret = xvfprintf(output_cb, param, fmt, args);
    va_end(args);
    return ret;
}



