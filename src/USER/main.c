#include "led.h"
#include "delay.h"
#include "sys.h"
#include "usart.h"
#include "key.h"
#include "spi.h"
#include "flash.h"


#include "usb_lib.h"
#include "hw_config.h"
#include "usb_pwr.h"

   	
//要写入到W25Q64的字符串数组
const u8 TEXT_Buffer[]={"MiniSTM32 SPI TEST"};
#define SIZE sizeof(TEXT_Buffer)	 
 int main(void)
 { 
	u8 key;
	u16 i=0;
	u8 datatemp[SIZE];
	u32 FLASH_SIZE;
	delay_init();	    	 //延时函数初始化	  
	uart_init(9600);	 	//串口初始化为9600
	LED_Init();		  		//初始化与LED连接的硬件接口
	 
	delay_ms(1800);
	USB_Port_Set(0); 	
	delay_ms(700);
	USB_Port_Set(1);	
 	Set_USBClock();   
 	USB_Interrupts_Config();    
 	USB_Init();	 	 
	 
	KEY_Init();				//按键初始化		 	
	SPI_Flash_Init();  		//SPI FLASH 初始化 	 
 	
	while(SPI_Flash_ReadID()!=W25Q64)							//检测不到W25Q64
	{
		delay_ms(500);
		delay_ms(500);
		LED0=!LED0;//DS0闪烁
	}

	FLASH_SIZE=4*1024*1024;	//FLASH 大小为4M字节
	while(1)
	{
		key=KEY_Scan(0);
		if(key==WKUP_PRES)	//WK_UP 按下,写入W25Q64
		{
			
			SPI_Flash_Write((u8*)TEXT_Buffer,FLASH_SIZE-100,SIZE);		//从倒数第100个地址处开始,写入SIZE长度的数据
//提示传送完成
		}
		if(key==KEY0_PRES)	//KEY0 按下,读取字符串并显示
		{

			SPI_Flash_Read(datatemp,FLASH_SIZE-100,SIZE);				//从倒数第100个地址处开始,读出SIZE个字节
//提示传送完成
				//显示读到的字符串
		}
		i++;
		delay_ms(10);
		if(i==20)
		{
			LED0=!LED0;//提示系统正在运行	
			i=0;
		}		   
	}
}

