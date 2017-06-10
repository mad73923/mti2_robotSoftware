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
uint8_t WLANconnectToAp(const char* SSID,const char* PW,const char* IP);
uint8_t WLANstartServer(const char* Port);
uint8_t WLANhandleConnectionsWithHtml(void);
const char* WLANgetIncomingTCPdata(void);
uint8_t WLANconnectToTCPserver(const char* IP, const char* Port);
uint8_t WLANsendUID(const char* UID);
uint8_t WLANsendDistances(uint16_t Dist[], uint16_t cntDistanceVal);
uint8_t WLANsendPos(int32_t xPos,int32_t yPos,float angle);
uint8_t WLANsendTCPmessage(const char* Message);
uint8_t WLANhandleTCP(void);


#endif /* WLAN_WLAN_H_ */
