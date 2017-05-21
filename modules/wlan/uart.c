/*
 * uart.c
 *
 *  Created on: 17.05.2017
 *      Author: thomas
 */

#include "uart.h"

void DMA1_TransmitComplete_Callback(void);
void USART_TransferError_Callback(void);
void USART3_RecieveCallback(void);
void UARTStartTransfers(const char* Command);
void UARTwaitEndOfTransfer(void);
uint8_t UARTwaitForOkOrError(uint32_t cyclesTimeout);


uint8_t TxLength;
char TxBuffer[500];
volatile uint8_t TransmissionComplete;
volatile uint8_t TransmissionError;

char RxBuffer[1000] = "";
volatile uint16_t Characters = 0;
volatile uint8_t Lines = 0; //wir ind ISR um 1 erhöht bei jedem \n = 0A


void UARTinit(){
	/*
	 * PIN CONFIG
	 */

	//Enable clock for UART-Pins
	LL_AHB2_GRP1_EnableClock(LL_AHB2_GRP1_PERIPH_GPIOC);

	//Configure Tx Pin as : Alternate function, High Speed, Push pull, Pull up
	LL_GPIO_SetPinMode(GPIOC, LL_GPIO_PIN_10, LL_GPIO_MODE_ALTERNATE);
	LL_GPIO_SetAFPin_8_15(GPIOC, LL_GPIO_PIN_10, LL_GPIO_AF_7);
	LL_GPIO_SetPinSpeed(GPIOC, LL_GPIO_PIN_10, LL_GPIO_SPEED_FREQ_HIGH);
	LL_GPIO_SetPinOutputType(GPIOC, LL_GPIO_PIN_10, LL_GPIO_OUTPUT_PUSHPULL);
	LL_GPIO_SetPinPull(GPIOC, LL_GPIO_PIN_10, LL_GPIO_PULL_UP);

	//Configure Rx Pin as : Alternate function, High Speed, Push pull, Pull up
	LL_GPIO_SetPinMode(GPIOC, LL_GPIO_PIN_11, LL_GPIO_MODE_ALTERNATE);
	LL_GPIO_SetAFPin_8_15(GPIOC, LL_GPIO_PIN_11, LL_GPIO_AF_7);
	LL_GPIO_SetPinSpeed(GPIOC, LL_GPIO_PIN_11, LL_GPIO_SPEED_FREQ_HIGH);
	LL_GPIO_SetPinOutputType(GPIOC, LL_GPIO_PIN_11, LL_GPIO_OUTPUT_PUSHPULL);
	LL_GPIO_SetPinPull(GPIOC, LL_GPIO_PIN_11, LL_GPIO_PULL_UP);

	/*
	 * USART CONFIG
	 */
	//Enable clock for USART3 and select Clock source
	LL_APB1_GRP1_EnableClock(LL_APB1_GRP1_PERIPH_USART3);
	LL_RCC_SetUSARTClockSource(LL_RCC_USART3_CLKSOURCE_PCLK1);

	//Set transferDirection to TX and RX
	LL_USART_SetTransferDirection(USART3, LL_USART_DIRECTION_TX_RX);

	//Set Data Structure to 8N1
	LL_USART_ConfigCharacter(USART3, LL_USART_DATAWIDTH_8B, LL_USART_PARITY_NONE, LL_USART_STOPBITS_1);

	//Set Baud Rate
	LL_USART_SetBaudRate(USART3, SystemCoreClock, LL_USART_OVERSAMPLING_16, 115200);

	//Enable USART3
	LL_USART_Enable(USART3);

	//Wait for USART being enabled
	while((!(LL_USART_IsActiveFlag_TEACK(USART3))) || (!(LL_USART_IsActiveFlag_REACK(USART3))))
	{

	}

	/*
	 * DMA CONFIG
	 */
	//Enable DMA-Clock
	LL_AHB1_GRP1_EnableClock(LL_AHB1_GRP1_PERIPH_DMA1);

	//Configure NVIC for DMA transfer complete/error interrupts
	NVIC_SetPriority(DMA1_Channel2_IRQn, 0);
	NVIC_EnableIRQ(DMA1_Channel2_IRQn);

	// Configure the DMA functional parameters for transmission */
	LL_DMA_ConfigTransfer(DMA1, LL_DMA_CHANNEL_2,
						LL_DMA_DIRECTION_MEMORY_TO_PERIPH |
						LL_DMA_PRIORITY_HIGH              |
						LL_DMA_MODE_NORMAL                |
						LL_DMA_PERIPH_NOINCREMENT         |
						LL_DMA_MEMORY_INCREMENT           |
						LL_DMA_PDATAALIGN_BYTE            |
						LL_DMA_MDATAALIGN_BYTE);
	LL_DMA_ConfigAddresses(DMA1, LL_DMA_CHANNEL_2,
						 (uint32_t)TxBuffer,
						 LL_USART_DMA_GetRegAddr(USART3, LL_USART_DMA_REG_DATA_TRANSMIT),
						 LL_DMA_GetDataTransferDirection(DMA1, LL_DMA_CHANNEL_2));
	LL_DMA_SetDataLength(DMA1, LL_DMA_CHANNEL_2, TxLength);
	LL_DMA_SetPeriphRequest(DMA1, LL_DMA_CHANNEL_2, LL_DMA_REQUEST_2);


	//Enable DMA transfer complete/error interrupts
	LL_DMA_EnableIT_TC(DMA1, LL_DMA_CHANNEL_2);
	LL_DMA_EnableIT_TE(DMA1, LL_DMA_CHANNEL_2);

	TransmissionComplete=1;
	TransmissionError = 0;

	/*
	 * Rx-Interrupt Config
	 */
	NVIC_SetPriority(USART3_IRQn, 2);
	LL_USART_EnableIT_RXNE(USART3);
	NVIC_EnableIRQ(USART3_IRQn);
}

