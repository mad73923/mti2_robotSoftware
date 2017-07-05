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
#include "../modules/odometry/odometry.h"
#include "../modules/motor/motor.h"
#include "../modules/env_sensor/env_sensor.h"

void ESP8266init();
void ESP8266connectToAp(const char* SSID,const char* PW,const char* IP, void(*ESP8266readyCallback)(uint8_t));
void ESP8266connectToTCPserver(const char* IP, const char* Port, void(*ESP8266readyCallback)(uint8_t));
const char* ESP8266getIncomingTCPdata(void);
uint8_t ESP8266sendTCPmessage(const char* Message);
uint8_t ESP8266handleTCP(void);
void ESP8266startTCPlistener(void(*ESP8266readyCallback)(uint8_t));
void ESP8266reconnectToWLAN(void(*ESP8266readyCallback)(uint8_t));
uint16_t* generateDistances(void);




#endif /* WLAN_ESP8266_H_ */
