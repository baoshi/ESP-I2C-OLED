/*
 * oled_fonts.c
 *
 *  Created on: Jan 3, 2015
 *      Author: Baoshi
 */
#include "esp_common.h"
#include "oled_fonts.h"




extern const font_info_t glcd_5x7_font_info;
extern const font_info_t tahoma_8pt_font_info;

const font_info_t * oled_fonts[OLED_NUM_FONTS] =
{
    &glcd_5x7_font_info,
    &tahoma_8pt_font_info
};
