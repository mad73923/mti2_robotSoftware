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
uint8_t WLANconnectToAp(const char* SSID,const char* PW,const char* IP);
uint8_t WLANconnectToTCPserver(const char* IP, const char* Port);
uint8_t WLANsendTCPmessage(const char* Message);
uint8_t WLANhandleTCP(void);


#endif /* WLAN_WLAN_H_ */
