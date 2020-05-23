#include<iom16v.h>
#include<macros.h>
#define S_1      PORTD&=~BIT(7);PORTD|=BIT(6);PORTD|=BIT(5);PORTD|=BIT(4); 
#define S_2      PORTD|=BIT(7);PORTD&=~BIT(6);PORTD|=BIT(5);PORTD|=BIT(4);
#define S_3      PORTD|=BIT(7);PORTD|=BIT(6);PORTD&=~BIT(5);PORTD|=BIT(4);  
#define S_4      PORTD|=BIT(7);PORTD|=BIT(6);PORTD|=BIT(5);PORTD&=~BIT(4);
#define S_OFF    PORTD|=BIT(7);PORTD|=BIT(6);PORTD|=BIT(5);PORTD|=BIT(4);  
#define S_ON     PORTD&=~BIT(7);PORTD&=~BIT(6);PORTD&=~BIT(5);PORTD&=~BIT(4);
#include <iom16v.h>
#include <macros.h>
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
 PORTA = 0xFF;//LED
 DDRA  = 0xFF;
 DDRB=0xFB;//INT0 as input
 PORTB=0x04;//enable pull-up resistor
 DDRD=0xF3;//INT1(PD2),INT2(PD3) 1 as input 
 PORTD=0xFC;//enable pull-up resistor
 PORTC = 0XFF; //m103 output only
 DDRC  = 0XFF;
}
void init_devices(void)
{
 port_init();
 MCUCR =0x01;//0000 0001.INT1 low level,INT0 logic change
 MCUCSR=0x40;//0100 0000.INT2 falling edge
 GICR  = 0xE0;//enable INT1,0,2 
 asm("SEI"); //enable interrupts
}
unsigned char tab[16] = {
		 	  		  	 0x3f,0x06,0x5b,0x4f,
						 0x66,0x6d,0x7d,0x07,
						 0x7f,0x6f,0x77,0x7c,
						 0x39,0x5e,0x79,0x71
						 };
void display(unsigned char n)
{
	PORTC = tab[n];
}

#pragma interrupt_handler INT1_isr:3
void INT1_isr(void){
	PORTA=0x00;
	delay_ms(50);
	PORTA=0xFF;
	display(7);S_1;		  
	delay_us(3000);
}

#pragma interrupt_handler INT0_isr:2
void INT0_isr(void){
	PORTA=0x00;
	delay_ms(50);
	PORTA=0xFF;
	S_OFF;		  
	display(6);S_1;		  
	delay_us(3000);
}
#pragma interrupt_handler INT2_isr:19
void INT2_isr(void){
	PORTA=0x00;
	delay_ms(50);
	PORTA=0xFF;
	S_OFF;		  
	display(3);S_1;		  
	delay_us(3000);
}

void main(void)
{
   init_devices();
   PORTA = 0XFF;
   while(1)
   {
   }
}
