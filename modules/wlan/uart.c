/*
 * uart.c
 *
 *  Created on: 17.05.2017
 *      Author: thomas
 */

#include "uart.h"

void DMA1_TransmitComplete_Callback(void);
void USART_TransferError_Callback(void);

uint8_t TxLength;
uint8_t TxBuffer[50];
volatile uint8_t TransmissionComplete;
volatile uint8_t TransmissionError;


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