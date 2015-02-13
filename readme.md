#SSD1305/1306 OLED driver for ESP8266

This project demostrates how to drive SSD1305/1306 based OLED panel using ESP8266. 
The project is build with ESP8266 RTOS SDK available at https://github.com/espressif/esp_iot_rtos_sdk, and compile using Espressif supplied VM.
Other useful modules within this project includes:
i2c.h/i2c.c is software bitbanged I2C driver
shell.h/shell.c/shell_cmds.c implements command line shell on UART0 (9600,8,N,1), with rudimentary line editing compatiblity.
dmsg.h/dmsg.c implements a simple tx-only debug output on UART1 (74480,8,N,1)
xmit/xsnprintf implements printf-alike output functions, based on Chan's excellent xprintf code.

If you feel this code is useful, please support my ESP8266 breadboard adapter on Tindie store:
https://www.tindie.com/products/Ba0sh1/esp8266-esp-0712-full-io-breadboard-adapter/

Copyright (c) 2015, Baoshi Zhu. All rights reserved.
Source code in this project governed by a BSD-style license that can be found in the LICENSE.txt file. 

 