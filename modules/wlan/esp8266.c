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
char Buffer2[50];
char Buffer3[20];

char SSID_loc[50];
char PW_loc[50];
char IP_loc[20];
char ServerIP_loc[20];
char Port_loc[5];
uint8_t mutex;

void(*esp8266readyCallback)(uint8_t);

void (*ESP8266_readyReceived)(char*,uint16_t);
void (*ESP8266_OK_Received)(char*, uint16_t);
void (*ESP8266_IPD_ReceivedCallback)(char*, uint16_t);
void (*ESP8266_StartIndicator_Received)(char*, uint16_t);
void (*ESP8266_SendOk_Received)(char*, uint16_t);

void ESP8266ExpectReadyCallback(char* buffer, uint16_t length);
void ESP8266ExpectOKCallback(char* buffer, uint16_t length);
void ESP8266ExpectIPDCallback(char* buffer, uint16_t length);
void ESP8266ExpectSendOkCallback(char* buffer, uint16_t length);
void ESP8266ExpectWIFI_CONNECTEDCallback(char* buffer, uint16_t length);

void ESP8266connectToApCallback1(char* Buffer,uint16_t Length);
void ESP8266connectToApCallback2(char* Buffer,uint16_t Length);
void ESP8266connectToApCallback3(char* Buffer,uint16_t Length);
void ESP8266connectToApCallback4(char* Buffer,uint16_t Length);
void ESP8266connectToApCallback5(char* Buffer,uint16_t Length);

void ESP8266connectToTCPserverCallback1(char* Buffer, uint16_t Length);
void ESP8266connectToTCPserverCallback2(char* RxBuffer,uint16_t Length);

void ESP8266_IPD_FinalCallback(char* RxBuffer, uint16_t Length);
void ESP8266_IPD_Callback1(char* RxBuffer,uint16_t Length);
void ESP8255_IPD_SendUIDCallback1(char* RxBuffer,uint16_t Length);
void ESP8255_IPD_SendUIDCallback2(char* RxBuffer,uint16_t Length);
void ESP8255_IPD_SendPosCallback1(char* RxBuffer,uint16_t Length);
void ESP8255_IPD_SendDistancesCallback1(char* RxBuffer,uint16_t Length);
void ESP8255_IPD_SetThrottleCallback1(char* RxBuffer,uint16_t Length);
void ESP8255_IPD_SetPositionCallback1(char* RxBuffer,uint16_t Length);
void ESP8255_IPD_SetPIDCallback1(char* RxBuffer,uint16_t Length);
void ESP8255_IPD_SetSpeedCallback1(char* RxBuffer,uint16_t Length);

void ESP8266_IPD_SetHornCallback1(char* RxBuffer, uint16_t Length);


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

void ESP8266connectToAp(const char* SSID,const char* PW,const char* IP, void(*ESP8266readyCallback)(uint8_t)){
	mutex_lock();
	esp8266readyCallback = ESP8266readyCallback;
	strcpy(SSID_loc,SSID);
	strcpy(PW_loc,PW);
	strcpy(IP_loc,IP);
	//debug_printf("Start to connect to AP\n\r");
	ESP8266_readyReceived = ESP8266connectToApCallback1;
	UARTclearBuffer();
	UARTStartTransfersCB("AT+RST\r\n",ESP8266ExpectReadyCallback);
}

void ESP8266reconnectToWLAN(void(*ESP8266readyCallback)(uint8_t)){
	mutex_lock();
	esp8266readyCallback = ESP8266readyCallback;
	UARTsetNewLineCallback(ESP8266ExpectWIFI_CONNECTEDCallback);
}

void ESP8266connectToTCPserver(const char* IP, const char* Port, void(*ESP8266readyCallback)(uint8_t)){
	mutex_lock();
	esp8266readyCallback = ESP8266readyCallback;	//do that when everything is done
	strcpy(ServerIP_loc,IP);
	strcpy(Port_loc,Port);
	ESP8266_OK_Received = ESP8266connectToTCPserverCallback1;
	UARTclearBuffer();
	UARTStartTransfersCB("AT+CIPMUX=0\r\n",ESP8266ExpectOKCallback);
}

