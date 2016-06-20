#ifndef __HW_CONFIG_H
#define __HW_CONFIG_H
#include "platform_config.h"
#include "usb_type.h" 

  
 
#define USB_USART_TXFIFO_SIZE   1024	//USB虚拟串口发送FIFO大小		
#define USB_USART_REC_LEN	 	200		//USB串口接收缓冲区最大字节数

#define BULK_MAX_PACKET_SIZE 0x00000040            /* Max packet size for FullSpeed bulk transfer */
#define VCP_DATA_SIZE        0x40                  /* Should be the same as BULK_MAX_PACKET_SIZE */

#define USB_SC_PORT						GPIOC							//D+上拉电阻
#define USB_SC_PIN						GPIO_Pin_9
#define USB_SC_CLOCK					RCC_APB2Periph_GPIOC

//定义一个USB USART FIFO结构体
typedef struct  
{										    
	u8  buffer[USB_USART_TXFIFO_SIZE];	//buffer
	vu16 writeptr;						//写指针
	vu16 readptr;						//读指针
}_usb_usart_fifo; 
extern _usb_usart_fifo uu_txfifo;		//USB串口发送FIFO

extern u8  USB_USART_RX_BUF[USB_USART_REC_LEN]; //接收缓冲,最大USB_USART_REC_LEN个字节.末字节为换行符 
extern u16 USB_USART_RX_STA;   					//接收状态标记	
 
//USB通用代码函数声明
void Set_USBClock(void);
void Enter_LowPowerMode(void);
void Leave_LowPowerMode(void);
void USB_Interrupts_Config(void);
void USB_Cable_Config (FunctionalState NewState);
void USB_Port_Set(u8 enable);
void IntToUnicode (u32 value,u8 *pbuf,u8 len);
void Get_SerialNum(void);




extern void     usb_putc(char data);
extern char     usb_getc(void);
extern uint32_t usb_getu24(void);
extern uint32_t usb_getu32(void);
extern void     usb_putu32(uint32_t ww);
extern void     usb_sync(void);


#endif  




























