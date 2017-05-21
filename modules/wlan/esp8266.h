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
uint8_t ESP8266startServer(const char* Port);
uint8_t ESP8266handleConnections(void);


#endif /* WLAN_ESP8266_H_ */
