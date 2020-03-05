/*------------------------------------------------------------------------------
 * Rotinas para comunicação serial                                           |
 -----------------------------------------------------------------------------*/

#include "global.h"
#include "protocolo.h"
#include "usart.h"


volatile unsigned char usart_buffer[USART_BUFFER_SIZE]; 

extern volatile unsigned int tmr_led_usart;
extern volatile unsigned int Delay_Led_Usart; //VERMELHO



/*----------------------------------------------------------------------------*/
void USART_init(unsigned long baudrate)
     {
     unsigned char erro; 
    
     #ifdef XTAL_8MHZ
        if(baudrate==19200)
          {
          //................................USART.....................................
          RCSTAbits.SPEN = 1;
          RCSTAbits.CREN = 1;

          TXSTAbits.BRGH  = 1; //Trasmitter hight speeed                             |
          TXSTAbits.SYNC  = 0; //Async mode                                          |
          TXSTAbits.TX9   = 0; //without 9o bit                                      |
          SPBRG           =12; //BaudRate                                            |
          INTCONbits.PEIE = 1; //Periferical Interrupt Enable                        |
          PIE1bits.RCIE   = 1; //USART receiver RX                                   |
          }
        else if(baudrate==115200)
                {
                //................................USART.....................................
                SPBRG             =16; //BaudRate                                            |
                BAUDCTLbits.BRG16 = 1;  //                                                   |
                TXSTAbits.BRGH    = 1; //Trasmitter hight speeed                             |
                RCSTAbits.SPEN    = 1;
                TXSTAbits.SYNC    = 0; //Async mode                                          |
                TXSTAbits.TX9     = 0; //without 9o bit                                      |               
                RCSTAbits.CREN    = 1;
                INTCONbits.PEIE   = 1; //Periferical Interrupt Enable                        |
                PIE1bits.RCIE     = 1; //USART receiver RX                                   |                
                } 
        
     #endif
     
     #ifdef XTAL_20MHZ
        if(baudrate==19200)
          {
          //................................USART.....................................
          RCSTAbits.SPEN = 1;
          RCSTAbits.CREN = 1;

          TXSTAbits.BRGH  = 1; //Trasmitter hight speeed                             |
          TXSTAbits.SYNC  = 0; //Async mode                                          |
          TXSTAbits.TX9   = 0; //without 9o bit                                      |
          SPBRG           =64; //BaudRate                                            |
          INTCONbits.PEIE = 1; //Periferical Interrupt Enable                        |
          PIE1bits.RCIE   = 1; //USART receiver RX                                   |
          }
     #endif
    
     TRISC6= 1;
     TRISC7= 1;
    
     CREN=0;
     CREN=1;
     for(unsigned char i=0;i<15;i++) erro=RCREG;        
     }


void USART_to_Protocol(t_usart_protocol *usart_protocol){
     usart_protocol->header=((unsigned int)usart_buffer[0]<<8)+usart_buffer[1];
     usart_protocol->origem=usart_buffer[2];
     usart_protocol->destino=usart_buffer[3];     
     usart_protocol->command=usart_buffer[4];
     usart_protocol->size=usart_buffer[5];
     for(int i=0;i<usart_protocol->size;i++)
        usart_protocol->value[i]=(unsigned char) usart_buffer[i+6];    
}



void USART_putc(char value)
{     
    Delay_Led_Usart = DEFAULT_LEDS;    
    while(!TXSTAbits.TRMT) continue;//Registrador vazio    
    TXREG=value;           
    TXSTAbits.TXEN  = 1;
    
}


void USART_put_buffer(char *vetor, char size){       
    char loop=0;
    char *ptr;
    ptr = vetor;
    
    while(loop<size)
    {
        USART_putc(*ptr);        
        ptr++;
        loop++;
    }
}


#ifndef NTC_BOARD
unsigned char USART_input_buffer(void){
    unsigned int  time=0;
    unsigned char result=FALSE;
    
    while(time<RX_MAX_WAIT_TIME){ 
        if(flag_usart_rx){
           flag_usart_rx=FALSE;
           result = TRUE;
           break;
           }
        time++;
        __delay_ms(1);        
    }
    return result;
}
#endif







void USART_put_int(int value){ 
     char *dado;   
     dado=(char*)&value;
     USART_putc(*(dado+1));     
     USART_putc(*dado);
}







void USART_put_long(unsigned long value){
     USART_putc(value>>24);
     USART_putc(value>>16);
     USART_putc(value>>8);
     USART_putc(value);
}


void USART_put_float24(float value){
     unsigned char *pt;
     pt=(unsigned char *) &value;
     USART_putc(*(pt++));
     USART_putc(*(pt++));
     USART_putc(*(pt++));     
}


void USART_put_string(char *vetor)
{       
    char *ptr;
    ptr=vetor;
    while(*ptr){ 
        USART_putc(*ptr);
        ptr++;
    }
    USART_putc(*ptr);    
}