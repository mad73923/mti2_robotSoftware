/*
 * esp8266.c
 *
 *  Created on: 17.05.2017
 *      Author: thomas
 */

#include "esp8266.h"
#include "html.h"

enum TCP_state {waiting_for_data,
	wait_for_UID_startindicator,
	wait_for_UID_SEND_OK,
	wait_for_Pos_startindicator,
	wait_for_Pos_SEND_OK,
	wait_for_Dist_startindicator,
	wait_for_Dist_SEND_OK,};
enum TCP_state actTCPstate = waiting_for_data;

char Buffer[1450];
char Buffer2[5];
char Buffer3[20];

char SSID_loc[50];
char PW_loc[50];
char IP_loc[20];
uint8_t mutex;

void(*esp8266readyCallback)(uint8_t);

void ESP8266connectToApCallback1(char* Buffer,uint16_t Length);
void ESP8266connectToApCallback2(char* Buffer,uint16_t Length);
void ESP8266connectToApCallback3(char* Buffer,uint16_t Length);
void ESP8266connectToApCallback4(char* Buffer,uint16_t Length);
void ESP8266connectToApCallback5(char* Buffer,uint16_t Length);

void mutex_lock(void);
void mutex_unlock(void);

void mutex_lock(void){
	while(mutex);
	mutex = 1;
}
void mutex_unlock(void){
	mutex = 0;
}

void ESP8266init(){
	mutex_unlock();
	UARTinit();
}

