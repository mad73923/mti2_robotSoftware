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
#include "../modules/uid/uid.h"

void ESP8266init();
uint8_t ESP8266connectToAp(const char* SSID,const char* PW,const char* IP, void(*ESP8266readyCallback)(uint8_t));
uint8_t ESP8266connectToTCPserver(const char* IP, const char* Port);
const char* ESP8266getIncomingTCPdata(void);
uint8_t ESP8266sendTCPmessage(const char* Message);
uint8_t ESP8266handleTCP(void);
uint16_t* generateDistances(void);




#endif /* WLAN_ESP8266_H_ */
