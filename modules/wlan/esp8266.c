/*
 * esp8266.c
 *
 *  Created on: 17.05.2017
 *      Author: thomas
 */

#include "esp8266.h"

void ESP8266init(){
	UARTinit();
}

uint8_t ESP8266createAP(const char* SSID,const char* PW,const char* IP){
	char buffer[80];
	uint8_t returnval = 0;
	//Set Mode to Soft AP
	UARTStartTransfers("AT+CWMODE_CUR=2\r\n");
	UARTwaitEndOfTransfer();
	returnval = UARTwaitForOkOrError(100000);
	if(returnval!=0){
		return returnval;
	}
	strcpy(buffer,"AT+CWSAP_CUR=\"");
	strcat(buffer,SSID);
	strcat(buffer,"\",\"");
	strcat(buffer,PW);
	strcat(buffer,"\",5,3\r\n");
	UARTStartTransfers(buffer);
	UARTwaitEndOfTransfer();
	returnval = UARTwaitForOkOrError(100000);
	if(returnval!=0){
			return returnval;
	}
	strcpy(buffer,"AT+CIPAP_CUR=\"");
	strcat(buffer,IP);
	strcat(buffer,"\",\"");
	strcat(buffer,IP);
	strcat(buffer,"\",\"255.255.255.0\"\r\n");
	UARTStartTransfers(buffer);
	UARTwaitEndOfTransfer();
	returnval = UARTwaitForOkOrError(100000);
	if(returnval!=0){
			return returnval;
	}
	return returnval;
}

uint8_t ESP8266startServer(const char* Port){
	char buffer[80];
	uint8_t returnval = 0;
	//Set Mode to Soft AP
	UARTStartTransfers("AT+CIPMUX=1\r\n");
	UARTwaitEndOfTransfer();
	returnval = UARTwaitForOkOrError(100000);
	if(returnval!=0){
		return returnval;
	}
	strcpy(buffer,"AT+CIPSERVER=1,");
	strcat(buffer,Port);
	strcat(buffer,"\r\n");
	UARTStartTransfers(buffer);
	UARTwaitEndOfTransfer();
	returnval = UARTwaitForOkOrError(100000);
	if(returnval!=0){
			return returnval;
	}
	return returnval;
}
