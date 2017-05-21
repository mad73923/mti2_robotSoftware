/*
 * wlan.h
 *
 *  Created on: 17.05.2017
 *      Author: thomas
 */

#ifndef WLAN_WLAN_H_
#define WLAN_WLAN_H_

#include "esp8266.h"

void WLANinit();


uint8_t WLANcreateAP(const char* SSID,const char* PW,const char* IP);
uint8_t WLANstartServer(const char* Port);
uint8_t WLANhandleConnections(void);


#endif /* WLAN_WLAN_H_ */
