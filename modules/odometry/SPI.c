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
void (*TLE_SPI_Tx_done)(void);

uint16_t* dataRX;
uint32_t nbDataToReceive;
volatile uint32_t nbIndexReceive;
void (*TLE_SPI_Rx_done)(void);

void (*SPI_communicate_async_done_callback)(void);

/*
 * Private function prototypes
 */

void TLE_SPI_Tx_Callback(void);
void TLE_SPI_Rx_Callback(void);
void SPI_cleanRXBuffer(void);
void SPI_setParamsToVariables(uint16_t* pdataTX, uint32_t ndataTX, uint16_t* pdataRX, uint32_t ndataRX);

void SPI_setTxDoneCallback(void* callback);
void SPI_resetTxDoneCallback(void);
void SPI_setRxDoneCallback(void* callback);
void SPI_resetRxDoneCallback(void);
void SPI_communicate_async_TxDoneCallback(void);
void SPI_communicate_async_RxDoneCallback(void);

void SPI_enableTX(void);
void SPI_enableRX(void);

/*
 * Public functions
 */

void SPI_init(void){
	/* Configure SCK Pin */
	TLE_GPIO_SCK_CLK_INIT();
	LL_GPIO_SetPinMode(TLE_GPIO_SCK, TLE_GPIO_SCK_PIN, LL_GPIO_MODE_ALTERNATE);
	TLE_GPIO_SCK_SETAF(TLE_GPIO_SCK, TLE_GPIO_SCK_PIN, TLE_GPIO_SCK_AF);
	LL_GPIO_SetPinSpeed(TLE_GPIO_SCK, TLE_GPIO_SCK_PIN, LL_GPIO_SPEED_FREQ_VERY_HIGH);
	LL_GPIO_SetPinPull(TLE_GPIO_SCK, TLE_GPIO_SCK_PIN, LL_GPIO_PULL_DOWN);

	/* Configure MOSI Pin */
	TLE_GPIO_MOSI_CLK_INIT();
	LL_GPIO_SetPinMode(TLE_GPIO_MOSI, TLE_GPIO_MOSI_PIN, LL_GPIO_MODE_ALTERNATE);
	TLE_GPIO_MOSI_SETAF(TLE_GPIO_MOSI, TLE_GPIO_MOSI_PIN, TLE_GPIO_MOSI_AF);
	LL_GPIO_SetPinSpeed(TLE_GPIO_MOSI, TLE_GPIO_MOSI_PIN, LL_GPIO_SPEED_FREQ_VERY_HIGH);
	LL_GPIO_SetPinPull(TLE_GPIO_MOSI, TLE_GPIO_MOSI_PIN, LL_GPIO_PULL_DOWN);

	/* (2) Configure NVIC for TLE_SPI_INST transfer complete/error interrupts **********/
	/* Set priority for TLE_SPI_INST_IRQn */
	NVIC_SetPriority(TLE_SPI_IRQN, TLE_SPI_IRQ_PRIO);
	/* Enable TLE_SPI_INST_IRQn           */
	NVIC_EnableIRQ(TLE_SPI_IRQN);

	/* (3) Configure TLE_SPI_INST functional parameters ********************************/
	/* Enable the peripheral clock of GPIOB */
	TLE_SPI_CLK_INIT();

	/* Configure TLE_SPI_INST communication */
	LL_SPI_SetBaudRatePrescaler(TLE_SPI_INST, LL_SPI_BAUDRATEPRESCALER_DIV256);
	LL_SPI_SetTransferDirection(TLE_SPI_INST,LL_SPI_HALF_DUPLEX_TX);
	LL_SPI_SetClockPhase(TLE_SPI_INST, LL_SPI_PHASE_2EDGE);
	LL_SPI_SetClockPolarity(TLE_SPI_INST, LL_SPI_POLARITY_LOW);
	/* Reset value is LL_SPI_MSB_FIRST */
	LL_SPI_SetTransferBitOrder(TLE_SPI_INST, LL_SPI_MSB_FIRST);
	LL_SPI_SetDataWidth(TLE_SPI_INST, LL_SPI_DATAWIDTH_16BIT);
	LL_SPI_SetNSSMode(TLE_SPI_INST, LL_SPI_NSS_SOFT);
	LL_SPI_SetRxFIFOThreshold(TLE_SPI_INST, LL_SPI_RX_FIFO_TH_HALF);
	LL_SPI_SetMode(TLE_SPI_INST, LL_SPI_MODE_MASTER);

	/* Configure TLE_SPI_INST transfer interrupts */
	/* Enable TXE   Interrupt */
	LL_SPI_EnableIT_TXE(TLE_SPI_INST);
	/* Enable RXNE   Interrupt */
	LL_SPI_EnableIT_RXNE(TLE_SPI_INST);
}

