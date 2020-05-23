
#include <iom16v.h>
#include <macros.h>
#define LED PORTA
unsigned char i = 0;
unsigned int LED_order[16] = {0XFE,0XFC,0XF8,0XF0,0XE0,0XC0,0X80,0x00,0x00,0X80,0XC0,0xE0,0XF0,0XF8,0XFC,0XFE};
void delay_us(unsigned int microsecond) //lighting in the order of 1-8,and turn off in the order of 8-1
{      
    do 
	{ 
        microsecond--; 
	}         
    while (microsecond>1); 
}
void delay_ms(unsigned int millisecond) 
{      
    while (millisecond--)
	{
        delay_us(999);
	}  
}
void port_init(void)
{
 PORTA = 0xFF;//only portA is used. 
 DDRA  = 0xFF;//portA is for led
}

void main(void)
{
    port_init();
    while(1)
    {
		for(i = 0;i < 16;i ++)
		{
		    LED = LED_order[i];
			delay_ms(300);//change the condition every 300ms
		}		
		i = 0;
    }
}
