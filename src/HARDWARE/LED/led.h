#ifndef __LED_H
#define __LED_H	 
#include "sys.h"

#define LED0 PBout(7)	
#define LED1 PBout(8)	
#define LED2 PBout(4)	

//红色是PC7
//音频蓝色PC8
//蓝牙蓝色B4
void LED_Init(void);//初始化

		 				    
#endif
