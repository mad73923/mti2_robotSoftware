/*
 * motor.c
 *
 *  Created on: 17.05.2017
 *      Author: felix
 */
#include "motor.h"

//
void configure_gpio(void){
  /* Configure IO in output push-pull mode to drive external LED2 & Motor*/
  LL_GPIO_SetPinMode(MOTORLEFT_DIR_PORT, MOTORLEFT_DIR_PIN, LL_GPIO_MODE_OUTPUT);
  LL_GPIO_SetPinMode(MOTORRIGHT_DIR_PORT, MOTORRIGHT_DIR_PIN, LL_GPIO_MODE_OUTPUT);
  LL_GPIO_SetPinMode(PIEP_PORT, PIEP_PIN, LL_GPIO_MODE_OUTPUT);

  /* Predefine Motor-Directions */
  LL_GPIO_SetOutputPin(MOTORLEFT_DIR_PORT, MOTORLEFT_DIR_PIN);
  LL_GPIO_SetOutputPin(MOTORRIGHT_DIR_PORT, MOTORRIGHT_DIR_PIN);
}

int setSpeed(char cDir, int iSpeed){

}


int setDirection(char cMotor,char cDir){

}


int setSpeedDir(char cMotor, char cDir, int iSpeed){

}


int stop(char cMotor){

}
