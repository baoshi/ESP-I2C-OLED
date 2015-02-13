/*
 * uart.c
 *
 *  Created on: Dec 14, 2014
 *      Author: Baoshi
 */

#include "esp_common.h"
#include "uart.h"
#include "freertos/portmacro.h"


LOCAL STATUS ICACHE_FLASH_ATTR _uart_tx_one_char(uint8_t uart, char c)
{
    while (1)
    {
        uint32_t fifo_cnt = READ_PERI_REG(UART_STATUS(uart)) & (UART_TXFIFO_CNT << UART_TXFIFO_CNT_S);
        if ((fifo_cnt >> UART_TXFIFO_CNT_S & UART_TXFIFO_CNT) < 126)
        {
            break;
        }
    }
    WRITE_PERI_REG(UART_FIFO(uart) , c);
    return OK;
}


xQueueHandle xQueueUart;


LOCAL void _uart0_rx_isr(void)
{
    os_event_t e;
    portBASE_TYPE xHigherPriorityTaskWoken = pdFALSE;
    char ch;

    // Frame error, ignore and continue
    if (UART_FRM_ERR_INT_ST == (READ_PERI_REG(UART_INT_ST(UART0)) & UART_FRM_ERR_INT_ST))
    {
        // printf("FRM_ERR\r\n");
        WRITE_PERI_REG(UART_INT_CLR(UART0), UART_FRM_ERR_INT_CLR);
    }
    // RX FIFO FULL
    else if (UART_RXFIFO_FULL_INT_ST == (READ_PERI_REG(UART_INT_ST(UART0)) & UART_RXFIFO_FULL_INT_ST))
    {
        //os_printf("RX_FULL\r\n");
        while (READ_PERI_REG(UART_STATUS(UART0)) & (UART_RXFIFO_CNT << UART_RXFIFO_CNT_S))
        {
            ch = READ_PERI_REG(UART_FIFO(UART0)) & 0xFF;
            e.event = UART_EVENT_RX_CHAR;
            e.param = ch;
            xQueueSendFromISR(xQueueUart, (void *)&e, &xHigherPriorityTaskWoken);
            portEND_SWITCHING_ISR(xHigherPriorityTaskWoken);
        }
        WRITE_PERI_REG(UART_INT_CLR(UART0), UART_RXFIFO_FULL_INT_CLR);
    }
}


void ICACHE_FLASH_ATTR uart0_putchar(char ch)
{
    _uart_tx_one_char(UART0, ch);
}


void ICACHE_FLASH_ATTR uart0_puts(const char *str)
{
    while (*str)
    {
        uart0_putchar(*str++);
    }
}


void ICACHE_FLASH_ATTR uart0_init(uart_param_t* param)
{
    portENTER_CRITICAL();

    PIN_PULLUP_DIS(PERIPHS_IO_MUX_U0TXD_U);
    PIN_FUNC_SELECT(PERIPHS_IO_MUX_U0TXD_U, FUNC_U0TXD);

    WRITE_PERI_REG(UART_CLKDIV(UART0), UART_CLK_FREQ / (param->uart_baud_rate));

    WRITE_PERI_REG(UART_CONF0(UART0),
        param->uart_parity_mode
        |
        (param->uart_stop_bit << UART_STOP_BIT_NUM_S)
        |
        (param->uart_xfer_bit << UART_BIT_NUM_S));

    // Clear RX and TX FIFO
    SET_PERI_REG_MASK(UART_CONF0(UART0), UART_RXFIFO_RST | UART_TXFIFO_RST);
    CLEAR_PERI_REG_MASK(UART_CONF0(UART0), UART_RXFIFO_RST | UART_TXFIFO_RST);

    // Clear all interrupt
    WRITE_PERI_REG(UART_INT_CLR(UART0), 0x1ff);

    _xt_isr_attach(ETS_UART_INUM, _uart0_rx_isr);

    // RX interrupt conditions: FIFO full (1 char)
    SET_PERI_REG_MASK(UART_CONF1(UART0), ((0x01 & UART_RXFIFO_FULL_THRHD) << UART_RXFIFO_FULL_THRHD_S));

    // Enable RX Interrupts
    SET_PERI_REG_MASK(UART_INT_ENA(UART0), UART_RXFIFO_FULL_INT_ENA | UART_FRM_ERR_INT_ENA);

    _xt_isr_unmask(1 << ETS_UART_INUM);  // ETS_UART_INTR_ENABLE();

    xQueueUart = xQueueCreate(32, sizeof(os_event_t));

    portEXIT_CRITICAL();
}


void ICACHE_FLASH_ATTR uart1_putchar(char c)
{
    _uart_tx_one_char(UART1, c);
}


void ICACHE_FLASH_ATTR uart1_puts(const char *str)
{
    while (*str)
    {
        uart1_putchar(*str++);
    }
}


// UART1 is used for debug output, configuration fixed at 74880,N,1
void ICACHE_FLASH_ATTR uart1_init(void)
{
    portENTER_CRITICAL();

    PIN_FUNC_SELECT(PERIPHS_IO_MUX_GPIO2_U, FUNC_U1TXD_BK);

    WRITE_PERI_REG(UART_CLKDIV(UART1), UART_CLK_FREQ / UART_BAUD_RATE_74880);

    WRITE_PERI_REG(UART_CONF0(UART1),
        UART_PARITY_NONE
        |
        (UART_1_STOP_BIT << UART_STOP_BIT_NUM_S)
        |
        (UART_XFER_8_BIT << UART_BIT_NUM_S));

    // Clear RX and TX FIFO
    SET_PERI_REG_MASK(UART_CONF0(UART1), UART_RXFIFO_RST | UART_TXFIFO_RST);
    CLEAR_PERI_REG_MASK(UART_CONF0(UART1), UART_RXFIFO_RST | UART_TXFIFO_RST);

    // Clear all interrupt
    WRITE_PERI_REG(UART_INT_CLR(UART1), 0x1ff);

    // Use UART1 for system firmware output
    os_install_putc1((void *)uart1_putchar);

    portEXIT_CRITICAL();
}


