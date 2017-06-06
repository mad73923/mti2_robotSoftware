/*
 * uart.c
 *
 *  Created on: 17.05.2017
 *      Author: thomas
 */

#include "uart.h"

void DMA_TransmitComplete_Callback(void);
void USART_TransferError_Callback(void);
void USART_RecieveCallback(void);
void UARTStartTransfers(const char* Command);
void UARTwaitEndOfTransfer(void);
uint8_t UARTwaitForOkOrError(uint32_t cyclesTimeout);
const char* UARTCheckForIPD(void);


uint8_t TxLength;
char TxBuffer[500];
volatile uint8_t TransmissionComplete;
volatile uint8_t TransmissionError;

char RxBuffer[1000] = "";
volatile uint16_t Characters = 0;
volatile uint8_t Lines = 0; //wir ind ISR um 1 erhöht bei jedem \n = 0A

char IPDBuffer[100] = "";


void UARTinit(){
	/*
	 * PIN CONFIG
	 */

	//Enable clock for UART-Pins
	WLAN_UART_GPIO_CLK_ENABLE();

	//Configure Tx Pin as : Alternate function, High Speed, Push pull, Pull up
	LL_GPIO_SetPinMode(WLAN_UART_GPIO_TX_PORT, WLAN_UART_GPIO_TX_PIN, LL_GPIO_MODE_ALTERNATE);
	WLAN_UART_SET_TX_AF(WLAN_UART_GPIO_TX_PORT, WLAN_UART_GPIO_TX_PIN, WLAN_UART_GPIO_TX_AF);
	LL_GPIO_SetPinSpeed(WLAN_UART_GPIO_TX_PORT, WLAN_UART_GPIO_TX_PIN, LL_GPIO_SPEED_FREQ_HIGH);
	LL_GPIO_SetPinOutputType(WLAN_UART_GPIO_TX_PORT, WLAN_UART_GPIO_TX_PIN, LL_GPIO_OUTPUT_PUSHPULL);
	LL_GPIO_SetPinPull(WLAN_UART_GPIO_TX_PORT, WLAN_UART_GPIO_TX_PIN, LL_GPIO_PULL_UP);

	//Configure Rx Pin as : Alternate function, High Speed, Push pull, Pull up
	LL_GPIO_SetPinMode(WLAN_UART_GPIO_RX_PORT, WLAN_UART_GPIO_RX_PIN, LL_GPIO_MODE_ALTERNATE);
	WLAN_UART_SET_RX_AF(WLAN_UART_GPIO_RX_PORT, WLAN_UART_GPIO_RX_PIN, WLAN_UART_GPIO_RX_AF);
	LL_GPIO_SetPinSpeed(WLAN_UART_GPIO_RX_PORT, WLAN_UART_GPIO_RX_PIN, LL_GPIO_SPEED_FREQ_HIGH);
	LL_GPIO_SetPinOutputType(WLAN_UART_GPIO_RX_PORT, WLAN_UART_GPIO_RX_PIN, LL_GPIO_OUTPUT_PUSHPULL);
	LL_GPIO_SetPinPull(WLAN_UART_GPIO_RX_PORT, WLAN_UART_GPIO_RX_PIN, LL_GPIO_PULL_UP);

	/*
	 * USART CONFIG
	 */
	//Enable clock for USART3 and select Clock source
	WLAN_UART_CLK_ENABLE();
	LL_RCC_SetUSARTClockSource(WLAN_UART_CLK_SRC);

	//Set transferDirection to TX and RX
	LL_USART_SetTransferDirection(WLAN_UART_INST, LL_USART_DIRECTION_TX_RX);

	//Set Data Structure to 8N1
	LL_USART_ConfigCharacter(WLAN_UART_INST, LL_USART_DATAWIDTH_8B, LL_USART_PARITY_NONE, LL_USART_STOPBITS_1);

	//Set Baud Rate
	LL_USART_SetBaudRate(WLAN_UART_INST, SystemCoreClock, LL_USART_OVERSAMPLING_16, 115200);

	//Enable USART3
	LL_USART_Enable(WLAN_UART_INST);

	//Wait for USART being enabled
	while((!(LL_USART_IsActiveFlag_TEACK(WLAN_UART_INST))) || (!(LL_USART_IsActiveFlag_REACK(WLAN_UART_INST))))
	{

	}

	/*
	 * DMA CONFIG
	 */
	//Enable DMA-Clock
	WLAN_UART_DMA_CLK_INIT();

	//Configure NVIC for DMA transfer complete/error interrupts
	NVIC_SetPriority(WLAN_UART_TX_DMA_IRQn, WLAN_UART_TX_DMA_PRIO);
	NVIC_EnableIRQ(WLAN_UART_TX_DMA_IRQn);

	// Configure the DMA functional parameters for transmission */
	LL_DMA_ConfigTransfer(WLAN_UART_TX_DMA_INST, WLAN_UART_TX_DMA_CH,
						LL_DMA_DIRECTION_MEMORY_TO_PERIPH |
						LL_DMA_PRIORITY_HIGH              |
						LL_DMA_MODE_NORMAL                |
						LL_DMA_PERIPH_NOINCREMENT         |
						LL_DMA_MEMORY_INCREMENT           |
						LL_DMA_PDATAALIGN_BYTE            |
						LL_DMA_MDATAALIGN_BYTE);
	LL_DMA_ConfigAddresses(WLAN_UART_TX_DMA_INST, WLAN_UART_TX_DMA_CH,
						 (uint32_t)TxBuffer,
						 LL_USART_DMA_GetRegAddr(WLAN_UART_INST, LL_USART_DMA_REG_DATA_TRANSMIT),
						 LL_DMA_GetDataTransferDirection(WLAN_UART_TX_DMA_INST, WLAN_UART_TX_DMA_CH));
	LL_DMA_SetDataLength(WLAN_UART_TX_DMA_INST, WLAN_UART_TX_DMA_CH, TxLength);
	LL_DMA_SetPeriphRequest(WLAN_UART_TX_DMA_INST, WLAN_UART_TX_DMA_CH, WLAN_UART_TX_DMA_REQ);


	//Enable DMA transfer complete/error interrupts
	LL_DMA_EnableIT_TC(WLAN_UART_TX_DMA_INST, WLAN_UART_TX_DMA_CH);
	LL_DMA_EnableIT_TE(WLAN_UART_TX_DMA_INST, WLAN_UART_TX_DMA_CH);

	TransmissionComplete=1;
	TransmissionError = 0;

	/*
	 * Rx-Interrupt Config
	 */
	NVIC_SetPriority(WLAN_UART_IRQn, WLAN_UART_RX_PRIO);
	LL_USART_EnableIT_RXNE(WLAN_UART_INST);
	NVIC_EnableIRQ(WLAN_UART_IRQn);
}

