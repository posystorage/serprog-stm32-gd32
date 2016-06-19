#ifndef __SPI_H
#define __SPI_H
#include "sys.h"

void SPI_IO_Init(void);			 //³õÊ¼»¯SPI¿Ú

/* SPI */
#define SPI_BUS_USED         SPI1
#define SPI_ENGINE_RCC       RCC_APB2Periph_SPI1
#define SPI_DEFAULT_SPEED    9000000              /* Default SPI clock = 9MHz to support most chips.*/
#define SPI_DR_Base          (&(SPI_BUS_USED->DR))
#define SPI_TX_DMA_CH        DMA1_Channel3         /* SPI1 TX is only available on DMA1 CH3 */
#define SPI_TX_DMA_FLAG      DMA1_FLAG_TC3
#define SPI_RX_DMA_CH        DMA1_Channel2         /* SPI1 RX is only available on DMA1 CH2 */
#define SPI_RX_DMA_FLAG      DMA1_FLAG_TC2


#define select_chip() GPIO_ResetBits(GPIOA,GPIO_Pin_4)
#define unselect_chip() GPIO_SetBits(GPIOA,GPIO_Pin_4)

uint32_t spi_conf(uint32_t speed_hz);
void spi_bulk_write(uint32_t size);
void spi_bulk_read(uint32_t size);
void DMA_configuration(void);


		 
#endif

