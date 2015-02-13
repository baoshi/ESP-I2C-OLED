/*
 * shell_cmds.c
 *
 *  Created on: Dec 20, 2014
 *      Author: Baoshi
 */


#include "esp_common.h"
#include "ssd1306.h"
#include "dmsg.h"
#include "shell.h"



shell_errno_t ICACHE_FLASH_ATTR shell_do_help(const char* args)
{
    if (args && *args)
    {
        dmsg_info_printf("Show help(%s)\r\n", args)
    }
    else
    {
        dmsg_info_puts("Show help\r\n");
    }
    return SHELL_EOK;
}


shell_errno_t ICACHE_FLASH_ATTR shell_do_status(const char* args)
{
    struct ip_info info;
    dmsg_info_puts("Show status\r\n");
    if (wifi_get_ip_info(STATION_IF, &info))
    {
        shell_puts("Station IP   : "); shell_printf(IPSTR, IP2STR(&(info.ip))); shell_puts("\r\n");
        shell_puts("Station GW   : "); shell_printf(IPSTR, IP2STR(&(info.gw))); shell_puts("\r\n");
        shell_puts("Station Mask : "); shell_printf(IPSTR, IP2STR(&(info.netmask))); shell_puts("\r\n");
    }
    else
    {
        shell_puts("wifi_get_ip_info failed.\r\n");
    }
    return SHELL_EOK;
}


shell_errno_t ICACHE_FLASH_ATTR shell_do_reset(const char* args)
{
    system_restart();
    return SHELL_EOK;
}


shell_errno_t ICACHE_FLASH_ATTR shell_do_write(const char* args)
{
    // Testing 24L02 write
    uint8_t ad, i;
    ad = 0;
    bool ack;
    do
    {
        ack = false;
        while (!ack) // wait until device acknowledge
        {
            i2c_start();
            ack = i2c_write(0xA0);
            if (!ack)
                os_delay_us(1000);
        }
        i2c_write(ad);
        for (i = 0; i < 8; ++i)
        {
            i2c_write(ad);
            ++ad;
        }
        i2c_stop();
        os_delay_us(4000);
    } while (ad != 0);
    return SHELL_EOK;
}


shell_errno_t ICACHE_FLASH_ATTR shell_do_read(const char* args)
{
    // Testing 24L02 read
    uint8_t i;
    uint8_t c[256];
    i2c_start();
    i2c_write(0xA0); // write
    i2c_write(0x00); // address
    i2c_start();
    i2c_write(0xA1); // read
    for (i = 0; i != 255; ++i)
    {
        c[i] = i2c_read();
        i2c_set_ack(true);
    }
    c[i] = i2c_read();
    i2c_set_ack(false);
    i2c_stop();
    i = 0;
    do
    {
        shell_printf("%02X ", c[i]);
        if ((i & 0x07) == 0x07)
            shell_puts("\r\n");
        ++i;
    } while (i != 0);
    return SHELL_EOK;
}


shell_errno_t ICACHE_FLASH_ATTR shell_do_init(const char* args)
{
    if (ssd1306_init(0))
        shell_puts("Init OLED0 OK\r\n");
    else
        shell_puts("Init OLED0 failed\r\n");
    if (ssd1306_init(1))
        shell_puts("Init OLED1 OK\r\n");
    else
        shell_puts("Init OLED1 failed\r\n");
    return SHELL_EOK;
}


shell_errno_t ICACHE_FLASH_ATTR shell_do_term(const char* args)
{
    ssd1306_term(0);
    ssd1306_term(1);
    return SHELL_EOK;
}


shell_errno_t ICACHE_FLASH_ATTR shell_do_draw(const char* args)
{
    int8_t w;
    ssd1306_select_font(0, 1);
    w = ssd1306_measure_string(0, "www.ba0sh1.com");
    ssd1306_draw_string(0, (ssd1306_get_width(0) - w) / 2, 12, "www.ba0sh1.com", SSD1306_COLOR_WHITE, SSD1306_COLOR_BLACK);
    ssd1306_refresh(0, false);

    for (w = 40; w > 10; w -= 5)
        ssd1306_draw_circle(1, 64, 40, w, SSD1306_COLOR_WHITE);
    ssd1306_select_font(1, 1);
    ssd1306_fill_rectangle(1, 0, 0, 128, 16, SSD1306_COLOR_WHITE);
    w = ssd1306_measure_string(1, "www.ba0sh1.com");
    ssd1306_draw_string(1, (ssd1306_get_width(1) - w) / 2, 4, "www.ba0sh1.com", SSD1306_COLOR_BLACK, SSD1306_COLOR_TRANSPARENT);
    ssd1306_refresh(1, false);
    return SHELL_EOK;
}



shell_command_t shell_commands[] =
{
    {"HELP", shell_do_help},
    {"STATUS", shell_do_status},
    {"RESET", shell_do_reset},
    {"INIT", shell_do_init},
    {"TERM", shell_do_term},
    {"DRAW", shell_do_draw},
    {0, 0}
};
