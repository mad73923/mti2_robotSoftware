/*
 * hardware.c
 *
 *  Created on: 17.05.2017
 *      Author: matthias
 */

#include "debug_hardware.h"

/*
 * Variables
 */

volatile uint8_t transmissionComplete;

/*
 * Private function prototypes
 */

void Configure_DMA(void);
void Configure_USART(void);

/*
 * Public functions
 */

void debug_hardware_init(void){
	Configure_DMA();
	Configure_USART();

	transmissionComplete = 1;
}

void uart_send(uint8_t* buffer, uint32_t length){
	transmissionComplete = 0;
	LL_DMA_ConfigAddresses(DEBUG_UART_TX_DMA_INST, DEBUG_UART_TX_DMA_CH,
	                         (uint32_t)buffer,
	                         LL_USART_DMA_GetRegAddr(DEBUG_UART_INST, LL_USART_DMA_REG_DATA_TRANSMIT),
	                         LL_DMA_GetDataTransferDirection(DEBUG_UART_TX_DMA_INST, DEBUG_UART_TX_DMA_CH));
	LL_DMA_SetDataLength(DEBUG_UART_TX_DMA_INST, DEBUG_UART_TX_DMA_CH, length);

	/* Enable DMA TX Interrupt */
	LL_USART_EnableDMAReq_TX(DEBUG_UART_INST);
	/* Enable DMA Channel Tx */
	LL_DMA_EnableChannel(DEBUG_UART_TX_DMA_INST, DEBUG_UART_TX_DMA_CH);

}

void uart_waitTransmissionComplete(void){
	while(transmissionComplete==0);
}

/*
 * Private functions
 */


/**
  * @brief  This function configures the DMA Channels for TX and RX transfers
  * @note   This function is used to :
  *         -1- Enable DEBUG_UART_TX_DMA_INST clock
  *         -2- Configure NVIC for DMA transfer complete/error interrupts
  *         -3- Configure DMA TX channel functional parameters
  *         -4- Configure DMA RX channel functional parameters
  *         -5- Enable transfer complete/error interrupts
  * @param  None
  * @retval None
  */
void Configure_DMA(void){
  /* DEBUG_UART_TX_DMA_INST used for DEBUG_UART_INST Transmission and Reception
   */
  /* (1) Enable the clock of DEBUG_UART_TX_DMA_INST */
	DEBUG_UART_DMA_CLK_INIT();

  /* (2) Configure NVIC for DMA transfer complete/error interrupts */
  NVIC_SetPriority(DEBUG_UART_TX_DMA_IRQn, DEBUG_UART_TX_DMA_PRIO);
  NVIC_EnableIRQ(DEBUG_UART_TX_DMA_IRQn);

  /* (3) Configure the DMA functional parameters for transmission */
  LL_DMA_ConfigTransfer(DEBUG_UART_TX_DMA_INST, DEBUG_UART_TX_DMA_CH,
                        LL_DMA_DIRECTION_MEMORY_TO_PERIPH |
                        LL_DMA_PRIORITY_HIGH              |
                        LL_DMA_MODE_NORMAL                |
                        LL_DMA_PERIPH_NOINCREMENT         |
                        LL_DMA_MEMORY_INCREMENT           |
                        LL_DMA_PDATAALIGN_BYTE            |
                        LL_DMA_MDATAALIGN_BYTE);

  LL_DMA_SetPeriphRequest(DEBUG_UART_TX_DMA_INST, DEBUG_UART_TX_DMA_CH, DEBUG_UART_TX_DMA_REQ);

  /* (5) Enable DMA transfer complete/error interrupts  */
  LL_DMA_EnableIT_TC(DEBUG_UART_TX_DMA_INST, DEBUG_UART_TX_DMA_CH);
  LL_DMA_EnableIT_TE(DEBUG_UART_TX_DMA_INST, DEBUG_UART_TX_DMA_CH);
}

/**
  * @brief  This function configures USARTx Instance.
  * @note   This function is used to :
  *         -1- Enable GPIO clock and configures the DEBUG_UART_INST pins.
  *         -2- Enable the DEBUG_UART_INST peripheral clock and clock source.
  *         -3- Configure DEBUG_UART_INST functional parameters.
  *         -4- Enable DEBUG_UART_INST.
  * @note   Peripheral configuration is minimal configuration from reset values.
  *         Thus, some useless LL unitary functions calls below are provided as
  *         commented examples - setting is default configuration from reset.
  * @param  None
  * @retval None
  */
