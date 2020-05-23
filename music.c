
#include <iom16v.h>
#include <macros.h>

//¼ÆÊ±Öµ=65536-system frequency/N/tune frequency/2   
//      tune value  frequency Hz 
#define LL6	 38263   //220	
#define LL7  41245   //247   	
#define L1	 42635   //262    
#define L2	 45128   //294	
#define L3	 47354   //330   	
#define L4	 48344   //349	
#define L5	 50230   //392	
#define L6	 51900   //440	
#define L7	 53390   //494	
#define N1	 54064   //523	
#define N2	 55315   //587	
#define N3	 56431   //659	
#define N4	 56940   //698	
#define N5	 57883   //784	
#define N6	 58718   //880	
#define N7	 59463   //988	
#define H1	 59805   //1047	 
#define H2	 60430   //1175	
#define H3	 60987   //1319	
#define H4	 61241   //1397	
#define H5	 61709   //1568	
#define H6	 62127   //1760	
#define H7	 62500   //1976	
#define HH1	 62669   //2093
#define O    0       //stop bit.
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
unsigned int song[]=
{
N4,8,N4,8,N6,8,N6,8,N5,8,N5,8,H1,8,H1,8,H2,8,H2,8,H2,8,H2,8,N5,4,N4,4,
 N3,4,N1,4,N2,4,0,4,N2,4,N6,4,/*1*/
 N5,4,0,4,N4,4,0,4,N3,4,0,4,N3,8,N5,4,0,4,N4,4,N4,4,N2,4,0,4,N2,4,H4,4
 ,H3,4,H4,4,H3,4,H4,4 ,N2,4,0,4,N2,4,H4,4,H3,4,H4,4,H3,4,H4,4,N2,4,0,4
 ,N2,4,N6,4,/*2*/
 N5,4,0,4,N4,4,0,4,N3,4,0,4,N3,8,N5,4,0,4,N4,4,N4,4,N2,4,0,4,N2,4,H4,4
 ,H3,4,H4,4,H3,4,H4,4 ,N2,4,0,4,N2,4,H4,4,H3,4,H4,4,H3,4,H4,4,N4,16,
 /*3*/
 N5,8,N5,8,H1,8,H1,8,H2,8,H2,8,H2,8,H2,8,N5,4,N4,4,
 N3,4,N1,4,N2,4,0,4,N2,4,N6,4,N5,4,0,4,N4,4,0,4,/*4*/
 
 N3,4,0,4,N3,4,0,4,N3,4,N3,4,N5,4,0,4,N4,4,N3,4,N2,4,0,4,N2,4,H3,4,H4,4,
 H3,4,H4,4,N2,4,0,4,N2,4,N6,4,N5,4,0,4,N4,4,0,4,/*5*/
 
 N3,4,0,4,N3,4,0,4,N3,4,N3,4,N5,4,0,4,N4,4,N3,4,N2,4,0,4,N2,4,H3,4,H4,4,
 H3,4,H4,4,0,32,/*6*/
 0xFF
};

unsigned int beat;
void Music(unsigned int *pMusic) 
{ 
  while(*pMusic!=0xFF) 			//pointer to song 
    { 
      TIMSK=0x04;				//Timer1 overflow enable      
	  beat=*pMusic;             //get the tune
	  TCNT1H=beat/256;			 
	  TCNT1L=beat%256;          //transfer the beat into     
      pMusic++;					//get the time for tune
      delay_ms((*pMusic)*52);   
      TIMSK=0x00;			   	//disable timer1             
      pMusic++;					//get another beat
    } 
    delay_ms(1000);    			//end of music             
}

//timer1 interrupt 
#pragma interrupt_handler timer1_ovf_isr:iv_TIM1_OVF
void timer1_ovf_isr(void)
{
 if(beat)
 {
  PORTA ^= BIT(0);     //PA0 as output
  TCNT1H=beat/256;			
  TCNT1L=beat%256;     //transfer the beat into 
 }
}
void timer_init(void){
		DDRA  = 0xff;      //PORTA is output
 		PORTA = 0x00;      
 		TCCR1B = 0x01;     //Timer1 N=1
 		SEI();             //enable globle interrupt
}
void main(void)
{
 timer_init();
 while(1)
 {
  Music(song);
 }
}

