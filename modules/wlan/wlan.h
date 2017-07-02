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
void WLANconnectToAp(const char* SSID,const char* PW,const char* IP);
void WLANconnectToTCPserver(const char* IP, const char* Port);
uint8_t WLANsendTCPmessage(const char* Message);
uint8_t WLANhandleTCP(void);
void WLANstartTCPlistener(void);
uint8_t WLANgetWLANConnectionStatus(void);
uint8_t WLANgetTCPConnectionStatus(void);


#endif /* WLAN_WLAN_H_ */
