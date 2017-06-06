/*
 * uid.c
 *
 *  Created on: 05.06.2017
 *      Author: matthias
 */

#include "uid.h"

uint8_t uidBuffer[36];

uint8_t* uid_getUIDString(void){
	uint32_t UID[3];
	UID[0] = LL_GetUID_Word0();
	UID[1] = LL_GetUID_Word1();
	UID[2] = LL_GetUID_Word2();

	for(uint8_t i = 0; i<3; i++){
		for(uint8_t j = 0; j<4; j++){
			snprintf(&uidBuffer[i*12+j*3], 4, "%02X:", (UID[2-i]&0xFF000000)>>24);
			UID[2-i] = UID[2-i]<<8;
		}
	}
	// delete the last :
	uidBuffer[35] = 0;
	return uidBuffer;
}
