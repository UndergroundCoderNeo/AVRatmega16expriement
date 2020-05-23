
#include <iom16v.h>
#include <macros.h>
#define LED PORTA

unsigned char i = 0;

void delay_us(unsigned int microsecond) 
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
 PORTA = 0xFF; //commom positive write 1 to shut down all LED
 DDRA  = 0xFF;//set A to be output
}

void main(void)
{	
	port_init();
	while(1)
	{		for(i=0;i<3;++i){//left 4
			PORTA=0X0F;
			delay_ms(500);
			PORTA=0XFF;
			delay_ms(500);
			}
			
			for(i=0;i<3;++i){//right 4
			PORTA=0XF0;
			delay_ms(500);
			PORTA=0XFF;
			delay_ms(500);
			}
			for(i=0;i<3;++i){//middle 4
			PORTA=0XC3;
			delay_ms(500);
			PORTA=0XFF;
			delay_ms(500);
			}
	}
	
}

