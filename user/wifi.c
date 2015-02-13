/*
 * wifi.c
 *
 *  Created on: Dec 14, 2014
 *      Author: Baoshi
 */

#include "esp_common.h"
#include "wifi.h"

#define STA_SSID    "hackerspace.sg"
#define STA_PASSWORD  "xxxxxxxx"


void ICACHE_FLASH_ATTR wifi_init(void)
{
    wifi_set_opmode(STATION_MODE);
    {
        struct station_config *config = (struct station_config *)zalloc(sizeof(struct station_config));
        sprintf(config->ssid, STA_SSID);
        sprintf(config->password, STA_PASSWORD);
        /* need to sure that you are in station mode first,
        * otherwise it will fail. */
        wifi_station_set_config(config);
        free(config);
    }
}



