/*
 * esp8266.h
 *
 *  Created on: 17.05.2017
 *      Author: thomas
 */

#ifndef WLAN_ESP8266_H_
#define WLAN_ESP8266_H_

#include "uart.h"

void ESP8266init();

uint8_t ESP8266createAP(const char* SSID,const char* PW);



#endif /* WLAN_ESP8266_H_ */
