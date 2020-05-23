//transmit data from 0
#include<iom16v.h>
#include<macros.h>
#define fosc 8000000 	//8kHZ
#define baud 9600		//baud rate
void uart0_init(void)
{
 UCSRB = 0x00; 
 UCSRA = 1<<U2X;//double speed
 UCSRC =(1<<UCSZ1)|(1<<UCSZ0);//8 bit data. no parity. 1 stop bit.
 UBRRL=fosc/(8*baud)-1;
 UBRRH=(fosc/(8*baud)-1)>>8;
 UCSRB =(1<<RXEN)|(1<<TXEN);//this program will only use TXEN
}
void putnum0(unsigned int c)
	{	 
       while ((UCSRA&0x20)==0x00);//wait for RXC to be 1
	   UDR=c;//store the value into UD
	}	
unsigned char getchar0(void)
  	{
	   while((UCSRA&0x80)==0x00);//when UDRE=1 break out the cycle
       return UDR;//receive data from UDR
	}	
void main(void)
{
 unsigned char i;
 	uart0_init();
 	for(;i<8;++i)
	{  
	   putnum0(i);
	}
}	
		
