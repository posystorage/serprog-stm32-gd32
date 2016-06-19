#include "led.h"
#include "delay.h"
#include "sys.h"
#include "usart.h"
#include "spi.h"
#include "serprog.h"

#include "usb_lib.h"
#include "hw_config.h"
#include "usb_pwr.h"

	
void serprog_handle_command(unsigned char command) {
  //led_off();

  static uint8_t   i;        /* Loop            */
  static uint8_t   l;        /* Length          */
  static uint32_t  slen;     /* SPIOP write len */
  static uint32_t  rlen;     /* SPIOP read len  */
  static uint32_t  freq_req;

  switch(command) {
    case S_CMD_NOP:
      usb_putc(S_ACK);
      break;
    case S_CMD_Q_IFACE:
      usb_putc(S_ACK);
      /* little endian multibyte value to complete to 16bit */
      usb_putc(S_IFACE_VERSION);
      usb_putc(0);
    break;
      case S_CMD_Q_CMDMAP:
      usb_putc(S_ACK);
      /* little endian */
      usb_putu32(S_CMD_MAP);
      for(i = 0; i < 32 - sizeof(uint32_t); i++) usb_putc(0);
      break;
    case S_CMD_Q_PGMNAME:
      usb_putc(S_ACK);
      l = 0;
      while(S_PGM_NAME[l]) {
        usb_putc(S_PGM_NAME[l]);
        l ++;
      }
      for(i = l; i < 16; i++) usb_putc(0);
      break;
    case S_CMD_Q_SERBUF:
      usb_putc(S_ACK);
      /* Pretend to be 64K (0xffff) */
      usb_putc(0xff);
      usb_putc(0xff);
      break;
    case S_CMD_Q_BUSTYPE:
      // TODO: LPC / FWH IO support via PP-Mode
      usb_putc(S_ACK);
      usb_putc(S_SUPPORTED_BUS);
      break;
    case S_CMD_Q_CHIPSIZE: break;
    case S_CMD_Q_OPBUF:
      // TODO: opbuf function 0
      break;
    case S_CMD_Q_WRNMAXLEN: break;
    case S_CMD_R_BYTE:     break;
    case S_CMD_R_NBYTES:   break;
    case S_CMD_O_INIT:     break;
    case S_CMD_O_WRITEB:
      // TODO: opbuf function 1
      break;
    case S_CMD_O_WRITEN:
      // TODO: opbuf function 2
      break;
    case S_CMD_O_DELAY:
      // TODO: opbuf function 3
      break;
    case S_CMD_O_EXEC:
      // TODO: opbuf function 4
      break;
    case S_CMD_SYNCNOP:
      usb_putc(S_NAK);
      usb_putc(S_ACK);
      break;
    case S_CMD_Q_RDNMAXLEN:
      // TODO
      break;
    case S_CMD_S_BUSTYPE:
      /* We do not have multiplexed bus interfaces,
       * so simply ack on supported types, no setup needed. */
      if((usb_getc() | S_SUPPORTED_BUS) == S_SUPPORTED_BUS) usb_putc(S_ACK);
      else usb_putc(S_NAK);
      break;
    case S_CMD_O_SPIOP:
      slen = usb_getu24();
      rlen = usb_getu24();

      select_chip();

      /* TODO: handle errors with S_NAK */
      if(slen) spi_bulk_write(slen);
      usb_putc(S_ACK);
      if(rlen) spi_bulk_read(rlen);

      unselect_chip();
      break;
    case S_CMD_S_SPI_FREQ:
      freq_req = usb_getu32();
      if(freq_req == 0) usb_putc(S_NAK);
      else {
        usb_putc(S_ACK);
        usb_putu32(spi_conf(freq_req));
      }
      break;
    default: break; // TODO: Debug malformed command
  }

  //led_on();
}


 int main(void)
 { 
	 u8 usbstatus=0;
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
	 	 	
	 SPI_IO_Init(); 
	 DMA_configuration();
	spi_conf(SPI_DEFAULT_SPEED);
	 LED0=0;
	while(1)
	{
		    /* Get command */
    serprog_handle_command(usb_getc());
    /* Flush output via USB */
    usb_sync();
		if(usbstatus!=bDeviceState)
		{
			usbstatus=bDeviceState;
			if(usbstatus==CONFIGURED)
			{
				LED1=0;
			}else
			{				
				LED1=1;
			}
		}
	}
}