void UARTStartTransfers(const char* Command)
{
	TransmissionComplete = 0;
	strcpy(TxBuffer, Command);
	TxLength = strlen(TxBuffer);
	LL_DMA_SetDataLength(DMA1, LL_DMA_CHANNEL_2, TxLength);

	/* Enable DMA TX Interrupt */
	LL_USART_EnableDMAReq_TX(USART3);

	/* Enable DMA Channel Tx */
	LL_DMA_EnableChannel(DMA1, LL_DMA_CHANNEL_2);
}


void UARTwaitEndOfTransfer(void)
{
  /* 1 - Wait end of transmission */
  while (TransmissionComplete != 1)
  {
  }
  /* Disable DMA1 Tx Channel */
  LL_DMA_DisableChannel(DMA1, LL_DMA_CHANNEL_2);
}


/* wartet auf ein "OK" vom ESP8266 oder ein "ERROR" und liefert einen Zahlenwert zurück:
 * return 0: "OK" wurde empfangen
 * return 1: "Error" wurde empfangen
 * return 2: "Timeout wurde erreicht*/
uint8_t UARTwaitForOkOrError(uint32_t cyclesTimeout){
	for(int i=0; i<cyclesTimeout; i++){
		if(strstr(RxBuffer,"OK")){
			//debug_printf("OK recieved\n\r");
			memset(RxBuffer,0,sizeof(RxBuffer));
			Lines = 0;
			Characters = 0;
			return 0;
		}
		if(strstr(RxBuffer,"ERROR")){
			debug_printf("ERROR recieved\n\r");
			memset(RxBuffer,0,sizeof(RxBuffer));
			Lines = 0;
			Characters = 0;
			return 1;
		}
	}
	debug_printf("timeout\n\r");
	memset(RxBuffer,0,sizeof(RxBuffer));
	Lines = 0;
	Characters = 0;
	return 2;
}

uint8_t UARTcheckForNewConnection(void){
	uint8_t clientnr = 10;
	char *ptr = strstr(RxBuffer,"CONNECT");
	if(ptr){
		ptr = ptr-2;
		clientnr = *ptr - '0';
		memset(RxBuffer,0,sizeof(RxBuffer));
		Characters = 0;
		Lines = 0;
		}
	return clientnr;
}

uint8_t UARTwaitForStartIndicator(uint32_t cyclesTimeout){
for(int i=0; i<cyclesTimeout; i++){
		if(strstr(RxBuffer,">")){
			//debug_printf("OK recieved\n\r");
			memset(RxBuffer,0,sizeof(RxBuffer));
			Lines = 0;
			Characters = 0;
			return 0;
		}
	}
	debug_printf("timeout\n\r");
	memset(RxBuffer,0,sizeof(RxBuffer));
	Lines = 0;
	Characters = 0;
	return 2;
}

uint8_t UARTwaitForReady(uint32_t cyclesTimeout){
for(int i=0; i<cyclesTimeout; i++){
		if(strstr(RxBuffer,"ready")){
			//debug_printf("OK recieved\n\r");
			memset(RxBuffer,0,sizeof(RxBuffer));
			Lines = 0;
			Characters = 0;
			return 0;
		}
	}
	debug_printf("timeout\n\r");
	memset(RxBuffer,0,sizeof(RxBuffer));
	Lines = 0;
	Characters = 0;
	return 2;
}

uint8_t UARTwaitForSendOK(uint32_t cyclesTimeout){
for(int i=0; i<cyclesTimeout; i++){
		if(strstr(RxBuffer,"SEND OK")){
			//debug_printf("OK recieved\n\r");
			memset(RxBuffer,0,sizeof(RxBuffer));
			Lines = 0;
			Characters = 0;
			return 0;
		}
		if(strstr(RxBuffer,"FAILED")){
			debug_printf("Failed recieved\n\r");
			memset(RxBuffer,0,sizeof(RxBuffer));
			Lines = 0;
			Characters = 0;
			return 1;
		}
	}
	debug_printf("timeout\n\r");
	memset(RxBuffer,0,sizeof(RxBuffer));
	Lines = 0;
	Characters = 0;
	return 2;
}

void DMA1_Channel2_IRQHandler(void)
{

  if(LL_DMA_IsActiveFlag_TC2(DMA1))
  {
    LL_DMA_ClearFlag_GI2(DMA1);
    /* Call function Transmission complete Callback */
    DMA1_TransmitComplete_Callback();
  }
  else if(LL_DMA_IsActiveFlag_TE2(DMA1))
  {
    /* Call Error function */
    USART_TransferError_Callback();
  }
}


void DMA1_TransmitComplete_Callback(void)
{
  //DMA Tx transfer completed
  TransmissionComplete = 1;
}

void USART_TransferError_Callback(void)
{
  /* Disable DMA1 Tx Channel */
  LL_DMA_DisableChannel(DMA1, LL_DMA_CHANNEL_2);
}

void USART3_IRQHandler(void){

	if(LL_USART_IsActiveFlag_RXNE(USART3)){
		USART3_RecieveCallback();
	}
}

void USART3_RecieveCallback(void){
	if(Characters<=999){
		char recieved = LL_USART_ReceiveData8(USART3);
		Characters++;
		strncat(RxBuffer,&recieved,1);
		if(recieved=='\n'){
			Lines++;
			//debug_printf("%s",RxBuffer);
		}
	}

}
