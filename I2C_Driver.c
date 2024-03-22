#include <LPC21xx.H>
#include"header.h"
#define si ((I2CONSET>>3)&1)
void uart0_tx_string(char *ptr);
void i2c_init(void){
  PINSEL0|=0x50;
  I2SCLH=I2SCLL=75;
  I2CONSET=(1<<6);
}

void i2c_bytes_write_frame(u8 sa, u8 mr,u8 data){
    I2CONSET=(1<<5);
    I2CONCLR=(1<<3); 
    while(si==0);
I2CONCLR=(1<<5);
if(I2STAT!=0x08){
     uart0_tx_string("starting condition\r\n");
	 goto exit;
}
I2DAT=sa;
I2CONCLR=(1<<3);
while(si==0);
if(I2STAT==0x20){
     uart0_tx_string("slave address problem +v err\r\n");
	 goto exit;
}
I2DAT=mr;
I2CONCLR=(1<<3);
while(si==0);
if(I2STAT==0x30){
   uart0_tx_string("memory add err \r\n");
   goto exit;
}
   I2DAT=data;
   I2CONCLR=(1<<3);
   while(si==0);
   if(I2STAT==0x30){
   uart0_tx_string("data \r\n");
   }
   exit:
   I2CONSET=(1<<4);
   I2CONCLR=(1<<3);
}

u8 i2c_byte_read_frame(u8 sa,u8 mr){
  u8 temp;
I2CONSET=(1<<5);
I2CONCLR=(1<<3);
while(si==0);
I2CONCLR=(1<<5);
if(I2STAT!=0x08){
     uart0_tx_string("ERR:starting condition\r\n");
	 goto exit;
}
I2DAT=sa;
I2CONCLR=(1<<3);
while(si==0);
if(I2STAT==0x20){
     uart0_tx_string("ERR: slave address+W \r\n");
	 goto exit;
}
    I2DAT=mr;
    I2CONCLR=(1<<3);
   while(si==0);
   if(I2STAT==0x30){
   uart0_tx_string("ERR:memory add\r\n");
   goto exit;
   }
   I2CONSET=(1<<5);
   I2CONCLR=(1<<3);
   while(si==0);
   I2CONCLR=(1<<5);
   if(I2STAT!=0x10){
        uart0_tx_string("ERR: start condition \r\n");
		goto exit;
	}
	 I2DAT=sa|1;
	 I2CONCLR=(1<<3);
	 while(si==0);
	 if(I2STAT==0x48){
	     uart0_tx_string("ERR: SA+R \r\n");
		 goto exit;
	 }
	 I2CONCLR=(1<<3);
	 while(si==0);
	 temp=I2DAT;
	 exit:
	 I2CONSET=(1<<4);
	 I2CONCLR=(1<<3);

	  return temp;
}