uint8_t ESP8266connectToAp(const char* SSID,const char* PW,const char* IP, void(*ESP8266readyCallback)(uint8_t)){
	mutex_lock();
	esp8266readyCallback = ESP8266readyCallback;
	strcpy(SSID_loc,SSID);
	strcpy(PW_loc,PW);
	strcpy(IP_loc,IP);
	uint8_t returnval = 0;
	//debug_printf("Start to connect to AP\n\r");
	UARTclearBuffer();
	UARTStartTransfersCB("AT+RST\r\n",ESP8266connectToApCallback1);
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


uint8_t ESP8266handleTCP(void){

	uint8_t temp = 0;
	switch(actTCPstate){
	case waiting_for_data:{
		const char* ptrMessage = ESP8266getIncomingTCPdata();
		if(ptrMessage==NULL){break;}				//if no new data recieved return
		debug_printf("Data revieved: %s\n\r", ptrMessage );		//else send recieved message over debug-uart

		if(strcmp(ptrMessage,"GetUID?")==0){
			debug_printf("Robot was asked for UID\n\r");
			strcpy(Buffer,"AT+CIPSEND=");
			sprintf(Buffer2,"%d",strlen(uid_getUIDString())+4);
			strcat(Buffer,Buffer2);
			strcat(Buffer,"\r\n");
			debug_printf(Buffer);
			UARTStartTransfers(Buffer);
			actTCPstate = wait_for_UID_startindicator;
			break;
		}

		if(strcmp(ptrMessage,"GetPos?")==0){
			debug_printf("Robot was asked for Pos\n\r");
			strcpy(Buffer,"ActPos=");
			sprintf(Buffer2,"[%ld,",123);
			strcat(Buffer,Buffer2);
			sprintf(Buffer2,"%ld,",345);
			strcat(Buffer,Buffer2);
			sprintf(Buffer2,"%.1f",67.8);
			strcat(Buffer,Buffer2);
			strcpy(Buffer3,"AT+CIPSEND=");
			sprintf(Buffer2,"%d",strlen(Buffer));
			strcat(Buffer3,Buffer2);
			strcat(Buffer3,"\r\n");
			UARTStartTransfers(Buffer3);
			actTCPstate = wait_for_Pos_startindicator;
			break;
		}

		if(strcmp(ptrMessage,"GetDistances?")==0){
			debug_printf("Robot was asked for Distances\n\r");
			strcpy(Buffer,"ActDistances=[");
			uint16_t Dist[36];
			for(int i=0;i<36;i++){
					Dist[i] = i*20;
			}
			for(int i =0;i<36;i++){
				if(i<(36-1)){
					sprintf(Buffer2,"%d,",Dist[i]);
				}
				else{
					sprintf(Buffer2,"%d]",Dist[i]);
				}
				strcat(Buffer,Buffer2);
			}
			strcpy(Buffer3,"AT+CIPSEND=");
			sprintf(Buffer2,"%d",strlen(Buffer));
			strcat(Buffer3,Buffer2);
			strcat(Buffer3,"\r\n");
			UARTStartTransfers(Buffer3);
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
		strcpy(Buffer,"UID=");
		strcat(Buffer,uid_getUIDString());
		//debug_printf(buffer);
		UARTStartTransfers(Buffer);
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
		UARTStartTransfers(Buffer);
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
		debug_printf(Buffer);
		UARTStartTransfers(Buffer);
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

void ESP8266connectToApCallback1(char* RxBuffer,uint16_t Length){
	//debug_printf("Callback\n\r");
	if(strstr(RxBuffer,"ready")){
//		debug_printf("ready recieved!\n\r");
		UARTclearBuffer();
		UARTStartTransfersCB("ATE1\r\n",ESP8266connectToApCallback2);
	}
	else{
		UARTsetNewLineCallback(ESP8266connectToApCallback1);
		UARTclearBuffer();
	}
}

void ESP8266connectToApCallback2(char* RxBuffer,uint16_t Length){
	if(strstr(RxBuffer,"OK")){
		UARTclearBuffer();
		UARTStartTransfersCB("AT+CWMODE_CUR=1\r\n",ESP8266connectToApCallback3);
	}
	else if(strstr(RxBuffer,"ERROR")){
		debug_printf("ERROR recieved\n\r");
		UARTclearBuffer();
		mutex_unlock();
		if(esp8266readyCallback!=0){
			esp8266readyCallback(1);
			esp8266readyCallback = 0;
		}
	}
	else{
		UARTsetNewLineCallback(ESP8266connectToApCallback2);
	}
}

void ESP8266connectToApCallback3(char* RxBuffer,uint16_t Length){
	if(strstr(RxBuffer,"OK")){
		UARTclearBuffer();
		//debug_printf("Setting STA-Mode successful!\n\r");
		strcpy(Buffer,"AT+CWJAP_CUR=\"");
		strcat(Buffer,SSID_loc);
		strcat(Buffer,"\",\"");
		strcat(Buffer,PW_loc);
		strcat(Buffer,"\"\r\n");
		UARTStartTransfersCB(Buffer,ESP8266connectToApCallback4);
		UARTcheckEndOfTransfer();
	}
	else if(strstr(RxBuffer,"ERROR")){
		//debug_printf("ERROR recieved\n\r");
		UARTclearBuffer();
		mutex_unlock();
		if(esp8266readyCallback!=0){
			esp8266readyCallback(1);
			esp8266readyCallback = 0;
		}
	}
	else{
			UARTsetNewLineCallback(ESP8266connectToApCallback3);
		}
}

void ESP8266connectToApCallback4(char* RxBuffer,uint16_t Length){
	if(strstr(RxBuffer,"OK")){
		UARTclearBuffer();
		//debug_printf("Connecting to AP successful!\n\r");
		strcpy(Buffer,"AT+CIPSTA_CUR=\"");
		strcat(Buffer,IP_loc);
		strcat(Buffer,"\"\r\n");
		UARTStartTransfersCB(Buffer,ESP8266connectToApCallback5);
		UARTcheckEndOfTransfer();
	}
	else if(strstr(RxBuffer,"ERROR")){
		//debug_printf("ERROR recieved\n\r");
		UARTclearBuffer();
		mutex_unlock();
		if(esp8266readyCallback!=0){
			esp8266readyCallback(1);
			esp8266readyCallback = 0;
		}
	}
	else{
				UARTsetNewLineCallback(ESP8266connectToApCallback4);
			}
}

void ESP8266connectToApCallback5(char* RxBuffer,uint16_t Length){
	if(strstr(RxBuffer,"OK")){
		//debug_printf("Setting IP-Address successful!\n\r");
		UARTclearBuffer();
		mutex_unlock();
		if(esp8266readyCallback!=0){
			esp8266readyCallback(0);
			esp8266readyCallback = 0;
		}
	}
	else if(strstr(RxBuffer,"ERROR")){
		//debug_printf("ERROR recieved\n\r");
		UARTclearBuffer();
		mutex_unlock();
		if(esp8266readyCallback!=0){
			esp8266readyCallback(1);
			esp8266readyCallback = 0;
		}
	}
	else{
					UARTsetNewLineCallback(ESP8266connectToApCallback4);
				}

}
