/*
 * esp8266.h
 *
 *  Created on: 17.05.2017
 *      Author: thomas
 */

#ifndef WLAN_ESP8266_H_
#define WLAN_ESP8266_H_

#include "uart.h"
#include "../modules/debug/debug.h"

void ESP8266init();

uint8_t ESP8266createAP(const char* SSID,const char* PW,const char* IP);
uint8_t ESP8266connectToAp(const char* SSID,const char* PW,const char* IP);
uint8_t ESP8266startServer(const char* Port);
uint8_t ESP8266handleConnectionsWithHtml(void);
const char* ESP8266getIncomingTCPdata(void);
uint8_t ESP8266connectToTCPserver(const char* IP, const char* Port);
uint8_t ESP8266sendUID(const char* UID);
uint8_t ESP8266sendDistances(uint16_t Dist[],uint16_t cntDistanceVal);
uint8_t ESP8266sendPos(int32_t xPos,int32_t yPos,float angle);
uint8_t ESP8266sendTCPmessage(const char* Message);




#endif /* WLAN_ESP8266_H_ */
