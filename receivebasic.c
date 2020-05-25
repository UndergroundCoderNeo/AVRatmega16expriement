//transmit data from 0
#include<iom16v.h>
#include<macros.h>
#include "lcd_1602.h"
#define fosc 8000000 	//8kHZ
#define baud 9600		//baud rate
unsigned char rdata;
unsigned int result;
void delay_us(uint n)   //8*0.125=1us
{
    int i,j;
	for(j=0;j<8;j++)
	{
	  for (i=0;i<n;i++) 
	  NOP();
	}  
}

//ms
void delay_ms(uint i)
{
    while(i--)
    {                          
	    uint j;                
        for(j=1;j<=1332;j++)   
			   ;               
    } 
}
//Control LCD write timing
void LCD_en_write(void)        /*The en terminal generates a high-level pulse to control the 
								writing sequence of LCD*/
  {
    E_ON;//EN_SET;
    delay_us(3);
    E_OFF;//EN_CLR;
	delay_us(3);
  }
//Write instruction function
void Write_Instruction(uchar command)
{
  RS_OFF;//RS_CLR;
  RW_OFF;//RW_CLR;
  E_ON;//EN_SET;
  PORTC = command;
  LCD_en_write();//Write instruction data
}

//write data
void Write_Data(uchar Wdata)
{
  RS_ON;//RS_SET;
  RW_OFF;//RW_CLR;
  E_ON;//EN_SET;
  PORTC = Wdata;
  LCD_en_write();//write data
} 
//clear screen
void LCD_clear(void)
{
  Write_Instruction(0x01);
  delay_ms(2);
}

//Character display initial address setting
void LCD_SET_XY(uchar X,uchar Y)
{
    uchar address;
    if(Y==0)
       address=0x80+X;/*Y = 0, indicating that it is displayed in the 
	                    first line, and the address cardinality is 0x80*/ 
    else 
       address=0xc0+X;/*When y is not 0, it is displayed in the second row of 
	                   the table, and the address base is 0xc0*/ 
    Write_Instruction(address);//Write instruction, set display initial address
}

//Starting at line x, column y, the string pointed to by pointer * s
void LCD_write_str(uchar X,uchar Y,uchar *s)
{
  LCD_SET_XY(X,Y);//Set initial character display address
  while(*s)//Write the display character one by one until the last character "/ 0"
  {
    Write_Data(*s);//Write the current character and display
	s++;//Address pointer plus 1 points to the next character to be written
  }
}

//Display the single character corresponding to wdata at the beginning of Y column in line x
void LCD_write_char(uchar X,uchar Y,uchar Wdata)
{
  LCD_SET_XY(X,Y);//Address
  Write_Data(Wdata);//Write the current character and display
}
//Display initialization function
void LCD_init(void) 
{
	DDRC = 0xFF;						    //I / O port direction setting
	DDRD|=(1<<PD5)|(1<<PD6)|(1<<PD7);
	delay_ms(15);                           /*Power on delay for a period of time to 
											make the power supply stable*/ 
	Write_Instruction(0x38);				//8bit interface,2line,5*7dots
	delay_ms(5);
	Write_Instruction(0x38);	
	delay_ms(5);
	Write_Instruction(0x38);	
	
	Write_Instruction(0x08);	//Turn off the display, do not 
								//show the cursor, and the cursor does not flash
	Write_Instruction(0x01);	//clear screen
	delay_ms(5);
	
	Write_Instruction(0x04);	//
	delay_ms(5);
	Write_Instruction(0x0C);	//disable lighting
}
void port_init(void)
{
  PORTC=0XFF;
  DDRC =0XFF;
  DDRB=0XE0;
  PORTB=0X1F;
  MCUCR = 0x00;
  GICR  = 0x00;
  TIMSK = 0x00; 
  SEI();
 
  
  PORTD |= BIT(5)|BIT(6)|BIT(7);   
  DDRD |= BIT(5)|BIT(6)|BIT(7); 
}

void init_devices(void)
{
 //stop errant interrupts until set up
 CLI(); //disable all interrupts
 port_init(); 
 SEI(); //re-enable interrupts
 //all peripherals are now initialized
}
void uart0_init(void)
{
 UCSRA = 1<<U2X;//double speed
 UCSRC =(1<<UCSZ1)|(1<<UCSZ0);//8 bit data. no parity. 1 stop bit.
 UBRRL=fosc/(8*baud)-1;
 UBRRH=(fosc/(8*baud)-1)>>8;
 UCSRB =(1<<RXEN)|(1<<TXEN);
 DDRD|=0x02;
}
void putc(unsigned char c)
	{	 
       while ((UCSRA&0x20)==0x00);//wait for UDRE to be 1
	   UDR=c;//store the value into UDR
	   while(!(UCSRA&(BIT(TXC))));
	   UCSRA|=BIT(TXC);
	}	

unsigned int rei_char(void)
  	{  
	 if((UCSRA&(1<<RXC))!=0){
	  rdata=UDR;//wait for RXC to be 1
	  return 1;
	 }
	  else return 0;
	}	
void main(void)
{	
	unsigned char i='T';
 	uart0_init();
 	DDRB=0x00;
 	PORTB=0xFF;
	while(1){
	if(PINB!=0xFF)
		putc(i);
	if(rei_char()){
   init_devices();
   LCD_init();
   LCD_clear();
   delay_ms(500); 
    LCD_write_str(0,0,"successfully receive");
	delay_ms(300);  
	}
	}
}


	
