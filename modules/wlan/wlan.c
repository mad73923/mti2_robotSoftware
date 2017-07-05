/*
 * wlan.c
 *
 *  Created on: 17.05.2017
 *      Author: thomas
 */

#include "wlan.h"

void(*wlanreadyCallback)(uint8_t);
void WlanConnectedToApCallback(uint8_t Error);
void WlanConnectedToTCPcallback(uint8_t Error);
void WlanHandledTCPmessageCallback(uint8_t Error);

volatile uint8_t WLANConnectionStatus = 0;
volatile uint8_t TCPConnectionStatus = 0;
char TCP_IP[20];
char TCP_Port[5];

void WLANinit(){
	ESP8266init();
}


void WLANconnectToAp(const char* SSID,const char* PW,const char* IP){
	wlanreadyCallback = WlanConnectedToApCallback;
	ESP8266connectToAp(SSID,PW,IP,wlanreadyCallback);
}

void WLANconnectToTCPserver(const char* IP, const char* Port){
	strcpy(TCP_IP,IP);
	strcpy(TCP_Port,Port);
	wlanreadyCallback = WlanConnectedToTCPcallback;
	ESP8266connectToTCPserver(IP, Port,wlanreadyCallback);
}

void WLANreconnectToTCPserver(){
	wlanreadyCallback = WlanConnectedToTCPcallback;
	ESP8266connectToTCPserver(TCP_IP, TCP_Port,wlanreadyCallback);
}

void WLANreconnectToWLAN(){
	wlanreadyCallback = WlanConnectedToApCallback;
	ESP8266reconnectToWLAN(wlanreadyCallback);
}

void WLANstartTCPlistener(){
	wlanreadyCallback = WlanHandledTCPmessageCallback;
	ESP8266startTCPlistener(wlanreadyCallback);
}

uint8_t WLANsendTCPmessage(const char* Message){
	return ESP8266sendTCPmessage(Message);
}

uint8_t WLANhandleTCP(void){
	return ESP8266handleTCP();
}



void WlanConnectedToApCallback(uint8_t Error){
	if(Error){
		debug_printf("WLAN connection failed! \n\r");
		WLANConnectionStatus = 0;
	}
	else{
		WLANConnectionStatus = 1;
		debug_printf("WLAN connection ready! \n\r");
	}
}

uint8_t WLANgetWLANConnectionStatus(){
	return WLANConnectionStatus;
}

void WlanConnectedToTCPcallback(uint8_t Error){
	if(Error){
	//	debug_printf("TCP failed! \n\r");
		TCPConnectionStatus = 0;
		WLANreconnectToTCPserver();
	}
	else{
		TCPConnectionStatus = 1;
		debug_printf("TCP connection ready! \n\r");
		WLANstartTCPlistener();
	}
}

uint8_t WLANgetTCPConnectionStatus(){
	return TCPConnectionStatus;
}

void WlanHandledTCPmessageCallback(uint8_t Error){
	if(Error==1){
		debug_printf("Handle TCP-Message failed! \n\r");
	}
	else if(Error==2){
		debug_printf("TCP-Connection destroyed! \n\r");
		TCPConnectionStatus = 0;
		WLANreconnectToTCPserver();
	}
	else if(Error==3){
		debug_printf("WLAN-Connection destroyed! \n\r");
		WLANConnectionStatus = 0;
		WLANreconnectToWLAN();
	}
	else{
		//debug_printf("Handled TCP-Message! \n\r");
		WLANstartTCPlistener();
	}
}
