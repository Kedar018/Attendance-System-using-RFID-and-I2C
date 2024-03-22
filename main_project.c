#include <LPC21xx.H>
#include"header.h"
main(){
//s8 arr[13];
u8 h,m,s,d,m1,y;
int f;
uart0_init(9600);
//uart1_init(9600);
i2c_init();
lcd_init();	
config_vic();				
en_uart0_interrupt();
//uart0_tx_string("DS1307 RTC\r\n");
/*
i2c_bytes_write_frame(0xD0,0x0,0x00);
i2c_bytes_write_frame(0xD0,0x1,0x30);
i2c_bytes_write_frame(0xD0,0x2,0x08|0x60);
i2c_bytes_write_frame(0xD0,0x4,0x18);
i2c_bytes_write_frame(0xD0,0x5,0x02);
i2c_bytes_write_frame(0xD0,0x6,0x24);
*/ 
while(1){
lcd_cmd(0xc0);
lcd_string("scan the card");
lcd_cmd(0x80);
h=i2c_byte_read_frame(0xD0,0x2);
m=i2c_byte_read_frame(0xD0,0x1);						   
s=i2c_byte_read_frame(0xD0,0x0);
d=i2c_byte_read_frame(0xD0,0x4);
m1=i2c_byte_read_frame(0xD0,0x5);
y=i2c_byte_read_frame(0xD0,0x6);
 f=((h>>5)&1);
h=(h&0x1F); 
lcd_data((h/0x10)+48);
lcd_data((h%0x10)+48);
lcd_data(':');

lcd_data((m/0x10)+48);
lcd_data((m%0x10)+48);
lcd_data(':');

lcd_data((s/0x10)+48);								    
lcd_data((s%0x10)+48);
lcd_data(' ');
if(f==1)
       lcd_string("PM");
else if(f==0)
        lcd_string("AM");   
/*lcd_data((d/0x10)+48);
lcd_data((d%0x10)+48);
lcd_data('-');

//lcd_data((m1/0x10)+48);
lcd_data((m1%0x10)+48);
lcd_data('-');

lcd_data((y/0x10)+48);
lcd_data((y%0x10)+48);
*/
delay_sec(1);
while(i==12)
{
temp[i]=' ';
temp[++i]=((h/0x10)+48);
temp[++i]=((h%0x10)+48);
temp[++i]=(':');
temp[++i]=((m/0x10)+48);
temp[++i]=((m%0x10)+48);
temp[++i]=(':');
temp[++i]=((s/0x10)+48);
temp[++i]=((s%0x10)+48);
if(f==1)
{
temp[++i]='p';
temp[++i]='M';
}
else if(f==0)
{
temp[++i]='A';
temp[++i]='M';
}
temp[++i]=(' ');
temp[++i]=((d/0x10)+48);
temp[++i]=((d%0x10)+48);
temp[++i]=('-');
temp[++i]=((m1%0x10)+48);
temp[++i]=('-');
temp[++i]=((y/0x10)+48);
temp[++i]=((y%0x10)+48);
temp[++i]='\0';
temp[++i]='\r';
temp[++i]='\n';
lcd_cmd(0x80);
lcd_string("card scaned successfully");
//uart0_tx_string("card scaned successfully\r\n");
uart0_tx_string(temp);
i=0;

}
}
}

