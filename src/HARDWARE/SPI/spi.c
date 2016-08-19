#include "spi.h"
#include "stm32f10x_dma.h"
#include "stm32f10x_spi.h"
#include "delay.h" 
#include "hw_config.h"

/* Do not place const in front of declarations.                  *
 * const variables are stored in flash that needs a 2-cycle wait */
uint8_t  DMA_Clk_Buf = 0;

/* Refer to USB IO for bulk transfer */
extern uint8_t  USB_Tx_Buf[];
extern uint16_t USB_Tx_ptr_in;
extern uint8_t  USB_Rx_Buf[];
extern uint16_t USB_Rx_ptr_out;
extern uint8_t  USB_Rx_len;

extern void usb_putp(void);
extern void usb_getp(void);
extern char usb_getc(void);


DMA_InitTypeDef DMA_InitStructure_RX;
DMA_InitTypeDef DMA_InitStructure_TX;
DMA_InitTypeDef DMA_InitStructure_CLK;
SPI_InitTypeDef SPI_InitStructure;


void SPI_IO_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
#ifdef USE_SPI2
	RCC_APB2PeriphClockCmd(	RCC_APB2Periph_GPIOB, ENABLE );
  RCC_APB1PeriphClockCmd(	RCC_APB1Periph_SPI2 , ENABLE );
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13 | GPIO_Pin_15;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;  //¸´ÓÃÍÆÍìÊä³ö
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
  GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_14;
  GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_IN_FLOATING;
//	GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_AF_PP;
  GPIO_Init(GPIOB, &GPIO_InitStructure);
	GPIO_InitStructure.GPIO_Pin   = SPI_SC_PIN;
  GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_Out_PP;
  GPIO_Init(SPI_SC_PORT, &GPIO_InitStructure);
 	GPIO_SetBits(SPI_SC_PORT,SPI_SC_PIN);
#endif
#ifdef USE_SPI1
	RCC_APB2PeriphClockCmd(	RCC_APB2Periph_GPIOA|RCC_APB2Periph_SPI1, ENABLE );
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5 | GPIO_Pin_7;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;  
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
  GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_6;
  GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_IN_FLOATING;
  GPIO_Init(GPIOA, &GPIO_InitStructure);
	GPIO_InitStructure.GPIO_Pin   = SPI_SC_PIN;
  GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_Out_PP;
  GPIO_Init(SPI_SC_PORT, &GPIO_InitStructure);
 	GPIO_SetBits(SPI_SC_PORT,SPI_SC_PIN);
#endif	
}   

