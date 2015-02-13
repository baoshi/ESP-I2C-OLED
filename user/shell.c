/**
  ******************************************************************************
  * @file    shell.c
  * @author  Baoshi <mail(at)ba0sh1(dot)com>
  * @version 0.1
  * @date    Dec 16, 2014
  * @brief   Implementing a simple command line shell with basic VT100 editing
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
#include "dmsg.h"
#include "xmit.h"
#include "shell.h"


extern shell_command_t shell_commands[];    //!< Command-handler table

#define SHELL_RX_BUFFER_SIZE 63             //!< Command buffer length (<127)

/**
 * @brief Shell context type
 */
typedef struct _shell_ctx
{
    char             rx_buffer[SHELL_RX_BUFFER_SIZE + 1];       //!< Received character buffer
    int8_t           rx_buffer_pos;                             //!< Current buffer tail
    int8_t           rx_cursor_pos;                             //!< Command line cursor position
    char             command_line[SHELL_RX_BUFFER_SIZE + 1];    //!< Command line buffer
    shell_putchar_cb putc_cb;                                   //!< Echo callback
    bool             busy;                                      //!< If shell is busy processing previous command
} shell_ctx_t;

LOCAL shell_ctx_t ctx;  //!< Shell context


/** @brief   Output callback for zprintf.c */
LOCAL int ICACHE_FLASH_ATTR _z_out_cb(void *param, char c)
{
    if (ctx.putc_cb)
    {
        ctx.putc_cb(c);
        return 1;
    }
    return 0;
}


/**
 * @brief   Reset receive buffer
 */
LOCAL void ICACHE_FLASH_ATTR _reset_rx_buffer(void)
{
    ctx.rx_buffer_pos = 0;
    ctx.rx_cursor_pos = 0;
    memset(ctx.rx_buffer, 0, sizeof(char) * (SHELL_RX_BUFFER_SIZE + 1));
}


/**
 * @brief   Translate VT100 escape sequence into single character
 * @param   ch Input character
 * @return  true if ch is translated or not part of escape sequence.
 *          false if ch is part of escape sequence and should be ignored by the caller
 */
LOCAL bool ICACHE_FLASH_ATTR _translate_escape_sequence(char *ch)
{
    static enum {NORM, ESC, ESC_LB, ESC_LB_1, ESC_LB_3, ESC_LB_4 } state = NORM;
    switch (state)
    {
    case NORM:
        if (*ch == 0x1B) // ESC
        {
            state = ESC;
            return false;
        }
        else
        {
            return true;
        }
    case ESC:
        if (*ch =='[')
        {
            state = ESC_LB;
            return false;
        }
        else
        {
            state = NORM;
            return false; // invalid
        }
    case ESC_LB:
        switch (*ch)
        {
        case 'D':   // left arrow
            *ch = 0x02;
            state = NORM;
            return true;
        case 'C':   // right arrow
            *ch = 0x06;
            state = NORM;
            return true;
        case 'A':   // up arrow
            *ch = 0x10;
            state = NORM;
            return true;
        case 'B':   // down arrow
            *ch = 0x0E;
            state = NORM;
            return true;
        case '1':
            state = ESC_LB_1;
            return false;
        case '3':
            state = ESC_LB_3;
            return false;
        case '4':
            state = ESC_LB_4;
            return false;
        }
        state = NORM;   // unknown sequence
        return false;
    case ESC_LB_1:  // ESC[1~ is VT100+ home key (PUTTY)
        if (*ch == '~')
        {
            *ch = 0x01;
            state = NORM;
            return true;
        }
        else
        {
            state = NORM;   // unknown sequence
            return false;
        }
    case ESC_LB_3:  // ESC[3~ is VT100+ delete key (PUTTY)
        if (*ch == '~')
        {
            *ch = 0x7F;
            state = NORM;
            return true;
        }
        else
        {
            state = NORM;
            return false;
        }
    case ESC_LB_4:  // ESC[4~ is VT100+ end key (PUTTY)
        if (*ch == '~')
        {
            *ch = 0x05;
            state= NORM;
            return true;
        }
        else
        {
            state= NORM;
            return false;
        }
    }
    return false; // shall not reach here
}


LOCAL void ICACHE_FLASH_ATTR _welcome(void)
{
    shell_puts("\r\n\r\nWelcome to ESP8266 shell\r\n");
}


