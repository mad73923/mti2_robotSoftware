/*
 * SPI.c
 *
 *  Created on: 11.05.2017
 *      Author: matthias
 */

#include "SPI.h"

uint16_t* dataTX;
uint32_t nbDataToTransmit;
volatile uint32_t nbIndexTransmit;

uint16_t* dataRX;
uint32_t nbDataToReceive;
volatile uint32_t nbIndexReceive;

/*
 * Private function prototypes
 */

void SPI1_Tx_Callback(void);
void SPI1_Rx_Callback(void);
void SPI_cleanRXBuffer(void);

/*
 * Public functions
 */

void SPI_init(void){
	/* (1) Enables GPIO clock and configures the SPI1 pins ********************/
	/* Enable the peripheral clock of GPIOB */
	LL_AHB2_GRP1_EnableClock(LL_AHB2_GRP1_PERIPH_GPIOB);

	/* Configure SCK Pin connected to pin 31 of CN10 connector */
	LL_GPIO_SetPinMode(GPIOB, LL_GPIO_PIN_3, LL_GPIO_MODE_ALTERNATE);
	LL_GPIO_SetAFPin_0_7(GPIOB, LL_GPIO_PIN_3, LL_GPIO_AF_5);
	LL_GPIO_SetPinSpeed(GPIOB, LL_GPIO_PIN_3, LL_GPIO_SPEED_FREQ_VERY_HIGH);
	LL_GPIO_SetPinPull(GPIOB, LL_GPIO_PIN_3, LL_GPIO_PULL_DOWN);

	/* Configure MOSI Pin connected to pin 29 of CN10 connector */
	LL_GPIO_SetPinMode(GPIOB, LL_GPIO_PIN_5, LL_GPIO_MODE_ALTERNATE);
	LL_GPIO_SetAFPin_0_7(GPIOB, LL_GPIO_PIN_5, LL_GPIO_AF_5);
	LL_GPIO_SetPinSpeed(GPIOB, LL_GPIO_PIN_5, LL_GPIO_SPEED_FREQ_VERY_HIGH);
	LL_GPIO_SetPinPull(GPIOB, LL_GPIO_PIN_5, LL_GPIO_PULL_DOWN);

	/* (2) Configure NVIC for SPI1 transfer complete/error interrupts **********/
	/* Set priority for SPI1_IRQn */
	NVIC_SetPriority(SPI1_IRQn, 0);
	/* Enable SPI1_IRQn           */
	NVIC_EnableIRQ(SPI1_IRQn);

	/* (3) Configure SPI1 functional parameters ********************************/
	/* Enable the peripheral clock of GPIOB */
	LL_APB2_GRP1_EnableClock(LL_APB2_GRP1_PERIPH_SPI1);

	/* Configure SPI1 communication */
	LL_SPI_SetBaudRatePrescaler(SPI1, LL_SPI_BAUDRATEPRESCALER_DIV256);
	LL_SPI_SetTransferDirection(SPI1,LL_SPI_HALF_DUPLEX_TX);
	LL_SPI_SetClockPhase(SPI1, LL_SPI_PHASE_2EDGE);
	LL_SPI_SetClockPolarity(SPI1, LL_SPI_POLARITY_LOW);
	/* Reset value is LL_SPI_MSB_FIRST */
	LL_SPI_SetTransferBitOrder(SPI1, LL_SPI_MSB_FIRST);
	LL_SPI_SetDataWidth(SPI1, LL_SPI_DATAWIDTH_16BIT);
	LL_SPI_SetNSSMode(SPI1, LL_SPI_NSS_SOFT);
	LL_SPI_SetRxFIFOThreshold(SPI1, LL_SPI_RX_FIFO_TH_HALF);
	LL_SPI_SetMode(SPI1, LL_SPI_MODE_MASTER);

	/* Configure SPI1 transfer interrupts */
	/* Enable TXE   Interrupt */
	LL_SPI_EnableIT_TXE(SPI1);
	/* Enable RXNE   Interrupt */
	LL_SPI_EnableIT_RXNE(SPI1);
	/* Enable SPI1 Error Interrupt */
	LL_SPI_EnableIT_ERR(SPI1);
}

void SPI_communicate_sync(uint16_t* pdataTX, uint32_t ndataTX, uint16_t* pdataRX, uint32_t ndataRX){
	dataTX = pdataTX;
	dataRX = pdataRX;
	nbDataToReceive = ndataRX;
	nbDataToTransmit = ndataTX;
	nbIndexReceive = 0;
	nbIndexTransmit = 0;

	LL_SPI_EnableIT_TXE(SPI1);
	LL_SPI_SetTransferDirection(SPI1, LL_SPI_HALF_DUPLEX_TX);
	LL_SPI_Enable(SPI1);
	while(nbIndexTransmit<nbDataToTransmit);
	while(LL_SPI_IsActiveFlag_BSY(SPI1));


	LL_SPI_Disable(SPI1);
	LL_SPI_SetTransferDirection(SPI1, LL_SPI_HALF_DUPLEX_RX);
	SPI_cleanRXBuffer();
	LL_SPI_EnableIT_RXNE(SPI1);
	LL_SPI_Enable(SPI1);

	while(nbIndexReceive<nbDataToReceive);
	LL_SPI_Disable(SPI1);
}

void SPI_waitForClearance(void){
	while(LL_SPI_IsActiveFlag_BSY(SPI1));
}

/*
 * Private functions
 */

void SPI_cleanRXBuffer(void){
	while(LL_SPI_IsActiveFlag_RXNE(SPI1))
		LL_SPI_ReceiveData16(SPI1);
	nbIndexReceive = 0;
}

void SPI1_Tx_Callback(void){
	if(nbIndexTransmit<nbDataToTransmit){
		LL_SPI_TransmitData16(SPI1, dataTX[nbIndexTransmit]);
		nbIndexTransmit++;
	}else{
		LL_SPI_DisableIT_TXE(SPI1);
	}
}

void SPI1_Rx_Callback(void){
	if(nbIndexReceive<nbDataToReceive){
		dataRX[nbIndexReceive] = LL_SPI_ReceiveData16(SPI1);
		nbIndexReceive++;
	}else{
		LL_SPI_DisableIT_RXNE(SPI1);
	}
}

/*
 * Interrupt handler
 */

void SPI1_IRQHandler(void){
	/* Check TXE flag value in ISR register */
	if(LL_SPI_IsActiveFlag_TXE(SPI1)){
		/* Call function Master Transmission Callback */
		SPI1_Tx_Callback();
	}
	/* Check RXNE flag value in ISR register */
	if(LL_SPI_IsActiveFlag_RXNE(SPI1)){
		/* Call function Slave Reception Callback */
		SPI1_Rx_Callback();
	}
}
