//transmit data from 0
#include<iom16v.h>
#include<macros.h>
#define fosc 8000000 	//8kHZ
#define baud 9600		//baud rate
int result=0;
char rdata;
void uart0_init(void)
{
 UCSRB = 0x00; 
 UCSRA = 1<<U2X;//double speed
 UCSRC =(1<<UCSZ1)|(1<<UCSZ0);//8 bit data. no parity. 1 stop bit.
 UBRRL=fosc/(8*baud)-1;
 UBRRH=(fosc/(8*baud)-1)>>8;
 UCSRB =(1<<RXEN)|(1<<TXEN)|(1<<RXCIE);//this program will only use TXEN
}
void putchar0(unsigned char c)
	{	 
       while ((UCSRA&0x20)==0x00);//wait for RXC to be 1
	   UDR=c;//store the value into UDR
	}
void putc(unsigned char str[20]){
	int i=0;
	while(str[i]!='\0'){
		putchar0(str[i]);
		i++;
	}
}	
void rei_char0(void)
  	{  
	   UCSRB=~(1<<RXCIE);
       rdata=UDR;//receive data from UDR
       UCSRB=1<<RXCIE;
       result=1;
	}	
void main(void)
{
 	unsigned char i;
 	uart0_init();
 	while(1){
 		if(result)
 		{
 			putc("receive successfully!");
 			result=0;
		 }
	 }
}	


	
