#include<iom16v.h>
#include<macros.h>
#define S_1      PORTD&=~BIT(7);PORTD|=BIT(6);PORTD|=BIT(5);PORTD|=BIT(4); 
#define S_2      PORTD|=BIT(7);PORTD&=~BIT(6);PORTD|=BIT(5);PORTD|=BIT(4);
#define S_3      PORTD|=BIT(7);PORTD|=BIT(6);PORTD&=~BIT(5);PORTD|=BIT(4);  
#define S_4      PORTD|=BIT(7);PORTD|=BIT(6);PORTD|=BIT(5);PORTD&=~BIT(4);
#define S_OFF    PORTD|=BIT(7);PORTD|=BIT(6);PORTD|=BIT(5);PORTD|=BIT(4);  
#define S_ON     PORTD&=~BIT(7);PORTD&=~BIT(6);PORTD&=~BIT(5);PORTD&=~BIT(4);
void port_init(void)
{
 DDRA=0x00;
 PORTA=0xFF;
 PORTC = 0xFF; //m103 output only
 DDRC  = 0xFF;
 PORTD = 0xFF;
 DDRD  = 0xFF;
 DDRB=0xE0;
 PORTB=0x1F;
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
unsigned int get_adc0(void)
{	 unsigned int lowb,highb,adr;
     DDRA&=~BIT(PA0);//ADC0 as input
     PORTA&=~BIT(PA0);//set port 0
	 ADMUX=0x40;//REFS01=1,enable AVCC,right adjusted,ADC0-MUX 00000            
	 ADCSRA|=BIT(ADEN);
	 ADCSRA|=BIT(ADSC);           
	 while(!(ADCSRA&(BIT(ADIF))));//if ADIF=1 break out the cycle
     ADCSRA&=~BIT(ADIF);//clear ADIF
     lowb = (unsigned int)ADCL;//low bit
     highb= (unsigned int)ADCH;//high bit
     adr=highb*256+lowb;
	 adr*=2.44;
     return adr;
}
unsigned int get_adc1(void)
{	 unsigned int lowb,highb,adr;
     DDRA&=~BIT(PA1);//ADC0 as input
     PORTA&=~BIT(PA1);//set port 0
	 ADMUX=0x41;//REFS01=1,enable AVCC,right adjusted,ADC0-MUX 00001           
	 ADCSRA|=BIT(ADEN);
	 ADCSRA|=BIT(ADSC);           
	 while(!(ADCSRA&(BIT(ADIF))));//if ADIF=1 break out the cycle
     ADCSRA&=~BIT(ADIF);//clear ADIF
     lowb = (unsigned int)ADCL;//low bit
     highb= (unsigned int)ADCH;//high bit
     adr=highb*256+lowb;
	 adr*=2.44;//2500/1023 
     return adr;
}

void main(void)
{
	unsigned int result;//most 1024
	unsigned int R[4];
	unsigned int i;
   while(1)
   {
   result=0; 
   port_init();
   switch(PINB){
   	case 0x1E:
			   	result=get_adc0();
			    for(i=0;i<4;i++){
			   		R[i]=result%10;
			   		result=result/10;
			    }	  
					  PORTB=0x1F;
					  while(PINB==0x1F)
			    	  {
					  S_OFF;		  
					  display(R[3]);S_1;		  
					  delay_us(500);
					  S_OFF;		  
					  display(R[2]);S_2;		  
					  delay_us(500);
					  S_OFF;		  
					  display(R[1]);S_3;		  
					  delay_us(500);
					  S_OFF;		  
					  display(R[0]);S_4;		  
					  delay_us(500);
					  }
					   break;
	case 0x1D:
				result=get_adc1();
			    for(i=0;i<4;i++){
			   		R[i]=result%10;
			   		result=result/10;
			    }	  
					  PORTB=0x1F;
					  while(PINB==0x1F)
			    	  {
			    	  S_OFF;		  
					  display(R[3]);S_1;		  
					  delay_us(500);
					  S_OFF;		  
					  display(R[2]);S_2;		  
					  delay_us(500);
					  S_OFF;		  
					  display(R[1]);S_3;		  
					  delay_us(500);
					  S_OFF;		  
					  display(R[0]);S_4;		  
					  delay_us(500);	
					 }	
					  break;
   }
}
}