void DMA_configuration(void)
{
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1|RCC_AHBPeriph_DMA2, ENABLE);	
	DMA_DeInit(SPI_RX_DMA_CH);
  DMA_DeInit(SPI_TX_DMA_CH);
	
	DMA_InitStructure_RX.DMA_PeripheralBaseAddr = (uint32_t)SPI_DR_Base;
	DMA_InitStructure_RX.DMA_MemoryBaseAddr     = (uint32_t)USB_Tx_Buf;
	DMA_InitStructure_RX.DMA_DIR                = DMA_DIR_PeripheralSRC;
	DMA_InitStructure_RX.DMA_BufferSize         = VCP_DATA_SIZE;
	DMA_InitStructure_RX.DMA_PeripheralInc      = DMA_PeripheralInc_Disable;
	DMA_InitStructure_RX.DMA_MemoryInc          = DMA_MemoryInc_Enable;
	DMA_InitStructure_RX.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;
	DMA_InitStructure_RX.DMA_MemoryDataSize     = DMA_MemoryDataSize_Byte;
	DMA_InitStructure_RX.DMA_Mode               = DMA_Mode_Normal;
	DMA_InitStructure_RX.DMA_Priority           = DMA_Priority_High;
	DMA_InitStructure_RX.DMA_M2M                = DMA_M2M_Disable;
	
  DMA_InitStructure_TX.DMA_PeripheralBaseAddr = (uint32_t)SPI_DR_Base;
  DMA_InitStructure_TX.DMA_MemoryBaseAddr     = (uint32_t)USB_Rx_Buf;
  DMA_InitStructure_TX.DMA_DIR                = DMA_DIR_PeripheralDST;
  DMA_InitStructure_TX.DMA_BufferSize         = VCP_DATA_SIZE;
  DMA_InitStructure_TX.DMA_PeripheralInc      = DMA_PeripheralInc_Disable;
  DMA_InitStructure_TX.DMA_MemoryInc          = DMA_MemoryInc_Enable;
  DMA_InitStructure_TX.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;
  DMA_InitStructure_TX.DMA_MemoryDataSize     = DMA_MemoryDataSize_Byte;
  DMA_InitStructure_TX.DMA_Mode               = DMA_Mode_Normal;
  DMA_InitStructure_TX.DMA_Priority           = DMA_Priority_Low;
  DMA_InitStructure_TX.DMA_M2M                = DMA_M2M_Disable;;	
	
  DMA_InitStructure_CLK.DMA_PeripheralBaseAddr = (uint32_t)SPI_DR_Base;
  DMA_InitStructure_CLK.DMA_MemoryBaseAddr     = (uint32_t)&DMA_Clk_Buf;
  DMA_InitStructure_CLK.DMA_DIR                = DMA_DIR_PeripheralDST;
  DMA_InitStructure_CLK.DMA_BufferSize         = VCP_DATA_SIZE;
  DMA_InitStructure_CLK.DMA_PeripheralInc      = DMA_PeripheralInc_Disable;
  DMA_InitStructure_CLK.DMA_MemoryInc          = DMA_MemoryInc_Disable;
  DMA_InitStructure_CLK.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;
  DMA_InitStructure_CLK.DMA_MemoryDataSize     = DMA_MemoryDataSize_Byte;
  DMA_InitStructure_CLK.DMA_Mode               = DMA_Mode_Normal;
  DMA_InitStructure_CLK.DMA_Priority           = DMA_Priority_Low;
  DMA_InitStructure_CLK.DMA_M2M                = DMA_M2M_Disable;
}

void dma_conf_spiwrite(void) {
	
  DMA_Init(SPI_RX_DMA_CH, &DMA_InitStructure_RX);
  DMA_Init(SPI_TX_DMA_CH, &DMA_InitStructure_TX);
}

void dma_conf_spiread(void) {
  DMA_Init(SPI_RX_DMA_CH, &DMA_InitStructure_RX);
  DMA_Init(SPI_TX_DMA_CH, &DMA_InitStructure_CLK);
}

void dma_commit(void) {
  /* Always enable TX channel first, especially under 36MHz clock. */
  SPI_I2S_DMACmd(SPI_BUS_USED, SPI_I2S_DMAReq_Tx | SPI_I2S_DMAReq_Rx, ENABLE);
  DMA_Cmd(SPI_RX_DMA_CH, ENABLE);
  DMA_Cmd(SPI_TX_DMA_CH, ENABLE);

  /* Wait for transfer to complete */
  while(!DMA_GetFlagStatus(SPI_RX_DMA_FLAG));

  /* Clear up */
  SPI_I2S_DMACmd(SPI_BUS_USED, SPI_I2S_DMAReq_Tx | SPI_I2S_DMAReq_Rx, DISABLE);
  DMA_DeInit(SPI_RX_DMA_CH);
  DMA_DeInit(SPI_TX_DMA_CH);
}