void UARTStartTransfers(const char* Command)
{
	TransmissionComplete = 0;
	strcpy(TxBuffer, Command);
	TxLength = strlen(TxBuffer);
	LL_DMA_SetDataLength(WLAN_UART_TX_DMA_INST, WLAN_UART_TX_DMA_CH, TxLength);

	/* Enable DMA TX Interrupt */
	LL_USART_EnableDMAReq_TX(WLAN_UART_INST);

	/* Enable DMA Channel Tx */
	LL_DMA_EnableChannel(WLAN_UART_TX_DMA_INST, WLAN_UART_TX_DMA_CH);
}


void UARTwaitEndOfTransfer(void)
{
  /* 1 - Wait end of transmission */
  while (TransmissionComplete != 1)
  {
  }
  /* Disable DMA1 Tx Channel */
  LL_DMA_DisableChannel(WLAN_UART_TX_DMA_INST, WLAN_UART_TX_DMA_CH);
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

void WLAN_UART_TX_DMA_HANDLER()
{

  if(WLAN_UART_TX_DMA_CompleteFlag())
  {
	  WLAN_UART_TX_DMA_ClearFlag();
    /* Call function Transmission complete Callback */
    DMA_TransmitComplete_Callback();
  }
  else if(WLAN_UART_TX_DMA_ErrorFlag())
  {
    /* Call Error function */
    USART_TransferError_Callback();
  }
}


void DMA_TransmitComplete_Callback(void)
{
  //DMA Tx transfer completed
  TransmissionComplete = 1;
}

void USART_TransferError_Callback(void)
{
  /* Disable DMA1 Tx Channel */
  LL_DMA_DisableChannel(WLAN_UART_TX_DMA_INST, WLAN_UART_TX_DMA_CH);
}

void USART3_IRQHandler(void){

	if(LL_USART_IsActiveFlag_RXNE(WLAN_UART_INST)){
		USART_RecieveCallback();
	}
}

void USART_RecieveCallback(void){
	if(Characters<=999){
		char recieved = LL_USART_ReceiveData8(WLAN_UART_INST);
		Characters++;
		strncat(RxBuffer,&recieved,1);
		if(recieved=='\n'){
			Lines++;
			//debug_printf("%s",RxBuffer);
		}
	}
}

const char* UARTCheckForIPD(void){
	const char* returnval = NULL;
	uint8_t length = 0;
		if(Lines>0 && strstr(RxBuffer,"+IPD,")){
			char *ptrComma, *ptrDoppelpkt ;
			ptrComma = strchr(RxBuffer,',');
			ptrDoppelpkt = strchr(RxBuffer,':');
			if((ptrComma)&&(ptrDoppelpkt)){
				length = strtod(ptrComma+1,NULL);
				char *ptrEnd = ptrDoppelpkt+length;
				while(*ptrEnd==0){}; //warten bis alle Zeichen empfangen sind
				strncpy(IPDBuffer,ptrDoppelpkt+1,length);
				//debug_printf("%d Zeichen empfangen: %s\n\r",length,IPDBuffer);
				memset(RxBuffer,0,sizeof(RxBuffer));
				Lines = 0;
				Characters = 0;
				returnval = IPDBuffer;
			}
		}
	return returnval;
}
