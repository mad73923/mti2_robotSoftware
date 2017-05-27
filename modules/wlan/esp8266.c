/*
 * esp8266.c
 *
 *  Created on: 17.05.2017
 *      Author: thomas
 */

#include "esp8266.h"
#include "html.h"

void ESP8266init(){
	UARTinit();
}

uint8_t ESP8266createAP(const char* SSID,const char* PW,const char* IP){
	char buffer[80];
	uint8_t returnval = 0;
	//Set Mode to Soft AP
	UARTStartTransfers("AT+RST\r\n");
	UARTwaitEndOfTransfer();
	returnval = UARTwaitForReady(500000);
	if(returnval!=0){
		return returnval;
	}
	UARTStartTransfers("ATE0\r\n");
	UARTwaitEndOfTransfer();
	returnval = UARTwaitForOkOrError(500000);
	if(returnval!=0){
		return returnval;
	}
	UARTStartTransfers("AT+CWMODE_CUR=2\r\n");
	UARTwaitEndOfTransfer();
	returnval = UARTwaitForOkOrError(500000);
	if(returnval!=0){
		return returnval;
	}
	debug_printf("Setting AP-Mode successful!\n\r");
	strcpy(buffer,"AT+CWSAP_CUR=\"");
	strcat(buffer,SSID);
	strcat(buffer,"\",\"");
	strcat(buffer,PW);
	strcat(buffer,"\",5,3\r\n");
	UARTStartTransfers(buffer);
	UARTwaitEndOfTransfer();
	returnval = UARTwaitForOkOrError(500000);
	if(returnval!=0){
			return returnval;
	}
	debug_printf("Setting SSID and Password successful!\n\r");
	strcpy(buffer,"AT+CIPAP_CUR=\"");
	strcat(buffer,IP);
	strcat(buffer,"\",\"");
	strcat(buffer,IP);
	strcat(buffer,"\",\"255.255.255.0\"\r\n");
	UARTStartTransfers(buffer);
	UARTwaitEndOfTransfer();
	returnval = UARTwaitForOkOrError(500000);
	if(returnval!=0){
			return returnval;
	}
	debug_printf("Setting IP-Address successful!\n\r");
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
	debug_printf("Setting multiple connections enable successful!\n\r");
	strcpy(buffer,"AT+CIPSERVER=1,");
	strcat(buffer,Port);
	strcat(buffer,"\r\n");
	UARTStartTransfers(buffer);
	UARTwaitEndOfTransfer();
	returnval = UARTwaitForOkOrError(100000);
	if(returnval!=0){
			return returnval;
	}
	debug_printf("Activating Server successful!\n\r");
	return returnval;
}

uint8_t ESP8266handleConnections(void){
	uint8_t returnval = -1;
	uint8_t client = UARTcheckForNewConnection();
	if(client<=3){
		debug_printf("Incoming Connection from client %d\n\r", client);
		//Antwort senden!
		char buffer[80];
		char buffer2[10];
		strcpy(buffer,"AT+CIPSEND=");
		buffer2[0] = client + '0';
		buffer2[1] = '\0';
		strcat(buffer,buffer2);
		strcat(buffer,",");
		sprintf(buffer2,"%d\0",strlen(webpage));
		strcat(buffer,buffer2);
		strcat(buffer,"\r\n");
		UARTStartTransfers(buffer);
		UARTwaitEndOfTransfer();
		returnval = UARTwaitForStartIndicator(100000);
		if(returnval!=0){
			return returnval;
		}
		debug_printf("Sending...\n\r");
		UARTStartTransfers(webpage);
		UARTwaitEndOfTransfer();
		returnval = UARTwaitForSendOK(10000);
			if(returnval!=0){
				return returnval;
			}
		debug_printf("Closing connection...\n\r");
		strcpy(buffer,"AT+CIPCLOSE=");
		buffer2[0] = client + '0';
		buffer2[1] = '\0';
		strcat(buffer,buffer2);
		strcat(buffer,"\r\n");
		UARTStartTransfers(buffer);
		UARTwaitEndOfTransfer();
	}
	return returnval;
}

