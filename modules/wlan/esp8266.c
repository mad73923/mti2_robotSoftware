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

void ESP8266createAP(const char* SSID,const char* PW){

	///* Initiate DMA transfers */
	// //Modus = Soft AP
	// StartTransfers("AT+CWMODE_CUR=2\r\n");
	// WaitAndCheckEndOfTransfer();
	//
	// //LL_mDelay(100);
	//
	// WaitForUserButtonPress();
	//
	// StartTransfers("AT+CWSAP_CUR=\"Robot1\",\"1234567890\",5,3\r\n");
	//
	// /* Wait for the end of the transfer and check received data */
	// WaitAndCheckEndOfTransfer();


}

