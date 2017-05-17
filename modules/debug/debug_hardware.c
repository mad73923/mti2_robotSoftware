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
	LL_DMA_ConfigAddresses(DMA1, LL_DMA_CHANNEL_7,
	                         (uint32_t)buffer,
	                         LL_USART_DMA_GetRegAddr(USART2, LL_USART_DMA_REG_DATA_TRANSMIT),
	                         LL_DMA_GetDataTransferDirection(DMA1, LL_DMA_CHANNEL_7));
	LL_DMA_SetDataLength(DMA1, LL_DMA_CHANNEL_7, length);

	/* Enable DMA TX Interrupt */
	LL_USART_EnableDMAReq_TX(USART2);
	/* Enable DMA Channel Tx */
	LL_DMA_EnableChannel(DMA1, LL_DMA_CHANNEL_7);

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
  *         -1- Enable DMA1 clock
  *         -2- Configure NVIC for DMA transfer complete/error interrupts
  *         -3- Configure DMA TX channel functional parameters
  *         -4- Configure DMA RX channel functional parameters
  *         -5- Enable transfer complete/error interrupts
  * @param  None
  * @retval None
  */
void Configure_DMA(void){
  /* DMA1 used for USART2 Transmission and Reception
   */
  /* (1) Enable the clock of DMA1 */
  LL_AHB1_GRP1_EnableClock(LL_AHB1_GRP1_PERIPH_DMA1);

  /* (2) Configure NVIC for DMA transfer complete/error interrupts */
  NVIC_SetPriority(DMA1_Channel7_IRQn, 0);
  NVIC_EnableIRQ(DMA1_Channel7_IRQn);
//  NVIC_SetPriority(DMA1_Channel6_IRQn, 0);
//  NVIC_EnableIRQ(DMA1_Channel6_IRQn);

  /* (3) Configure the DMA functional parameters for transmission */
  LL_DMA_ConfigTransfer(DMA1, LL_DMA_CHANNEL_7,
                        LL_DMA_DIRECTION_MEMORY_TO_PERIPH |
                        LL_DMA_PRIORITY_HIGH              |
                        LL_DMA_MODE_NORMAL                |
                        LL_DMA_PERIPH_NOINCREMENT         |
                        LL_DMA_MEMORY_INCREMENT           |
                        LL_DMA_PDATAALIGN_BYTE            |
                        LL_DMA_MDATAALIGN_BYTE);

  LL_DMA_SetPeriphRequest(DMA1, LL_DMA_CHANNEL_7, LL_DMA_REQUEST_2);

  /* (4) Configure the DMA functional parameters for reception */
//  LL_DMA_ConfigTransfer(DMA1, LL_DMA_CHANNEL_6,
//                        LL_DMA_DIRECTION_PERIPH_TO_MEMORY |
//                        LL_DMA_PRIORITY_HIGH              |
//                        LL_DMA_MODE_NORMAL                |
//                        LL_DMA_PERIPH_NOINCREMENT         |
//                        LL_DMA_MEMORY_INCREMENT           |
//                        LL_DMA_PDATAALIGN_BYTE            |
//                        LL_DMA_MDATAALIGN_BYTE);
//  LL_DMA_ConfigAddresses(DMA1, LL_DMA_CHANNEL_6,
//                         LL_USART_DMA_GetRegAddr(USART2, LL_USART_DMA_REG_DATA_RECEIVE),
//                         (uint32_t)aRxBuffer,
//                         LL_DMA_GetDataTransferDirection(DMA1, LL_DMA_CHANNEL_6));
//  LL_DMA_SetDataLength(DMA1, LL_DMA_CHANNEL_6, ubNbDataToReceive);
//  LL_DMA_SetPeriphRequest(DMA1, LL_DMA_CHANNEL_6, LL_DMA_REQUEST_2);

  /* (5) Enable DMA transfer complete/error interrupts  */
  LL_DMA_EnableIT_TC(DMA1, LL_DMA_CHANNEL_7);
  LL_DMA_EnableIT_TE(DMA1, LL_DMA_CHANNEL_7);
//  LL_DMA_EnableIT_TC(DMA1, LL_DMA_CHANNEL_6);
//  LL_DMA_EnableIT_TE(DMA1, LL_DMA_CHANNEL_6);
}

/**
  * @brief  This function configures USARTx Instance.
  * @note   This function is used to :
  *         -1- Enable GPIO clock and configures the USART2 pins.
  *         -2- Enable the USART2 peripheral clock and clock source.
  *         -3- Configure USART2 functional parameters.
  *         -4- Enable USART2.
  * @note   Peripheral configuration is minimal configuration from reset values.
  *         Thus, some useless LL unitary functions calls below are provided as
  *         commented examples - setting is default configuration from reset.
  * @param  None
  * @retval None
  */
