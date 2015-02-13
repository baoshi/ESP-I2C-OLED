/**
  ******************************************************************************
  * @file    oled.h
  * @author  Baoshi <mail(at)ba0sh1(dot)com>
  * @version 0.6
  * @date    Dec 29, 2014
  * @brief   SSD1306 OLED driver interface for ESP8266. For panel
  *          configuration, edit the panel option block in oled_i2c.c
  *
  ******************************************************************************
  * @copyright
  *
  * Many of the work here are based on Adafruit SSD1306 and GFX Arduino library.
  * Please support Adafruit by purchasing products from Adafruit!
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

#ifndef OLED_H
#define OLED_H



//! @brief Drawing color
typedef enum
{
    OLED_COLOR_TRANSPARENT = -1, //!< Transparent (not drawing)
    OLED_COLOR_BLACK = 0,        //!< Black (pixel off)
    OLED_COLOR_WHITE = 1,        //!< White (or blue, yellow, pixel on)
    OLED_COLOR_INVERT = 2,       //!< Invert pixel (XOR)
} oled_color_t;


/**
 * @brief   Initialize OLED panel
 * @param   id  Panel ID (0 or 1)
 * @return  true if successful
 * @remark  Possible reasons for failure include non-configured panel type, out of memory or I2C not responding
 */
bool oled_init(uint8_t id);


/**
 * @brief   De-initialize OLED panel, turn off power and free memory
 * @param   id  Panel ID (0 or 1)
 * @return  true if successful
 * @remark  Possible reasons for failure include non-configured panel type, out of memory or I2C not responding
 */
void oled_term(uint8_t id);


/**
 * @brief   Return OLED panel width
 * @param   id  Panel ID (0 or 1)
 * @return  Panel width, or return 0 if failed (panel not initialized)
 */
uint8_t oled_get_width(uint8_t id);


/**
 * @brief   Return OLED panel height
 * @param   id  Panel ID (0 or 1)
 * @return  Panel height, or return 0 if failed (panel not initialized)
 */
uint8_t oled_get_height(uint8_t id);

/**
 * @brief   Clear display buffer (fill with black)
 * @param   id Panel ID (0 or 1)
 */
void oled_clear(uint8_t id);

/**
 * @brief   Refresh display (send display buffer to the panel)
 * @param   id      Panel ID (0 or 1)
 * @param   force   The program automatically tracks "dirty" region to minimize refresh area. Set #force to true
 *                  ignores the dirty region and refresh the whole screen.
 */
void oled_refresh(uint8_t id, bool force);

/**
 * @brief   Draw one pixel
 * @param   id      Panel ID (0 or 1)
 * @param   x       X coordinate
 * @param   y       Y coordinate
 * @param   color   Color of the pixel
 */
void oled_draw_pixel(uint8_t id, int8_t x, int8_t y, oled_color_t color);


/**
 * @brief   Draw horizontal line
 * @param   id      Panel ID (0 or 1)
 * @param   x       X coordinate or starting (left) point
 * @param   y       Y coordinate or starting (left) point
 * @param   w       Line width
 * @param   color   Color of the line
 */
void oled_draw_hline(uint8_t id, int8_t x, int8_t y, uint8_t w, oled_color_t color);


/**
 * @brief   Draw vertical line
 * @param   id      Panel ID (0 or 1)
 * @param   x       X coordinate or starting (top) point
 * @param   y       Y coordinate or starting (top) point
 * @param   h       Line height
 * @param   color   Color of the line
 */
void oled_draw_vline(uint8_t id, int8_t x, int8_t y, uint8_t h, oled_color_t color);

/**
 * @brief   Draw a rectangle
 * @param   id      Panel ID (0 or 1)
 * @param   x       X coordinate or starting (top left) point
 * @param   y       Y coordinate or starting (top left) point
 * @param   w       Rectangle width
 * @param   h       Rectangle height
 * @param   color   Color of the rectangle border
 */
void oled_draw_rectangle(uint8_t id, int8_t x, int8_t y, uint8_t w, uint8_t h, oled_color_t color);


/**
 * @brief   Draw a filled rectangle
 * @param   id      Panel ID (0 or 1)
 * @param   x       X coordinate or starting (top left) point
 * @param   y       Y coordinate or starting (top left) point
 * @param   w       Rectangle width
 * @param   h       Rectangle height
 * @param   color   Color of the rectangle
 */
void oled_fill_rectangle(uint8_t id, int8_t x, int8_t y, uint8_t w, uint8_t h, oled_color_t color);


/**
 * @brief   Draw a circle
 * @param   id      Panel ID (0 or 1)
 * @param   x0      X coordinate or center
 * @param   y0      Y coordinate or center
 * @param   r       Radius
 * @param   color   Color of the circle
 */
void oled_draw_circle(uint8_t id, int8_t x0, int8_t y0, uint8_t r, oled_color_t color);

/**
 * @brief   Draw a filled circle
 * @param   id      Panel ID (0 or 1)
 * @param   x0      X coordinate or center
 * @param   y0      Y coordinate or center
 * @param   r       Radius
 * @param   color   Color of the circle
 */
void oled_fill_circle(uint8_t id, int8_t x0, int8_t y0, uint8_t r, oled_color_t color);



//! @fn
void oled_select_font(uint8_t id, uint8_t idx);
//! @fn
uint8_t oled_draw_char(uint8_t id, uint8_t x, uint8_t y, char c, oled_color_t foreground, oled_color_t background);
//! @fn
uint8_t oled_draw_string(uint8_t id, uint8_t x, uint8_t y, char *str, oled_color_t foreground, oled_color_t background);
//! @fn
uint8_t oled_measure_string(uint8_t id, char *str);
//! @fn
uint8_t oled_get_font_height(uint8_t id);
//! @fn
uint8_t oled_get_font_c(uint8_t id);

//! @fn
void oled_start_scroll_right(uint8_t id, uint8_t start, uint8_t stop);
//! @fn
void oled_start_scroll_left(uint8_t id, uint8_t start, uint8_t stop);
//! @fn
void oled_start_scroll_diag_right(uint8_t id, uint8_t start, uint8_t stop);
//! @fn
void oled_start_scroll_diag_left(uint8_t id, uint8_t start, uint8_t stop);
//! @fn
void oled_stop_scroll(uint8_t id);

//! @fn
void oled_invert_display(uint8_t id, bool invert);



#endif