uint32_t spi_conf(uint32_t speed_hz) {
  static uint16_t clkdiv;
  static uint32_t relspd;
	
  SPI_InitStructure.SPI_Direction          = SPI_Direction_2Lines_FullDuplex;
  SPI_InitStructure.SPI_Mode               = SPI_Mode_Master;
  SPI_InitStructure.SPI_DataSize           = SPI_DataSize_8b;
//SPI_InitStructure.SPI_CPOL               = SPI_CPOL_High;
//SPI_InitStructure.SPI_CPHA               = SPI_CPHA_2Edge;
	SPI_InitStructure.SPI_CPOL               = SPI_CPOL_Low;
  SPI_InitStructure.SPI_CPHA               = SPI_CPHA_1Edge;
  SPI_InitStructure.SPI_NSS                = SPI_NSS_Soft;
  SPI_InitStructure.SPI_FirstBit           = SPI_FirstBit_MSB;
  SPI_InitStructure.SPI_CRCPolynomial      = 7;	
	
  /* SPI_BUS_USED is on APB2 which runs @ 72MHz. */
  /* Lowest available */
  clkdiv = SPI_BaudRatePrescaler_256;
  relspd = 281250;
  if(speed_hz >= 562500) {
    clkdiv = SPI_BaudRatePrescaler_128;
    relspd = 562500;
  }
  if(speed_hz >= 1125000) {
    clkdiv = SPI_BaudRatePrescaler_64;
    relspd = 1125000;
  }
  if(speed_hz >= 2250000) {
    clkdiv = SPI_BaudRatePrescaler_32;
    relspd = 2250000;
  }
  if(speed_hz >= 4500000) {
    clkdiv = SPI_BaudRatePrescaler_16;
    relspd = 4500000;
  }
  if(speed_hz >= 9000000) {
    clkdiv = SPI_BaudRatePrescaler_8;
    relspd = 9000000;
  }
  if(speed_hz >= 18000000) {
    clkdiv = SPI_BaudRatePrescaler_4;
    relspd = 18000000;
  }
  if(speed_hz >= 36000000) {
    clkdiv = SPI_BaudRatePrescaler_2;
    relspd = 36000000;
  }

  SPI_I2S_DeInit(SPI_BUS_USED);

  SPI_InitStructure.SPI_BaudRatePrescaler = clkdiv;

  SPI_Init(SPI_BUS_USED, &SPI_InitStructure);
  SPI_CalculateCRC(SPI_BUS_USED, DISABLE);
  SPI_Cmd(SPI_BUS_USED, ENABLE);
  return relspd;
}

void spi_putc(uint8_t c) {
  /* transmit c on the SPI bus */
  SPI_I2S_SendData(SPI_BUS_USED, c);

  /* Those useless data just needs to be collected, or SPI engine will go crazy. */
  while(SPI_I2S_GetFlagStatus(SPI_BUS_USED, SPI_I2S_FLAG_RXNE) == RESET);
  SPI_I2S_ReceiveData(SPI_BUS_USED);
}

void spi_bulk_write(uint32_t size) {
  /* Prepare alignment */
  if(size >= (USB_Rx_len - USB_Rx_ptr_out)) {
    size -= (USB_Rx_len - USB_Rx_ptr_out);
    while(USB_Rx_ptr_out != USB_Rx_len) spi_putc(usb_getc());
  }
  /* else: size << VCP_DATA_SIZE, no bulk transfer */

  /* Do bulk transfer */
  while(size != 0) {
    usb_getp();

    if(USB_Rx_len < VCP_DATA_SIZE) {
      /* Host is not feeding fast enough / finish the left-over bytes */
      size -= USB_Rx_len;
      while(USB_Rx_ptr_out != USB_Rx_len) spi_putc(usb_getc());
    }
    else {
      size -= VCP_DATA_SIZE;
      /* DMA Engine must be configured for EVERY transfer */
      dma_conf_spiwrite();
      dma_commit();
    }
  }
}

void spi_bulk_read(uint32_t size) {
  /* Flush buffer and make room for DMA */
	 static int i;
	
  if(USB_Tx_ptr_in != 0) usb_putp();

 

  /* Do bulk transfer */
  while(size >= VCP_DATA_SIZE) {
    /* DMA Engine must be configured for EVERY transfer */
    dma_conf_spiread();
    dma_commit();

    USB_Tx_ptr_in = VCP_DATA_SIZE;
    usb_putp();
    size -= VCP_DATA_SIZE;
  }

  /* Finish the left-over bytes */
  if(size != 0) {
    for(i = 0; i < size; i ++) {
      SPI_I2S_SendData(SPI_BUS_USED, 0);
      while(SPI_I2S_GetFlagStatus(SPI_BUS_USED, SPI_I2S_FLAG_RXNE) == RESET);
      USB_Tx_Buf[i] = SPI_I2S_ReceiveData(SPI_BUS_USED);
    }
    USB_Tx_ptr_in = size;
  }
}























