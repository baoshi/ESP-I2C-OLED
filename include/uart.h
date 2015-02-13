/*
 * uart.h
 *
 *  This is based on ESP uart.h. But I removed some unused definitions and changed some
 *  constants based on Uart_reg_release_141118.xls
 *
 *  Created on: Dec 14, 2014
 *      Author: Baoshi
 */

#ifndef UART_H_
#define UART_H_

#include "freertos/FreeRTOS.h"
#include "freertos/queue.h"


#define UART0   0
#define UART1   1


// Number of stop bits, (bit[5:4], << UART_STOP_BIT_NUM_S)
typedef enum _UART_STOP_BIT
{
	UART_1_STOP_BIT    = 0x01,
	UART_1_5_STOP_BITS = 0x02,
	UART_2_STOP_BITS   = 0x03
} UART_STOP_BIT;


// Number of bits per transfer,(bit[3:2] << UART_BIT_NUM_S
typedef enum _UART_XFER_BIT
{
	UART_XFER_5_BIT = 0x0,
	UART_XFER_6_BIT = 0x1,
	UART_XFER_7_BIT = 0x02,
	UART_XFER_8_BIT = 0x03
} UART_XFER_BIT;


// Parity mode (bit[1:0])
typedef enum _UART_PARITY_MODE
{
	UART_PARITY_NONE	= 0x00,
	UART_PARITY_EVEN	= 0x02,
	UART_PARITY_ODD		= 0x03
} UART_PARITY_MODE;


// Baud rate
typedef enum _UART_BAUD_RATE
{
	UART_BAUD_RATE_9600     = 9600,
	UART_BAUD_RATE_19200   = 19200,
	UART_BAUD_RATE_38400   = 38400,
	UART_BAUD_RATE_57600   = 57600,
	UART_BAUD_RATE_74880   = 74880,
	UART_BAUD_RATE_115200 = 115200,
	UART_BAUD_RATE_230400 = 230400,
	UART_BAUD_RATE_256000 = 256000,
	UART_BAUD_RATE_460800 = 460800,
	UART_BAUD_RATE_921600 = 921600
} UART_BAUD_RATE;


// Flow control, not implemented
typedef enum _UART_FLOW_CONTROL
{
    UART_NONE_FLOW_CTRL,
    UART_HARDWARE_FLOW_CTRL,
    UART_XON_XOFF_FLOW_CTRL
} UART_FLOW_CONTROL;


typedef struct _uart_param_t
{
	UART_BAUD_RATE 	 	uart_baud_rate;
	UART_XFER_BIT    	uart_xfer_bit;
	UART_PARITY_MODE  	uart_parity_mode;
	UART_STOP_BIT  		uart_stop_bit;
	UART_FLOW_CONTROL	uart_flow_ctrl;
} uart_param_t;


/* FreeRTOS UART event stuff */

enum
{
    UART_EVENT_RX_CHAR,
    UART_EVENT_MAX
};

typedef struct _os_event_
{
    uint32_t event;
    uint32_t param;
} os_event_t;

extern xQueueHandle xQueueUart;


void uart0_init(uart_param_t* param);
void uart0_putchar(char ch);
void uart0_puts(const char *str);

void uart1_init(void);
void uart1_putchar(char ch);
void uart1_puts(const char *str);

#endif /* UART_H_ */
