/*
 * main.c
 *
 *  Created on: 10.05.2017
 *      Author: matthias
 */

#include "main.h"

int main(void){
	system_init();
	WLANinit();
	WLANcreateAP("Robot1","PWstinkt");
}
