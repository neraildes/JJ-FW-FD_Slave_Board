
#include "isr.h"
#include "global.h"
#include "usart.h"


/*----------------------------------------------------------------------------*/
volatile unsigned char usart_buffer[USART_BUFFER_SIZE];
volatile int count;
volatile unsigned char *pointer;

volatile unsigned int  tempodecorrido;
volatile unsigned int  milisegundo;
volatile unsigned char segundo;
volatile unsigned char minuto;
volatile unsigned char hora;
 
volatile unsigned int Delay_Led_Tmr0   ; //AZUL
volatile unsigned int Delay_Led_Usart  ; //VERMELHO
volatile unsigned int Delay_Led_Memory ; //Verde

#ifdef NTC_BOARD
extern volatile char TempoCNT_0;
extern volatile char TempoCNT_1;
#endif



void __interrupt() isr(void)
{
unsigned int  tempo;
    
    
    if(INTCONbits.INTF)
      {  
      INTCONbits.INTF=0;  
      }
    
    if(PIR1bits.TMR1IF)
                {
                TMR1H=0xB1;
                TMR1L=0xF3;
                PIR1bits.TMR1IF=0;
    
                if(flag_main_loop_WDT==1)
                  {
                  flag_main_loop_WDT=0;  
                  asm("CLRWDT");
                  }
                
                Delay_Led_Tmr0+=10;
                if(Delay_Led_Tmr0>=1000) 
                    Delay_Led_Tmr0=0;
                if(Delay_Led_Tmr0<500)                   
                   flag_led_tmr0=1;
                else
                   flag_led_tmr0=0;
                 
                
                if(Delay_Led_Usart){
                   Delay_Led_Usart-=10;
                   flag_led_usart=1;
                   }
                else{
                   flag_led_usart=0;
                   }        
                
                if(Delay_Led_Memory){
                   Delay_Led_Memory-=10;
                   flag_led_memory=1;//fix reativar
                   }
                else{
                   flag_led_memory=0; //fix reativar 
                   }        
                
                
                tempodecorrido+=10;
                milisegundo+=10;
                if(milisegundo==1000){
                   milisegundo=0;
                   segundo++;

                   #ifdef NTC_BOARD 
                   if(TempoCNT_0>0) TempoCNT_0--;
                   if(TempoCNT_1>0) TempoCNT_1--;                      
                   #endif
    
                   if(segundo==60){
                      segundo=0;                      
                      minuto++;
                      if(minuto==60){
                         minuto=0;
                         hora++;
                         if(hora==24){
                            hora=0;
                         }
                      }
                   }
                }
            }        

    

    if(PIR1bits.RCIF)
        { 
        //flag_led_memory=1; //FIX
        
        flag_usart_rx=TRUE;         
        tempo=RX_MAX_WAIT_TIME;        
        count=0;
        pointer=usart_buffer;
        while(tempo)
        {    
            if (PIR1bits.RCIF) 
            {   //flag_led_memory=0;  //FIX                          
                (*pointer)=RCREG;
                //flag_led_memory=1;  //FIX
                if(count<USART_BUFFER_SIZE-1) 
                   { 
                   count++; 
                   pointer++;                
                   }
                tempo=RX_MAX_WAIT_TIME;                   
            }
            else    
            {
                tempo--;    
            }  
            //__delay_us(100);
        }           
        (*pointer)=0;
        
        if(RCSTAbits.OERR){
           CREN=0;
           CREN=1;
           tempo=RCREG;
           tempo=RCREG;
           tempo=RCREG;           
           usart_buffer[0]=0;
           flag_usart_error=TRUE;   // Fix  - Utilizar 1 bit.           
        }
        //flag_led_memory=0;  //fix
    }
}
