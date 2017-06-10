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

uint8_t ESP8266connectToAp(const char* SSID,const char* PW,const char* IP){
	char buffer[80];
	uint8_t returnval = 0;
	//Set Mode to Soft AP
	UARTStartTransfers("AT+RST\r\n");
	UARTwaitEndOfTransfer();
	returnval = UARTwaitForReady(500000);
	if(returnval!=0){
		return returnval;
	}
	UARTStartTransfers("ATE1\r\n");
	UARTwaitEndOfTransfer();
	returnval = UARTwaitForOkOrError(500000);
	if(returnval!=0){
		return returnval;
	}
	UARTStartTransfers("AT+CWMODE_CUR=1\r\n");
	UARTwaitEndOfTransfer();
	returnval = UARTwaitForOkOrError(500000);
	if(returnval!=0){
		return returnval;
	}
	debug_printf("Setting STA-Mode successful!\n\r");
	strcpy(buffer,"AT+CWJAP_CUR=\"");
	strcat(buffer,SSID);
	strcat(buffer,"\",\"");
	strcat(buffer,PW);
	strcat(buffer,"\"\r\n");
	UARTStartTransfers(buffer);
	UARTwaitEndOfTransfer();
	returnval = UARTwaitForOkOrError(2000000);
	if(returnval!=0){
			return returnval;
	}
	debug_printf("Connecting to AP successful!\n\r");
	strcpy(buffer,"AT+CIPSTA_CUR=\"");
	strcat(buffer,IP);
	strcat(buffer,"\"\r\n");
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

uint8_t ESP8266handleConnectionsWithHtml(void){
	uint8_t returnval = -1;
	uint8_t client = UARTcheckForNewConnection();
	if(client<=3){
		debug_printf("Incoming Connection from client %d\n\r", client);
		//Antwort senden!
		char buffer[80];
		char buffer2[10];
		strcpy(buffer,"AT+CIPSEND=");
		buffer2[0] = client + '0'; //Client to ASCII
		buffer2[1] = '\0';
		strcat(buffer,buffer2);
		strcat(buffer,",");
		sprintf(buffer2,"%d",strlen(webpage));
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

uint8_t ESP8266connectToTCPserver(const char* IP, const char* Port){
	char buffer[80];
	uint8_t returnval = 0;
	UARTStartTransfers("AT+CIPMUX=0\r\n");
	UARTwaitEndOfTransfer();
	returnval = UARTwaitForOkOrError(100000);
	if(returnval!=0){
		return returnval;
	}
	UARTStartTransfers("AT+CIPCLOSE\r\n");
	UARTwaitEndOfTransfer();
	returnval = UARTwaitForOkOrError(100000);
	if(returnval==0){
		debug_printf("TCP-Connection closed!\r\n");
	}
	debug_printf("Setting multiple connections disable successful!\n\r");
	strcpy(buffer,"AT+CIPSTART=\"TCP\",\""); //192.168.101.110", 1000");
	strcat(buffer,IP);
	strcat(buffer,"\",");
	strcat(buffer,Port);
	strcat(buffer,"\r\n");
	UARTStartTransfers(buffer);
	UARTwaitEndOfTransfer();
	returnval = UARTwaitForOkOrError(100000);
	if(returnval!=0){
			return returnval;
	}
	debug_printf("Connection to Server successful!\n\r");
	return returnval;
}

const char* ESP8266getIncomingTCPdata(void){
	const char* returnval = NULL;
	returnval = UARTCheckForIPD();
	return returnval;
}

uint8_t ESP8266sendUID(const char* UID){
	uint8_t returnval = 0;
	char buffer[80];
	char buffer2[5];
	strcpy(buffer,"AT+CIPSEND=");
	sprintf(buffer2,"%d",strlen(UID)+4);
	strcat(buffer,buffer2);
	strcat(buffer,"\r\n");
	UARTStartTransfers(buffer);
	UARTwaitEndOfTransfer();
	returnval = UARTwaitForStartIndicator(100000);
	if(returnval!=0){
		return returnval;
	}
	debug_printf("Sending UID...\n\r");
	strcpy(buffer,"UID=");
	strcat(buffer,UID);
	UARTStartTransfers(buffer);
	UARTwaitEndOfTransfer();
	returnval = UARTwaitForSendOK(10000);
		if(returnval!=0){
			return returnval;
		}
	return returnval;
}

uint8_t ESP8266sendDistances(uint16_t Dist[],uint16_t cntDistanceVal){
	uint8_t returnval = 0;
	char bufferMessage[1450];
	char buffer[80];
	char buffer2[10];
	strcpy(bufferMessage,"ActDistances=[");
	for(int i =0;i<cntDistanceVal;i++){
		if(i<(cntDistanceVal-1)){
			sprintf(buffer2,"%d,",Dist[i]);
		}
		else{
			sprintf(buffer2,"%d]",Dist[i]);
		}
		strcat(bufferMessage,buffer2);
	}
	strcpy(buffer,"AT+CIPSEND=");
	sprintf(buffer2,"%d",strlen(bufferMessage));
	strcat(buffer,buffer2);
	strcat(buffer,"\r\n");
	UARTStartTransfers(buffer);
	UARTwaitEndOfTransfer();
	returnval = UARTwaitForStartIndicator(100000);
	if(returnval!=0){
		return returnval;
	}
	debug_printf("Sending ActDist...\n\r");
	//debug_printf(bufferMessage);
	UARTStartTransfers(bufferMessage);
	UARTwaitEndOfTransfer();
	returnval = UARTwaitForSendOK(10000000);
		if(returnval!=0){
			return returnval;
		}
	return returnval;
}

uint8_t ESP8266sendPos(int32_t xPos,int32_t yPos,float angle){
	uint8_t returnval = 0;
	char bufferMessage[80];
	char buffer[80];
	char buffer2[5];
	strcpy(bufferMessage,"ActPos=");
	sprintf(buffer2,"%ld,",xPos);
	strcat(bufferMessage,buffer2);
	sprintf(buffer2,"%ld,",yPos);
	strcat(bufferMessage,buffer2);
	sprintf(buffer2,"%.1f",angle);
	strcat(bufferMessage,buffer2);
	strcpy(buffer,"AT+CIPSEND=");
	sprintf(buffer2,"%d",strlen(bufferMessage));
	strcat(buffer,buffer2);
	strcat(buffer,"\r\n");
	UARTStartTransfers(buffer);
	UARTwaitEndOfTransfer();
	returnval = UARTwaitForStartIndicator(100000);
	if(returnval!=0){
		return returnval;
	}
	debug_printf("Sending ActPos...\n\r");
	UARTStartTransfers(bufferMessage);
	UARTwaitEndOfTransfer();
	returnval = UARTwaitForSendOK(10000);
		if(returnval!=0){
			return returnval;
		}
	return returnval;
}

uint8_t ESP8266sendTCPmessage(const char* Message){
	uint8_t returnval = 0;
	char buffer[80];
	char buffer2[5];
	strcpy(buffer,"AT+CIPSEND=");
	sprintf(buffer2,"%d\r\n",strlen(Message));
	strcat(buffer,buffer2);
	UARTStartTransfers(buffer);
	UARTwaitEndOfTransfer();
	returnval = UARTwaitForStartIndicator(100000);
	if(returnval!=0){
		return returnval;
	}
	debug_printf("Sending Message...\n\r");
	UARTStartTransfers(Message);
	UARTwaitEndOfTransfer();
	returnval = UARTwaitForSendOK(10000);
		if(returnval!=0){
			return returnval;
		}
	return returnval;
}

enum TCP_state {waiting_for_data,
	wait_for_UID_startindicator,
	wait_for_UID_SEND_OK,
	wait_for_Pos_startindicator,
	wait_for_Pos_SEND_OK,
	wait_for_Dist_startindicator,
	wait_for_Dist_SEND_OK,};
enum TCP_state actTCPstate = waiting_for_data;

char TCPbuffer[1450];
char TCPbuffer2[5];
char TCPbuffer3[20];

uint8_t ESP8266handleTCP(void){

	uint8_t temp = 0;
	switch(actTCPstate){
	case waiting_for_data:{
		const char* ptrMessage = ESP8266getIncomingTCPdata();
		if(ptrMessage==NULL){break;}				//if no new data recieved return
		debug_printf("Data revieved: %s\n\r", ptrMessage );		//else send recieved message over debug-uart

		if(strcmp(ptrMessage,"GetUID?")==0){
			debug_printf("Robot was asked for UID\n\r");
			strcpy(TCPbuffer,"AT+CIPSEND=");
			sprintf(TCPbuffer2,"%d",strlen(uid_getUIDString())+4);
			strcat(TCPbuffer,TCPbuffer2);
			strcat(TCPbuffer,"\r\n");
			debug_printf(TCPbuffer);
			UARTStartTransfers(TCPbuffer);
			actTCPstate = wait_for_UID_startindicator;
			break;
		}

		if(strcmp(ptrMessage,"GetPos?")==0){
			debug_printf("Robot was asked for Pos\n\r");
			strcpy(TCPbuffer,"ActPos=");
			sprintf(TCPbuffer2,"%ld,",123);
			strcat(TCPbuffer,TCPbuffer2);
			sprintf(TCPbuffer2,"%ld,",345);
			strcat(TCPbuffer,TCPbuffer2);
			sprintf(TCPbuffer2,"%.1f",67.8);
			strcat(TCPbuffer,TCPbuffer2);
			strcpy(TCPbuffer3,"AT+CIPSEND=");
			sprintf(TCPbuffer2,"%d",strlen(TCPbuffer));
			strcat(TCPbuffer3,TCPbuffer2);
			strcat(TCPbuffer3,"\r\n");
			UARTStartTransfers(TCPbuffer3);
			actTCPstate = wait_for_Pos_startindicator;
			break;
		}

		if(strcmp(ptrMessage,"GetDistances?")==0){
			debug_printf("Robot was asked for Distances\n\r");
			strcpy(TCPbuffer,"ActDistances=[");
			uint16_t Dist[36];
			for(int i=0;i<36;i++){
					Dist[i] = i*20;
			}
			for(int i =0;i<36;i++){
				if(i<(36-1)){
					sprintf(TCPbuffer2,"%d,",Dist[i]);
				}
				else{
					sprintf(TCPbuffer2,"%d]",Dist[i]);
				}
				strcat(TCPbuffer,TCPbuffer2);
			}
			strcpy(TCPbuffer3,"AT+CIPSEND=");
			sprintf(TCPbuffer2,"%d",strlen(TCPbuffer));
			strcat(TCPbuffer3,TCPbuffer2);
			strcat(TCPbuffer3,"\r\n");
			UARTStartTransfers(TCPbuffer3);
			actTCPstate = wait_for_Dist_startindicator;
			break;
		}
		debug_printf("Recieved not known command!");
		break;
	}

	case wait_for_UID_startindicator:{
		if(UARTcheckEndOfTransfer()==1){
			break;	//if not completely sent
		}
		temp = UARTcheckForStartIndicator();
		if(temp==1){	//If no > and no ERROR was recieved return
			break;
		}
		if(temp==2){	//If ERROR was recieved
			debug_printf("ERROR recieved when trying to send UID!\n\r");
			actTCPstate = waiting_for_data;
			break;
		}
		//if > recieved start sending UID
		debug_printf("Start sending UID...\n\r");
		strcpy(TCPbuffer,"UID=");
		strcat(TCPbuffer,uid_getUIDString());
		//debug_printf(buffer);
		UARTStartTransfers(TCPbuffer);
		actTCPstate = wait_for_UID_SEND_OK;
		break;
	}

	case wait_for_UID_SEND_OK:{
		if(UARTcheckEndOfTransfer()==1){
			break;	//if not completely sent
		}
		temp = UARTcheckForSendOK();
		if(temp==1){	//no SendOk, FAILED or ERROR was recieved
			//debug_printf("waiting!\n\r");
			break;
		}
		if(temp==2){
			debug_printf("ERROR/Failed recieved when trying to send UID!\n\r");
			actTCPstate = waiting_for_data;
			break;
		}
		//If SEND OK was revieved
		debug_printf("UID was sent!\n\r");
		actTCPstate = waiting_for_data;
		break;
	}
	case wait_for_Pos_startindicator:{
		if(UARTcheckEndOfTransfer()==1){
			break;	//if not completely sent
		}
		temp = UARTcheckForStartIndicator();
		if(temp==1){	//If no > and no ERROR was recieved return
			break;
		}
		if(temp==2){	//If ERROR was recieved
			debug_printf("ERROR recieved when trying to send Pos!\n\r");
			actTCPstate = waiting_for_data;
			break;
		}
		//if > recieved start sending UID
		debug_printf("Start sending Pos...\n\r");
		UARTStartTransfers(TCPbuffer);
		actTCPstate = wait_for_Pos_SEND_OK;
		break;
	}
	case wait_for_Pos_SEND_OK:{
		if(UARTcheckEndOfTransfer()==1){
			break;	//if not completely sent
		}
		temp = UARTcheckForSendOK();
		if(temp==1){	//no SendOk, FAILED or ERROR was recieved
			//debug_printf("waiting!\n\r");
			break;
		}
		if(temp==2){
			debug_printf("ERROR/Failed recieved when trying to send Pos!\n\r");
			actTCPstate = waiting_for_data;
			break;
		}
		//If SEND OK was revieved
		debug_printf("Pos was sent!\n\r");
		actTCPstate = waiting_for_data;
		break;
	}

	case wait_for_Dist_startindicator:{
		if(UARTcheckEndOfTransfer()==1){
			break;	//if not completely sent
		}
		temp = UARTcheckForStartIndicator();
		if(temp==1){	//If no > and no ERROR was recieved return
			break;
		}
		if(temp==2){	//If ERROR was recieved
			debug_printf("ERROR recieved when trying to send Distances!\n\r");
			actTCPstate = waiting_for_data;
			break;
		}
		//if > recieved start sending UID
		debug_printf("Start sending Distances...\n\r");
		debug_printf(TCPbuffer);
		UARTStartTransfers(TCPbuffer);
		actTCPstate = wait_for_Dist_SEND_OK;
		break;
	}
	case wait_for_Dist_SEND_OK:{
		if(UARTcheckEndOfTransfer()==1){
			break;	//if not completely sent
		}
		temp = UARTcheckForSendOK();
		if(temp==1){	//no SendOk, FAILED or ERROR was recieved
			//debug_printf("waiting!\n\r");
			break;
		}
		if(temp==2){
			debug_printf("ERROR/Failed recieved when trying to send Distances!\n\r");
			actTCPstate = waiting_for_data;
			break;
		}
		//If SEND OK was revieved
		debug_printf("Distances sent!\n\r");
		actTCPstate = waiting_for_data;
		break;
	}

	}
	return actTCPstate;
}