void ESP8266startTCPlistener(void(*ESP8266readyCallback)(uint8_t)){
	mutex_lock();
	UARTclearBuffer();
	esp8266readyCallback = ESP8266readyCallback;	//do that when everything is done
	ESP8266_IPD_ReceivedCallback = ESP8266_IPD_Callback1;	//do after IPD was recieved
	UARTsetNewLineCallback(ESP8266ExpectIPDCallback);	//Do for every new-line
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

void ESP8266ExpectReadyCallback(char* buffer, uint16_t length){
	if(strstr(buffer,"ready")){
	//		debug_printf("ready recieved!\n\r");
			UARTclearBuffer();
			ESP8266_readyReceived(buffer, length);
		}
	else if(strstr(buffer,"ERROR")){
		mutex_unlock();
		void(*esp8266readyCallbacktemp)(uint8_t)=esp8266readyCallback;
		if(esp8266readyCallback!=0){
			esp8266readyCallback = 0;
			esp8266readyCallbacktemp(1);
		}
	}
	else{
		UARTsetNewLineCallback(ESP8266ExpectReadyCallback);
		UARTclearBuffer();
	}
}

void ESP8266ExpectWIFI_CONNECTEDCallback(char* buffer, uint16_t length){
	if(strstr(buffer,"WIFI CONNECTED")){
		mutex_unlock();
		void(*esp8266readyCallbacktemp)(uint8_t)=esp8266readyCallback;
		if(esp8266readyCallback!=0){
			esp8266readyCallback = 0;
			esp8266readyCallbacktemp(0);
		}
		}
	else{
		UARTsetNewLineCallback(ESP8266ExpectWIFI_CONNECTEDCallback);
		UARTclearBuffer();
	}
}

void ESP8266ExpectOKCallback(char* buffer, uint16_t length){
	if(strstr(buffer,"OK")){
	//		debug_printf("ready recieved!\n\r");
			UARTclearBuffer();
			ESP8266_OK_Received(buffer, length);
		}
	else if(strstr(buffer,"ERROR")){
		mutex_unlock();
		void(*esp8266readyCallbacktemp)(uint8_t)=esp8266readyCallback;
		if(esp8266readyCallback!=0){
			esp8266readyCallback = 0;
			esp8266readyCallbacktemp(1);
		}
	}
	else if(strstr(buffer,"CLOSED")){
		mutex_unlock();
		UARTclearBuffer();
		void(*esp8266readyCallbacktemp)(uint8_t)=esp8266readyCallback;
		if(esp8266readyCallback!=0){
			esp8266readyCallback = 0;
			esp8266readyCallbacktemp(2);
		}
	}
	else{
		UARTsetNewLineCallback(ESP8266ExpectOKCallback);
		UARTclearBuffer();
	}
}

void ESP8266ExpectSendOkCallback(char* buffer, uint16_t length){
	if(strstr(buffer,"SEND OK")){
		UARTclearBuffer();
		mutex_unlock();
		void(*esp8266readyCallbacktemp)(uint8_t)=esp8266readyCallback;
		if(esp8266readyCallback!=0){
			esp8266readyCallback = 0;
			esp8266readyCallbacktemp(0);
		}
	}
	else if(strstr(buffer,"FAILED")){
		mutex_unlock();
		void(*esp8266readyCallbacktemp)(uint8_t)=esp8266readyCallback;
		if(esp8266readyCallback!=0){
			esp8266readyCallback = 0;
			esp8266readyCallbacktemp(1);
		}
	}
	else if(strstr(buffer,"ERROR")){
		mutex_unlock();
		void(*esp8266readyCallbacktemp)(uint8_t)=esp8266readyCallback;
		if(esp8266readyCallback!=0){
			esp8266readyCallback = 0;
			esp8266readyCallbacktemp(1);
		}
	}
	else if(strstr(buffer,"CLOSED")){
		mutex_unlock();
		UARTclearBuffer();
		void(*esp8266readyCallbacktemp)(uint8_t)=esp8266readyCallback;
		if(esp8266readyCallback!=0){
			esp8266readyCallback = 0;
			esp8266readyCallbacktemp(2);
		}
	}
	else{
		UARTsetNewLineCallback(ESP8266ExpectSendOkCallback);
		UARTclearBuffer();
	}
}

void ESP8266ExpectIPDCallback(char* buffer, uint16_t length){
	if(strncmp(buffer,"+IPD,",5)==0){
		ESP8266_IPD_ReceivedCallback=0;
		if(strncmp(buffer,"+IPD,8:GetUID?",14)==0){
			ESP8266_IPD_ReceivedCallback = ESP8255_IPD_SendUIDCallback1;
			UARTclearBuffer();
		}
		else if(strncmp(buffer,"+IPD,8:GetPos?",14)==0){
			ESP8266_IPD_ReceivedCallback = ESP8255_IPD_SendPosCallback1;
			UARTclearBuffer();
		}
		else if(strncmp(buffer,"+IPD,14:GetDistances?",21)==0){
			ESP8266_IPD_ReceivedCallback = ESP8255_IPD_SendDistancesCallback1;
			UARTclearBuffer();
		}
		else if(strstr(buffer,":SetThrottle![")){
			ESP8266_IPD_ReceivedCallback = ESP8255_IPD_SetThrottleCallback1;
		}
		else if(strstr(buffer, ":SetHorn![")){
			ESP8266_IPD_ReceivedCallback = ESP8266_IPD_SetHornCallback1;
		}
		else if(strstr(buffer, ":SetPosition![")){
					ESP8266_IPD_ReceivedCallback = ESP8255_IPD_SetPositionCallback1;
		}
		else if(strstr(buffer, ":SetSpeed![")){
							ESP8266_IPD_ReceivedCallback = ESP8255_IPD_SetSpeedCallback1;
		}
		else if(strstr(buffer, ":SetPID![")){
							ESP8266_IPD_ReceivedCallback = ESP8255_IPD_SetPIDCallback1;
		}

		if(ESP8266_IPD_ReceivedCallback!=0){
			ESP8266_IPD_ReceivedCallback(buffer, length);
			ESP8266_IPD_ReceivedCallback=0;
		}
		else{
			debug_printf("Undefined TCP-Data");
		}
		}
	else if(strstr(buffer,"ERROR")){
		mutex_unlock();
		UARTclearBuffer();
		void(*esp8266readyCallbacktemp)(uint8_t)=esp8266readyCallback;
		if(esp8266readyCallback!=0){
			esp8266readyCallback = 0;
			esp8266readyCallbacktemp(1);
		}
	}
	else if(strstr(buffer,"CLOSED")){
		mutex_unlock();
		UARTclearBuffer();
		void(*esp8266readyCallbacktemp)(uint8_t)=esp8266readyCallback;
		if(esp8266readyCallback!=0){
			esp8266readyCallback = 0;
			esp8266readyCallbacktemp(2);
		}
	}
	else if(strstr(buffer,"WIFI DISCONNECT")){
		mutex_unlock();
		UARTclearBuffer();
		void(*esp8266readyCallbacktemp)(uint8_t)=esp8266readyCallback;
		if(esp8266readyCallback!=0){
			esp8266readyCallback = 0;
			esp8266readyCallbacktemp(3);
		}
	}
	else{
		UARTsetNewLineCallback(ESP8266ExpectIPDCallback); //Wait again
		UARTclearBuffer();
	}
}

void ESP8266connectToApCallback1(char* RxBuffer,uint16_t Length){
	ESP8266_OK_Received = ESP8266connectToApCallback2;
	UARTclearBuffer();
	UARTStartTransfersCB("ATE1\r\n",ESP8266ExpectOKCallback);
}

void ESP8266connectToApCallback2(char* RxBuffer,uint16_t Length){
	ESP8266_OK_Received = ESP8266connectToApCallback3;
	UARTclearBuffer();
	UARTStartTransfersCB("AT+CWMODE_CUR=1\r\n",ESP8266ExpectOKCallback);
}

void ESP8266connectToApCallback3(char* RxBuffer,uint16_t Length){
	// skipped callback 4 for auto IP
	//ESP8266_OK_Received = ESP8266connectToApCallback4;
	ESP8266_OK_Received = ESP8266connectToApCallback5;
	strcpy(Buffer,"AT+CWJAP_CUR=\"");
	strcat(Buffer,SSID_loc);
	strcat(Buffer,"\",\"");
	strcat(Buffer,PW_loc);
	strcat(Buffer,"\"\r\n");
	UARTclearBuffer();
	UARTStartTransfersCB(Buffer,ESP8266ExpectOKCallback);
}

void ESP8266connectToApCallback4(char* RxBuffer,uint16_t Length){
	ESP8266_OK_Received = ESP8266connectToApCallback5;
	strcpy(Buffer,"AT+CIPSTA_CUR=\"");
	strcat(Buffer,IP_loc);
	strcat(Buffer,"\"\r\n");
	UARTclearBuffer();
	UARTStartTransfersCB(Buffer,ESP8266ExpectOKCallback);
}

void ESP8266connectToApCallback5(char* RxBuffer,uint16_t Length){
	mutex_unlock();
	UARTclearBuffer();
	void(*esp8266readyCallbacktemp)(uint8_t)=esp8266readyCallback;
	if(esp8266readyCallback!=0){
		esp8266readyCallback = 0;
		esp8266readyCallbacktemp(0);
	}
}

void ESP8266connectToTCPserverCallback1(char* RxBuffer,uint16_t Length){
	ESP8266_OK_Received = ESP8266connectToTCPserverCallback2;
	strcpy(Buffer,"AT+CIPSTART=\"TCP\",\""); //192.168.101.110", 1000");
	strcat(Buffer,ServerIP_loc);
	strcat(Buffer,"\",");
	strcat(Buffer,Port_loc);
	strcat(Buffer,"\r\n");
	UARTclearBuffer();
	UARTStartTransfersCB(Buffer,ESP8266ExpectOKCallback);
}

void ESP8266connectToTCPserverCallback2(char* RxBuffer,uint16_t Length){
	mutex_unlock();
	UARTclearBuffer();
	void(*esp8266readyCallbacktemp)(uint8_t)=esp8266readyCallback;
	if(esp8266readyCallback!=0){
		esp8266readyCallback = 0;
		esp8266readyCallbacktemp(0);

	}
}

void ESP8266_IPD_Callback1(char* RxBuffer,uint16_t Length){

}

void ESP8255_IPD_SendUIDCallback1(char* RxBuffer,uint16_t Length){
	uint32_t index = 0;
	index += sprintf(&Buffer[index], "%s", "AT+CIPSEND=");
	index += sprintf(&Buffer[index], "%d\r\n", strlen(uid_getUIDString())+4);
	UARTclearBuffer();
	UARTsetStartIndicatorCallback(ESP8255_IPD_SendUIDCallback2);
	UARTStartTransfersCB(Buffer,0);	//because of listen to >
}

void ESP8255_IPD_SendUIDCallback2(char* RxBuffer,uint16_t Length){
	uint32_t index = 0;
	index += sprintf(&Buffer[index], "%s", "UID=");
	index += sprintf(&Buffer[index], "%s", uid_getUIDString());
	UARTclearBuffer();
	UARTStartTransfersCB(Buffer,ESP8266ExpectSendOkCallback);
}

void ESP8255_IPD_SendPosCallback1(char* RxBuffer,uint16_t Length){
	uint32_t index = 0;
	odo_status stat = odometry_getStatus();
	index += sprintf(&Buffer[index], "%s", "ActPos=");
	index += sprintf(&Buffer[index], "[%.1f,",stat.position.posX);
	index += sprintf(&Buffer[index], "%.1f,",stat.position.posY);
	index += sprintf(&Buffer[index], "%.10f]",stat.theta);
	index = 0;
	index += sprintf(&Buffer2[index], "%s", "AT+CIPSEND=");
	index += sprintf(&Buffer2[index], "%d\r\n", strlen(Buffer));
	UARTclearBuffer();
	UARTsetStartIndicatorCallback(ESP8266_IPD_FinalCallback);
	UARTStartTransfersCB(Buffer2,0);	//because of listen to >
}

void ESP8266_IPD_FinalCallback(char* RxBuffer, uint16_t Length){
	UARTclearBuffer();
	UARTStartTransfersCB(Buffer,ESP8266ExpectSendOkCallback);
}

//Beispieldaten
uint16_t distances[36] = {1,2,3,4,5,4,3,2,1,0,10,20,30,40,50,40,30,20,10,0,100,200,300,400,500,400,300,200,100,0,20,40,60,80,90,100};
uint16_t numdistances = 36;

void ESP8255_IPD_SendDistancesCallback1(char* RxBuffer,uint16_t Length){
	uint32_t index = 0;
	index += sprintf(&Buffer[index], "%s", "ActDistances=[");
	for(uint16_t i = 0; i<numdistances; i++){
		index += sprintf(&Buffer[index], "%d,",distances[i]);
	}
	index += sprintf(&Buffer[index-1], "]");

	index = 0;
	index += sprintf(&Buffer2[index], "%s", "AT+CIPSEND=");
	index += sprintf(&Buffer2[index], "%d\r\n", strlen(Buffer));
	UARTclearBuffer();
	UARTsetStartIndicatorCallback(ESP8266_IPD_FinalCallback);
	UARTStartTransfersCB(Buffer2,0);	//because of listen to >
}

void ESP8255_IPD_SetThrottleCallback1(char* RxBuffer,uint16_t Length){
	int32_t throttle_l;
	int32_t throttle_r;
	sscanf(RxBuffer,"+IPD,%*d:SetThrottle![%d,%d]",&throttle_l,&throttle_r);
	//debug_printf("%d %d\n",throttle_l, throttle_r);

	PID_disable();
	throttle_l = motor_setSpeed(MOTORLEFT, throttle_l);
	throttle_r = motor_setSpeed(MOTORRIGHT, throttle_r);

	mutex_unlock();
	UARTclearBuffer();
	void(*esp8266readyCallbacktemp)(uint8_t)=esp8266readyCallback;
	if(esp8266readyCallback!=0){
		esp8266readyCallback = 0;
		esp8266readyCallbacktemp(0);

	}
	uint32_t index = 0;
	index += sprintf(&Buffer[index], "SetThrottle=[%d,%d]", throttle_l, throttle_r);
	index = 0;
	index += sprintf(&Buffer2[index], "%s", "AT+CIPSEND=");
	index += sprintf(&Buffer2[index], "%d\r\n", strlen(Buffer));
	UARTclearBuffer();
	UARTsetStartIndicatorCallback(ESP8266_IPD_FinalCallback);
	UARTStartTransfersCB(Buffer2,0);	//because of listen to >
}

void ESP8266_IPD_SetHornCallback1(char* RxBuffer,uint16_t Length){
	uint8_t horn;
	sscanf(RxBuffer,"+IPD,%*d:SetHorn![%d]",&horn);

	horn_enable(horn);

	mutex_unlock();
	UARTclearBuffer();
	void(*esp8266readyCallbacktemp)(uint8_t)=esp8266readyCallback;
	if(esp8266readyCallback!=0){
		esp8266readyCallback = 0;
		esp8266readyCallbacktemp(0);

	}
	uint32_t index = 0;
	index += sprintf(&Buffer[index], "%s", "SetHorn=OK");
	index = 0;
	index += sprintf(&Buffer2[index], "%s", "AT+CIPSEND=");
	index += sprintf(&Buffer2[index], "%d\r\n", strlen(Buffer));
	UARTclearBuffer();
	UARTsetStartIndicatorCallback(ESP8266_IPD_FinalCallback);
	UARTStartTransfersCB(Buffer2,0);	//because of listen to >
}

void ESP8255_IPD_SetPositionCallback1(char* RxBuffer,uint16_t Length){
	float posX;
	float posY;
	float theta;

	sscanf(RxBuffer,"+IPD,%*d:SetPosition![%f,%f,%f]",&posX,&posY,&theta);

	odometry_setStatus(posX, posY, theta);

	mutex_unlock();
	UARTclearBuffer();
	void(*esp8266readyCallbacktemp)(uint8_t)=esp8266readyCallback;
	if(esp8266readyCallback!=0){
		esp8266readyCallback = 0;
		esp8266readyCallbacktemp(0);

	}
	uint32_t index = 0;
	index += sprintf(&Buffer[index], "%s", "SetPosition=OK");
	index = 0;
	index += sprintf(&Buffer2[index], "%s", "AT+CIPSEND=");
	index += sprintf(&Buffer2[index], "%d\r\n", strlen(Buffer));
	UARTclearBuffer();
	UARTsetStartIndicatorCallback(ESP8266_IPD_FinalCallback);
	UARTStartTransfersCB(Buffer2,0);	//because of listen to >
}

void ESP8255_IPD_SetPIDCallback1(char* RxBuffer,uint16_t Length){
	int32_t kP;
	int32_t kI;
	int32_t kD;

	sscanf(RxBuffer,"+IPD,%*d:SetPID![%d,%d,%d]",&kP,&kI,&kD);

	PID_setParameter(kP, kI, kD);

	mutex_unlock();
	UARTclearBuffer();
	void(*esp8266readyCallbacktemp)(uint8_t)=esp8266readyCallback;
	if(esp8266readyCallback!=0){
		esp8266readyCallback = 0;
		esp8266readyCallbacktemp(0);

	}
	uint32_t index = 0;
	index += sprintf(&Buffer[index], "%s", "SetPID=OK");
	index = 0;
	index += sprintf(&Buffer2[index], "%s", "AT+CIPSEND=");
	index += sprintf(&Buffer2[index], "%d\r\n", strlen(Buffer));
	UARTclearBuffer();
	UARTsetStartIndicatorCallback(ESP8266_IPD_FinalCallback);
	UARTStartTransfersCB(Buffer2,0);	//because of listen to >
}

void ESP8255_IPD_SetSpeedCallback1(char* RxBuffer,uint16_t Length){
	int32_t speedLeft, speedRight;

	sscanf(RxBuffer,"+IPD,%*d:SetSpeed![%d,%d]",&speedLeft,&speedRight);

	PID_setSpeed(speedLeft, speedRight);
	PID_enable();

	mutex_unlock();
	UARTclearBuffer();
	void(*esp8266readyCallbacktemp)(uint8_t)=esp8266readyCallback;
	if(esp8266readyCallback!=0){
		esp8266readyCallback = 0;
		esp8266readyCallbacktemp(0);

	}
	uint32_t index = 0;
	index += sprintf(&Buffer[index], "%s", "SetSpeed=OK");
	index = 0;
	index += sprintf(&Buffer2[index], "%s", "AT+CIPSEND=");
	index += sprintf(&Buffer2[index], "%d\r\n", strlen(Buffer));
	UARTclearBuffer();
	UARTsetStartIndicatorCallback(ESP8266_IPD_FinalCallback);
	UARTStartTransfersCB(Buffer2,0);	//because of listen to >
}
