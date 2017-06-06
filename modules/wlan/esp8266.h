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



#endif /* WLAN_ESP8266_H_ */