void Configure_USART(void){

  /* (1) Enable GPIO clock and configures the USART pins **********************/

  /* Enable the peripheral clock of GPIO Port */
  LL_AHB2_GRP1_EnableClock(LL_AHB2_GRP1_PERIPH_GPIOA);

  /* Configure Tx Pin as : Alternate function, High Speed, Push pull, Pull up */
  LL_GPIO_SetPinMode(GPIOA, LL_GPIO_PIN_2, LL_GPIO_MODE_ALTERNATE);
  LL_GPIO_SetAFPin_0_7(GPIOA, LL_GPIO_PIN_2, LL_GPIO_AF_7);
  LL_GPIO_SetPinSpeed(GPIOA, LL_GPIO_PIN_2, LL_GPIO_SPEED_FREQ_VERY_HIGH);
  LL_GPIO_SetPinOutputType(GPIOA, LL_GPIO_PIN_2, LL_GPIO_OUTPUT_PUSHPULL);
  LL_GPIO_SetPinPull(GPIOA, LL_GPIO_PIN_2, LL_GPIO_PULL_UP);

  /* Configure Rx Pin as : Alternate function, High Speed, Push pull, Pull up */
  LL_GPIO_SetPinMode(GPIOA, LL_GPIO_PIN_3, LL_GPIO_MODE_ALTERNATE);
  LL_GPIO_SetAFPin_0_7(GPIOA, LL_GPIO_PIN_3, LL_GPIO_AF_7);
  LL_GPIO_SetPinSpeed(GPIOA, LL_GPIO_PIN_3, LL_GPIO_SPEED_FREQ_VERY_HIGH);
  LL_GPIO_SetPinOutputType(GPIOA, LL_GPIO_PIN_3, LL_GPIO_OUTPUT_PUSHPULL);
  LL_GPIO_SetPinPull(GPIOA, LL_GPIO_PIN_3, LL_GPIO_PULL_UP);

  /* (2) Enable USART2 peripheral clock and clock source ****************/
  LL_APB1_GRP1_EnableClock(LL_APB1_GRP1_PERIPH_USART2);

  /* Set clock source */
  LL_RCC_SetUSARTClockSource(LL_RCC_USART2_CLKSOURCE_PCLK1);

  /* (3) Configure USART2 functional parameters ********************************/

  /* Disable USART prior modifying configuration registers */
  /* Note: Commented as corresponding to Reset value */
  // LL_USART_Disable(USART2);

  /* TX/RX direction */
  LL_USART_SetTransferDirection(USART2, LL_USART_DIRECTION_TX_RX);

  /* 8 data bit, 1 start bit, 1 stop bit, no parity */
  LL_USART_ConfigCharacter(USART2, LL_USART_DATAWIDTH_8B, LL_USART_PARITY_NONE, LL_USART_STOPBITS_1);

  /* No Hardware Flow control */
  /* Reset value is LL_USART_HWCONTROL_NONE */
  // LL_USART_SetHWFlowCtrl(USART2, LL_USART_HWCONTROL_NONE);

  /* Oversampling by 16 */
  /* Reset value is LL_USART_OVERSAMPLING_16 */
  // LL_USART_SetOverSampling(USART2, LL_USART_OVERSAMPLING_16);

  /* Set Baudrate to 115200 using APB frequency set to 80000000 Hz */
  /* Frequency available for USART peripheral can also be calculated through LL RCC macro */
  /* Ex :
      Periphclk = LL_RCC_GetUSARTClockFreq(Instance); or LL_RCC_GetUARTClockFreq(Instance); depending on USART/UART instance

      In this example, Peripheral Clock is expected to be equal to 80000000 Hz => equal to SystemCoreClock
  */
  LL_USART_SetBaudRate(USART2, SystemCoreClock, LL_USART_OVERSAMPLING_16, 115200);

  /* (4) Enable USART2 **********************************************************/
  LL_USART_Enable(USART2);

  /* Polling USART initialisation */
  while((!(LL_USART_IsActiveFlag_TEACK(USART2))) || (!(LL_USART_IsActiveFlag_REACK(USART2))))
  {
  }
}

/*
 * Interrupt functions
 */

/**
  * @brief  This function handles DMA1 interrupt request.
  * @param  None
  * @retval None
  */
void DMA1_Channel7_IRQHandler(void)
{

  if(LL_DMA_IsActiveFlag_TC7(DMA1))
  {
    LL_DMA_ClearFlag_GI7(DMA1);
    transmissionComplete = 1;
    /* Disable DMA1 Tx Channel */
    LL_DMA_DisableChannel(DMA1, LL_DMA_CHANNEL_7);
  }
  else if(LL_DMA_IsActiveFlag_TE7(DMA1))
  {
    /* Call Error function */
    //USART_TransferError_Callback();
  }
}
