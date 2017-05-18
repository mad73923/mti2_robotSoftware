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

uint8_t ESP8266createAP(const char* SSID,const char* PW){
char buffer[30];

	//Set Mode to Soft AP
	UARTStartTransfers("AT+CWMODE_CUR=2\r\n");
	UARTwaitEndOfTransfer();
	if(UARTwaitForOkOrError(10000000)!=0){
		return 1;
	}
	strcpy(buffer,"AT+CWSAP_CUR=\"");
	strcat(buffer,SSID);
	strcat(buffer,"\",\"");
	strcat(buffer,PW);
	strcat(buffer,"\",5,3\r\n");
	UARTStartTransfers(buffer);
	if(UARTwaitForOkOrError(10000000)!=0){
			return 1;
	}
	return 0;
}