void Configure_USART(void){

  /* (1) Enable GPIO clock and configures the USART pins **********************/

  /* Enable the peripheral clock of GPIO Port */
	DEBUG_UART_GPIO_CLK_INIT();

  /* Configure Tx Pin as : Alternate function, High Speed, Push pull, Pull up */
  LL_GPIO_SetPinMode(DEBUG_UART_TX_GPIO_PORT, DEBUG_UART_TX_GPIO_PIN, LL_GPIO_MODE_ALTERNATE);
  DEBUG_UART_SET_AF(DEBUG_UART_TX_GPIO_PORT, DEBUG_UART_TX_GPIO_PIN, DEBUG_UART_TX_AF);
  LL_GPIO_SetPinSpeed(DEBUG_UART_TX_GPIO_PORT, DEBUG_UART_TX_GPIO_PIN, LL_GPIO_SPEED_FREQ_VERY_HIGH);
  LL_GPIO_SetPinOutputType(DEBUG_UART_TX_GPIO_PORT, DEBUG_UART_TX_GPIO_PIN, LL_GPIO_OUTPUT_PUSHPULL);
  LL_GPIO_SetPinPull(DEBUG_UART_TX_GPIO_PORT, DEBUG_UART_TX_GPIO_PIN, LL_GPIO_PULL_UP);

  /* (2) Enable DEBUG_UART_INST peripheral clock and clock source ****************/
  DEBUG_UART_CLK_EN();

  /* Set clock source */
  LL_RCC_SetUSARTClockSource(DEBUG_UART_CLK_SRC);

  /* (3) Configure DEBUG_UART_INST functional parameters ********************************/

  /* Disable USART prior modifying configuration registers */
  /* Note: Commented as corresponding to Reset value */
  // LL_USART_Disable(DEBUG_UART_INST);

  /* TX/RX direction */
  LL_USART_SetTransferDirection(DEBUG_UART_INST, LL_USART_DIRECTION_TX);

  /* 8 data bit, 1 start bit, 1 stop bit, no parity */
  LL_USART_ConfigCharacter(DEBUG_UART_INST, LL_USART_DATAWIDTH_8B, LL_USART_PARITY_NONE, LL_USART_STOPBITS_1);

  /* No Hardware Flow control */
  /* Reset value is LL_USART_HWCONTROL_NONE */
  // LL_USART_SetHWFlowCtrl(DEBUG_UART_INST, LL_USART_HWCONTROL_NONE);

  /* Oversampling by 16 */
  /* Reset value is LL_USART_OVERSAMPLING_16 */
  // LL_USART_SetOverSampling(DEBUG_UART_INST, LL_USART_OVERSAMPLING_16);

  /* Set Baudrate to 115200 using APB frequency set to 80000000 Hz */
  /* Frequency available for USART peripheral can also be calculated through LL RCC macro */
  /* Ex :
      Periphclk = LL_RCC_GetUSARTClockFreq(Instance); or LL_RCC_GetUARTClockFreq(Instance); depending on USART/UART instance

      In this example, Peripheral Clock is expected to be equal to 80000000 Hz => equal to SystemCoreClock
  */
  LL_USART_SetBaudRate(DEBUG_UART_INST, SystemCoreClock, LL_USART_OVERSAMPLING_16, 115200);

  /* (4) Enable DEBUG_UART_INST **********************************************************/
  LL_USART_Enable(DEBUG_UART_INST);

  /* Polling USART initialisation */
  while(!(LL_USART_IsActiveFlag_TEACK(DEBUG_UART_INST)))
  {
  }
}

/*
 * Interrupt functions
 */

/**
  * @brief  This function handles DEBUG_UART_TX_DMA_INST interrupt request.
  * @param  None
  * @retval None
  */
void DEBUG_UART_TX_DMA_HANDLER()
{

  if(LL_DMA_IsActiveFlag_TC7(DEBUG_UART_TX_DMA_INST))
  {
    LL_DMA_ClearFlag_GI7(DEBUG_UART_TX_DMA_INST);
    transmissionComplete = 1;
    /* Disable DEBUG_UART_TX_DMA_INST Tx Channel */
    LL_DMA_DisableChannel(DEBUG_UART_TX_DMA_INST, DEBUG_UART_TX_DMA_CH);
  }
  else if(LL_DMA_IsActiveFlag_TE7(DEBUG_UART_TX_DMA_INST))
  {
    /* Call Error function */
    //USART_TransferError_Callback();
  }
}
