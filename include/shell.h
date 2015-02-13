/**
  ******************************************************************************
  * @file    shell.h
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

#ifndef SHELL_H_
#define SHELL_H_

/**
 * @brief   Minimal collection of error codes. User can extend the list
 */
typedef enum _shell_errno_t
{
    SHELL_EOK = 0,              //!< No error
    SHELL_EUNKNOWN_COMMAND = 1, //!< Unknown command
    SHELL_EOVERFLOW = 2         //!< Out of memory (or buffer)
} shell_errno_t;


/**
 * @brief   Callback function for shell to echo back one character
 */
typedef void (*shell_putchar_cb)(char);


/**
 * @brief   Shell command handler
 * @return  Error code
 */
typedef shell_errno_t (*shell_cmd_handler)(const char*);


/**
 * @brief   An entry in the commands table that defines the command and
 *          corresponding command handler function
 */
typedef struct _shell_command_t
{
    const char*       cmd;      //!< Command string
    shell_cmd_handler handler;  //!< Command handler function
} shell_command_t;


/**
 * @brief   Initialize shell
 * @param   putc_cb A callback function for shell to echo back characters
 */
void shell_init(shell_putchar_cb putc_cb);

/**
 * @brief   Echo one character
 * @param   ch Character to echo
 */
void shell_putc(char ch);

/**
 * @brief   Echo one string
 * @param   str String to echo
 */
void shell_puts(const char *str);

/**
 * @brief   printf style echo
 * @param   fmt Format string, in printf fashion
 */
void shell_printf(const char *fmt, ...);

/**
 * @brief   Process received character, user shall call this function when
 *          character received from UART or Socket
 * @param   ch Character received
 */
void shell_process_char(char ch);

/**
 * @brief   Show a shell prompt symbol (e.g., >)
 */
void shell_show_prompt(void);

/**
 * @brief   Display user readable error code
 * @param   err Error code
 */
void shell_show_err(shell_errno_t err);


#endif /* SHELL_H_ */