#define TOUPPER(x) (('a' <= (x) && (x) <= 'z') ? ((x) - 32) : (x))  //!< To upper case


/**
 * @brief   Search for a valid command and execute its handler
 */
LOCAL void ICACHE_FLASH_ATTR _execute_command(void)
{
    shell_errno_t err = SHELL_EUNKNOWN_COMMAND;
    int8_t i, len, len_cmd;
    const char *token;

    ctx.busy = true;

    token = ctx.command_line;
    len = 0;
    while ((*token) && (*token > 0x20) && (*token < 0x7F) && (len <= SHELL_RX_BUFFER_SIZE))
    {
        ++token;
        ++len;
    }
    if ((len > 0) && (len <= SHELL_RX_BUFFER_SIZE))
    {
        for (i = 0; i < len; ++i)
        {
           ctx.command_line[i] = TOUPPER(ctx.command_line[i]);
        }
        i = 0;
        while (shell_commands[i].cmd)
        {
            len_cmd = strlen(shell_commands[i].cmd);
            if ((len == len_cmd) && (strncmp(shell_commands[i].cmd, ctx.command_line, len) == 0))
            {
                while (!((*token == '\0') || ((*token > 0x20) && (*token < 0x7F)))) ++token;
                err = shell_commands[i].handler(token);
            }
            ++i;
        }
    }
    shell_show_err(err);  // will reset ctx.busy to false inside
}


void ICACHE_FLASH_ATTR shell_init(shell_putchar_cb putc_Cb)
{
    memset(&ctx, 0, sizeof(shell_ctx_t));
    ctx.putc_cb = putc_Cb;
    _welcome();
    shell_show_prompt();
}


void ICACHE_FLASH_ATTR shell_putc(char c)
{
    xfputc(_z_out_cb, 0, c);
}


void ICACHE_FLASH_ATTR shell_puts(const char *s)
{
    xfputs(_z_out_cb, 0, s);
}


void ICACHE_FLASH_ATTR shell_printf(const char *fmt, ...)
{
    va_list args;
    va_start(args, fmt);
    xvfprintf(_z_out_cb, 0, fmt, args);
    va_end(args);
}


void ICACHE_FLASH_ATTR shell_show_prompt(void)
{
    shell_putc('>');
}


void ICACHE_FLASH_ATTR shell_show_err(shell_errno_t err)
{
    switch (err)
    {
    case SHELL_EOK:
        shell_puts("OK\r\n");
        break;
    case SHELL_EUNKNOWN_COMMAND:
        shell_puts("Command not found. Type HELP to see all available commands.\r\n");
        break;
    case SHELL_EOVERFLOW:
        shell_puts("OVERFLOW\r\n");
        break;
    }
    shell_show_prompt();  // next prompt
    ctx.busy = false;
}