void SPI_communicate_sync(uint16_t* pdataTX, uint32_t ndataTX, uint16_t* pdataRX, uint32_t ndataRX){
	SPI_setParamsToVariables(pdataTX, ndataTX, pdataRX, ndataRX);

	SPI_enableTX();
	while(nbIndexTransmit<nbDataToTransmit);
	SPI_waitForClearance();


	LL_SPI_Disable(TLE_SPI_INST);
	if(nbDataToReceive > 0){
		SPI_enableRX();
		while(nbIndexReceive<nbDataToReceive);
		LL_SPI_Disable(TLE_SPI_INST);
	}
}

void SPI_communicate_async(uint16_t* pdataTX, uint32_t ndataTX, uint16_t* pdataRX, uint32_t ndataRX, void* doneCallback){
	SPI_setParamsToVariables(pdataTX, ndataTX, pdataRX, ndataRX);
	SPI_setTxDoneCallback(SPI_communicate_async_TxDoneCallback);
	SPI_setRxDoneCallback(SPI_communicate_async_RxDoneCallback);
	SPI_communicate_async_done_callback = doneCallback;
	SPI_enableTX();
}

void SPI_waitForClearance(void){
	while(LL_SPI_IsActiveFlag_BSY(TLE_SPI_INST));
}

/*
 * Private functions
 */

void SPI_setParamsToVariables(uint16_t* pdataTX, uint32_t ndataTX, uint16_t* pdataRX, uint32_t ndataRX){
	dataTX = pdataTX;
	dataRX = pdataRX;
	nbDataToReceive = ndataRX;
	nbDataToTransmit = ndataTX;
	nbIndexReceive = 0;
	nbIndexTransmit = 0;
}

void SPI_communicate_async_TxDoneCallback(void){
	SPI_waitForClearance();
	LL_SPI_Disable(TLE_SPI_INST);
	if(nbDataToReceive>0){
		SPI_enableRX();
	}else{
		SPI_communicate_async_RxDoneCallback();
	}
}

void SPI_communicate_async_RxDoneCallback(void){
	SPI_resetRxDoneCallback();
	SPI_resetTxDoneCallback();
	LL_SPI_Disable(TLE_SPI_INST);
	if(SPI_communicate_async_done_callback != 0){
		SPI_communicate_async_done_callback();
	}
}

void SPI_setTxDoneCallback(void* callback){
	TLE_SPI_Tx_done = callback;
}

void SPI_resetTxDoneCallback(void){
	TLE_SPI_Tx_done = 0;
}

void SPI_setRxDoneCallback(void* callback){
	TLE_SPI_Rx_done = callback;
}

void SPI_resetRxDoneCallback(void){
	TLE_SPI_Rx_done = 0;
}

void SPI_enableTX(void){
	LL_SPI_EnableIT_TXE(TLE_SPI_INST);
	LL_SPI_SetTransferDirection(TLE_SPI_INST, LL_SPI_HALF_DUPLEX_TX);
	LL_SPI_Enable(TLE_SPI_INST);
}

void SPI_enableRX(void){
	LL_SPI_SetTransferDirection(TLE_SPI_INST, LL_SPI_HALF_DUPLEX_RX);
	SPI_cleanRXBuffer();
	LL_SPI_EnableIT_RXNE(TLE_SPI_INST);
	LL_SPI_Enable(TLE_SPI_INST);
}

void SPI_cleanRXBuffer(void){
	while(LL_SPI_IsActiveFlag_RXNE(TLE_SPI_INST))
		LL_SPI_ReceiveData16(TLE_SPI_INST);
	nbIndexReceive = 0;
}

void TLE_SPI_Tx_Callback(void){
	if(nbIndexTransmit<nbDataToTransmit){
		LL_SPI_TransmitData16(TLE_SPI_INST, dataTX[nbIndexTransmit]);
		nbIndexTransmit++;
	}else{
		LL_SPI_DisableIT_TXE(TLE_SPI_INST);
		if(TLE_SPI_Tx_done != 0){
			TLE_SPI_Tx_done();
		}
	}
}

void TLE_SPI_Rx_Callback(void){
	if(nbIndexReceive<nbDataToReceive){
		dataRX[nbIndexReceive] = LL_SPI_ReceiveData16(TLE_SPI_INST);
		nbIndexReceive++;
	}else{
		LL_SPI_DisableIT_RXNE(TLE_SPI_INST);
		if(TLE_SPI_Rx_done != 0){
			TLE_SPI_Rx_done();
		}
	}
}

/*
 * Interrupt handler
 */

void TLE_SPI_IRQ_HANDLER(void){
	/* Check TXE flag value in ISR register */
	if(LL_SPI_IsActiveFlag_TXE(TLE_SPI_INST) && LL_SPI_IsEnabledIT_TXE(TLE_SPI_INST)){
		/* Call function Master Transmission Callback */
		TLE_SPI_Tx_Callback();
	}
	/* Check RXNE flag value in ISR register */
	if(LL_SPI_IsActiveFlag_RXNE(TLE_SPI_INST) && LL_SPI_IsEnabledIT_RXNE(TLE_SPI_INST)){
		/* Call function Slave Reception Callback */
		TLE_SPI_Rx_Callback();
	}
}
