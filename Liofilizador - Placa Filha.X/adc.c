#include "global.h"
#include "adc.h"
#include "usart.h"
#include "my_delay.h"

extern float temperatura0, temperatura1;
extern char Board_Number;


void ADC_init(void){
     ADCON0bits.ADCS=3;  //Clock derivado de um oscilador interno.
     ADCON1=0b10000000;
     ADCON1bits.ADFM=1; //10 BITS
     ADCON0bits.ADON=1; //Ligar conversor     
}



#if ((defined PT100_UMIDADE) || (defined VACCUM_BOARD))
float ADC_Media_10bits(char canal){
      unsigned int value;
      unsigned char i;
      ADCON0bits.CHS=canal;
      my_delay_ms(10);
      
      value=0;
      for(i=1;i<=50;i++)
          value+=captura();//ADC_Media_Filtro3_Read(canal);
      value/=(i-1);    
      return value;
}
#endif

unsigned int ADC_Max_10Bits(char canal){
      unsigned int Temp;
      unsigned int value;
      unsigned char i;
      ADCON0bits.CHS=canal;
      my_delay_ms(10);
      Temp=0;
      value=0;
      for(i=1;i<=50;i++)
          { 
          Temp=captura();
          if (Temp>value) value=Temp;
          }         
      return value;    
}





//------------------------------------------------------------------------------
#ifdef NTC_BOARD
float ADC_Read_NTC(char canal){
    int B = 3975;
    float resistencia;
    float temperatura;
    float calc;
    unsigned int value;
    unsigned char i;
    
    value=0;
    for(i=1;i<=50;i++)
        value+=captura();//ADC_Media_Filtro3_Read(canal);
    value/=(i-1);
    resistencia =(1023.0-value);
    resistencia*=(10000.0-value);
    temperatura =value*10000.0/resistencia;
    calc=temperatura;
    temperatura =log(calc); 
    temperatura/=B;
    temperatura+=1/(25+273.15);
    temperatura =1/temperatura;
    temperatura-=273.15;
    
    return temperatura; //
}
#endif

//------------------------------------------------------------------------------
/*
#ifdef VACCUM_BOARD
unsigned int ADC_Max_Read(char canal)
{
    t_usart_protocol usart_protocol;
    static unsigned int valido;
    char count;
    int temp;
    int valor;
    
    ADCON0bits.CHS=canal; 
    
    valor=0;
    for(count=0;count<100;count++)
        {
        temp=captura();
        if(temp>valor) valor=temp;
           if(flag_usart_rx==1)
              {
              USART_to_Protocol(&usart_protocol);                         
              if((usart_protocol.destino==Board_Number)||(usart_protocol.destino==TODOS))
                 {                  
                 break;              
                 }
              else
                 { 
                 flag_usart_rx=0; 
                 }               
              }        
        }      
    if(++canal==2) canal=0;
    ADCON0bits.CHS=canal;    
    valido=valor;
    return valor;   
}
#endif
 */ 



unsigned int captura(){
    ADCON0bits.GO=1;
    while(ADCON0bits.GO)
          {
          continue;          
          }
    return ((ADRESH<<8)+ADRESL);    
}