void ICACHE_FLASH_ATTR shell_process_char(char ch)
{
    int8_t i, repeat;

    // handle VT100 escape sequences
    if (!_translate_escape_sequence(&ch))
        return;

    // basic command line editing
    switch (ch)
    {
    case 0x03: // Ctrl-C
        _reset_rx_buffer();
        shell_puts("^C\r\n");
        shell_show_prompt();
        break;
    case 0x08: // backspace ^H
        if (ctx.rx_cursor_pos != 0)  // not at the beginning?
        {
            if (ctx.rx_cursor_pos == ctx.rx_buffer_pos) // Cursor at the end?
            {
                --ctx.rx_buffer_pos;
                --ctx.rx_cursor_pos;
                ctx.rx_buffer[ctx.rx_buffer_pos] = '\0';
                shell_puts("\x08 \x08"); // very tricky way of deleting last character from terminal
            }
            else // Cursor in the middle
            {
                repeat = 0;
                --ctx.rx_cursor_pos;
                shell_puts("\x1B[D"); // move left
                for (i = ctx.rx_cursor_pos; i < ctx.rx_buffer_pos; ++i)
                {
                    ctx.rx_buffer[i] = ctx.rx_buffer[i + 1];
                    if (ctx.rx_buffer[i]) // not NULL
                        shell_putc(ctx.rx_buffer[i]);
                    else
                        shell_putc(' ');
                    ++repeat;
                }
                --ctx.rx_buffer_pos;
                shell_printf("\x1B[%dD", repeat); // move cursor back to original cursor position
            }
        }
        break;
    case 0x04: // ^D
    case 0x7F: // delete ^?
        if (ctx.rx_cursor_pos < ctx.rx_buffer_pos)  // delete does not happen when cursor is at the end
        {
            repeat = 0;
            for (i = ctx.rx_cursor_pos; i < ctx.rx_buffer_pos; ++i)
            {
                ctx.rx_buffer[i] = ctx.rx_buffer[i + 1];
                if (ctx.rx_buffer[i]) // not NULL
                    shell_putc(ctx.rx_buffer[i]);
                else
                    shell_putc(' ');
                ++repeat;
            }
            --ctx.rx_buffer_pos;
            shell_printf("\x1B[%dD", repeat); // move cursor back to original cursor position
        }
        break;
    case 0x02: // left arrow ^B
        if (ctx.rx_cursor_pos != 0) // Not at the beginning
        {
            --ctx.rx_cursor_pos;
            shell_puts("\x1B[D"); // move left
        }
        break;
    case 0x06:  // right arrow ^F
        if (ctx.rx_cursor_pos < ctx.rx_buffer_pos)
        {
            ++ctx.rx_cursor_pos;
            shell_puts("\x1B[C");    // move right
        }
        break;
    case 0x01:  // home ^A
        if (ctx.rx_cursor_pos != 0)
        {
            shell_printf("\x1B[%dD", ctx.rx_cursor_pos); // move left
            ctx.rx_cursor_pos = 0;
        }
        break;
    case 0x05:  // end ^E
        if (ctx.rx_cursor_pos != ctx.rx_buffer_pos)
        {
            shell_printf("\x1B[%dC", ctx.rx_buffer_pos - ctx.rx_cursor_pos); // move right;
            ctx.rx_cursor_pos = ctx.rx_buffer_pos;
        }
        break;
    case '\n':  // LF
    case '\r':  // CR
        // If terminal sends CRLF, we have processed command during CR, LF can be ignored
        if ((ctx.rx_buffer_pos == 0) && (ch == '\n'))
            break;
        shell_puts("\r\n"); // CRLF ending
        if (ctx.rx_buffer_pos == 0) // if empty command
        {
            _reset_rx_buffer();
            shell_show_prompt();
            break;
        }
        if (ctx.busy)
        {
            // A previous command is being processed
            // TODO: Reset if requested
            // ignore other commands
            _reset_rx_buffer();
        }
        else // not busy
        {
            memcpy(ctx.command_line, ctx.rx_buffer, ctx.rx_buffer_pos);
            ctx.command_line[ctx.rx_buffer_pos] = '\0';
            _reset_rx_buffer();
            _execute_command();
        }
        break;
    default:
        if (ctx.rx_buffer_pos == SHELL_RX_BUFFER_SIZE) // overflow
        {
            shell_putc('\x07'); // BELL
            return;
        }
        if ((ch >= 0x20) && (ch < 0x7F)) // visible char
        {
            if (ctx.rx_cursor_pos == ctx.rx_buffer_pos) // append at end
            {
                shell_putc(ch);
                ctx.rx_buffer[ctx.rx_buffer_pos] = ch;
                ++ctx.rx_buffer_pos;
                ++ctx.rx_cursor_pos;
                ctx.rx_buffer[ctx.rx_buffer_pos] = '\0'; // safer?
            }
            else    // insert in the middle
            {
                repeat = ctx.rx_buffer_pos - ctx.rx_cursor_pos;
                shell_printf("\x1B[%dC", repeat);  // move right to end
                i = ctx.rx_buffer_pos;
                while (i != ctx.rx_cursor_pos - 1)
                {
                    ctx.rx_buffer[i + 1] = ctx.rx_buffer[i];
                    if (ctx.rx_buffer[i])
                    {
                        shell_putc(ctx.rx_buffer[i]);
                        shell_puts("\x1B[2D"); // move left twice
                    }
                    --i;
                }
                shell_putc(ch);
                ctx.rx_buffer[ctx.rx_cursor_pos] = ch;
                ++ctx.rx_buffer_pos;
                ++ctx.rx_cursor_pos;
                ctx.rx_buffer[ctx.rx_buffer_pos] = '\0'; // safer?
            }
        }
        else    // unknown/unhandled character
        {
            shell_putc('\x07'); // BELL
        }
    }
}
