//head files
#include <iom16v.h>
#include <macros.h>
 
#define LED PORTA
//delay functions us/ms
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
//detect if the key is pressed
void key_scan(void)
{
		 delay_ms(10);
		 if((PINB&0x1F)!=0x1F)//press one switch PINB will not be 0x1F only more
    	  {
    	   	  delay_ms(20);
        	  if((PINB&0x1F)!=0x1F)
        	  { 
			    switch(PINB)
				{
				    case 0X1E:LED = 0XFE;break;//S1 PINB0x0001
					case 0X1D:LED = 0XFD;break;//S2 
					case 0X1B:LED = 0XFB;break;//S3
					case 0X17:LED = 0XF7;break;//S4
					case 0X0F:LED = 0XEF;break;//S5
					default:LED = 0X00;
				}				
				delay_ms(1);
			  }
          }
		  
		  else if((PIND&0x0C)!=0x0C)
    	  {
    	   	  delay_ms(20);
        	  if((PIND&0x0C)!=0x0C)
        	  { 
			    switch(PIND)
				{
				    case 0X08:LED = 0XDF;break;//S6
					case 0X04:LED = 0XBF;break;//S7
					default:LED = 0X00;
				}				
				delay_ms(1);
			  }
          }
		  
}

void port_int(void)
{
     DDRD  = 0XF3;//0x1111 0011
	 PORTD = 0X0C;//high
     DDRB  = 0XE0;//0x1110 0000
	 PORTB = 0X1F;//0x0001 1111
	 DDRA  = 0x00;
     PORTA = 0xFF; 
}

void delay(void)
{
 	 int i;
  	 for(i=0;i<200;i++);
}

void main(void)
{
	 port_int();
	 while(1)
	 {
	    key_scan(); 
	 }
}