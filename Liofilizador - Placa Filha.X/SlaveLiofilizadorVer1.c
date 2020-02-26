/*-----------------------------------------------------------------------------/
 * File:   SlaveLiofilizadorVer1.h
 * Author: Neraildes
 * Revisão 1.0.0
 * Created on 31 de Maio de 2018, 8:53
 * Descrição:
 *		SlaveLiofilizador versão 1.0
 *      Compilado com XC8 Pró.
 * 
 *      Conferindo os comandos
 -----------------------------------------------------------------------------*/
/*------------------------------------------------------------------------------
 To Do List 21/12/2019
 OK Led azul em PT100 e VACUO deficiente
 OK Status Global na placa NTC
 OK Comunicação incessante quando for editar parametros.
 *  Fazer PT100 ler corretamente a temperatura 
 *  Leitura de Tensao muito elevada.
 *  Ícone do alarme do vácuo
 
 
 -----------------------------------------------------------------------------*/


#include <xc.h>
#include "global.h"
#include "protocolo.h"
#include "proculus.h"
#include "usart.h"
#include "adc.h"
#include "eeprom.h"
#include "I2C.h"
#include "EEPROM_24C1025.h"
#include "my_delay.h"
#include "versao.h"


#define RUN_ON()     RD4=1;
#define RUN_OFF()    RD4=0;  
#define UPDATE_ON()  RB5=0;
#define UPDATE_OFF() RB5=1;
#define POWER_ON()   RD2=0;
#define POWER_OFF()  RD2=1;

#define Enter USART_putc(0x0D)

void Decodify_Command(void);
void Send_To_MB(unsigned char size);
void Send_Reply_OK(void);

#ifdef PT100_UMIDADE
void mediaLeituraPt100Umidadde(unsigned char canal);
#endif

#ifdef NTC_BOARD 
void mediatemperaturaNTC(unsigned char canal);
void Load_Work(void);

void Auto_Relay0(void);
void Auto_Relay1(void);

void Rele0Ligado(void);
void Rele1Ligado(void);


void Rele0Desligado(void);
void Rele1Desligado(void);

void DelayRele(char canal);


#endif

#ifdef VACCUM_BOARD
void MediaPlacaVaccum(unsigned char canal);
#endif

void Save_Log(unsigned long add_datalog);

extern volatile char usart_buffer[USART_BUFFER_SIZE]; 
extern volatile int count;
extern volatile unsigned char *pointer;


extern volatile unsigned int  tempodecorrido;
extern volatile unsigned int  milisegundo;
extern volatile unsigned char segundo;
extern volatile unsigned char minuto;
extern volatile unsigned char hora;

extern volatile unsigned int Delay_Led_Tmr0  ; //AZUL
extern volatile unsigned int Delay_Led_Usart ; //VERMELHO
extern volatile unsigned int Delay_Led_Memory; //Verde




#ifdef NTC_BOARD 

int   Status0;
volatile float SetPoint0;
float Histerese0;
char  TempoON_0;
char  TempoOFF_0;
char  IndexLeitura_0;
volatile char  TempoCNT_0;

int   Status1;
volatile float SetPoint1;
char  Histerese1;
char  TempoON_1;
char  TempoOFF_1;
char  IndexLeitura_1;
volatile char TempoCNT_1;


float Temperatura0,Temperatura1;
unsigned char RL0Status=SOBE; 
unsigned char RL1Status=SOBE;
#endif


#ifdef VACCUM_BOARD
float Vaccum0, Tensao1;
#endif

#ifdef PT100_UMIDADE
float Temperatura0,Umidade1;
#endif

t_usart_protocol usart_protocol;
unsigned char    canal;
char             Board_Number;
char             texto[16];
char             buffer[32];

unsigned char statuspower_memo;


unsigned int  header;
unsigned char destino;



void main(void) {
     __delay_ms(100);
     OSCCONbits.IRCF0=1; //Frequencia do oscilador interno = 8Mhz
     OSCCONbits.IRCF1=1; 
     OSCCONbits.IRCF2=1; 
     
     PORTA=0;
     PORTB=0;
     PORTC=0;
     PORTE=0;
     TRISA=0b11101111;
     TRISB=0b00001111;
     TRISC=0xF0;
     TRISE=0xFF; 

     ANSEL=0x00000011;                  //AN0 e AN1 ANALOGIC ON
     ANSELH=0x00000000;     
     
     OPTION_REGbits.nRBPU=0;
     WPUB=0b00001111;
     

     flag_led_tmr0  =1; 
     flag_led_usart =1;  
     flag_led_memory=1;
     my_delay_ms_WDT(1000);     
     
     OPTION_REGbits.T0CS=0;
     OPTION_REGbits.PSA =1;
     OPTION_REGbits.PS0 =1;
     OPTION_REGbits.PS1 =1;
     OPTION_REGbits.PS2 =1;    
     INTCONbits.T0IE    =0;  //Timer0     
     INTCONbits.INTE    =0;  //Interrupção em RB0    
     INTCONbits.GIE     =1;  //Global Interrupt                           |

     //------------timer1-----------------------
     INTCONbits.PEIE = 1; //Periferical Interrupt Enable 
     T1CKPS0=1;
     T1CKPS1=1;
     TMR1CS=0;
     TMR1IE=1;
     T1CON=1;
     
     


     //-------------------------------------------------------------------------
     asm("CLRWDT");
     ADC_init();
     I2C_Master_Init(100000);      //Initialize I2C Master with 400KHz clock
     USART_init(115200);     
     //-------------------------------------------------------------------------
     


     //-------------------------------------------------------------------------
     {//-----------TOTALIZADOR DE RESET-------------         
     unsigned int reset;    
     reset=EEPROM_Read_Integer(0x10);
     if(reset==0xFFFF) EEPROM_Write_Integer(0x10,0); 
     reset++;
     EEPROM_Write_Integer(0x10,reset);
     }     
     
     
     
     Delay_Led_Tmr0   = 0; //AZUL
     Delay_Led_Usart  = 0; //VERMELHO
     Delay_Led_Memory = 0; //Verde     
     
     segundo=0;
     hora=0;
     RELE_0=0;
     RELE_1=0;
     IN_1=0; 
     IN_2=0; 
     
          
     Board_Number=DEVICE_BOARD;
     canal=1; 
     //my_delay_ms(Board_Number*150);
     
     #ifdef PT100_UMIDADE
     flag_global_condensador=0;
     #endif
     
     #ifdef VACCUM_BOARD 
     flag_global_vacuo=0;
     #endif
     
     #ifdef NTC_BOARD          
     //IconIndex0 = 0;//(Board_Number-3)*2; 
     //IconIndex1 = 1((Board_Number-3)*2)+1;
     Load_Work();
     flag_global_hot=0;
     #endif  


    
     
     while(1)
          {
         //============================ WDT ====================================
         flag_main_loop_WDT=TRUE;
         
         //================PREPARAR DADOS A SEREM ENVIADOS======================
         #ifdef NTC_BOARD 
         if(++canal==2) canal=0;
         mediatemperaturaNTC(canal);
         
         //=====================================================================
         if(flag_global_hot)             
            {            
             Auto_Relay0();
             Auto_Relay1();
            }
         else
            {             
            RELE_0=0; 
            RELE_1=0; 
            }               
         #endif

         #ifdef PT100_UMIDADE
         if(++canal==2) canal=0;
         mediaLeituraPt100Umidadde(canal); 
         #endif         

         #ifdef VACCUM_BOARD    
         if(++canal==2) canal=0;
         MediaPlacaVaccum(canal);  
         #endif         
         //=====================================================================
 



         //======================= COMUNICAÇÃO =================================         
         if(flag_usart_rx==1)
            {               
            header  =(((unsigned int)usart_buffer[0]<<8)+usart_buffer[1]);         
            if(header==HEADER_LIOFILIZADOR)
               {
               destino =usart_buffer[3]; 
               if((destino==Board_Number)||(destino==TODOS))
                      {
                      Delay_Led_Usart=DEFAULT_LEDS;                       
                      Decodify_Command();
                      flag_usart_rx=0;
                      }             
               }            
            }
         
         //=====================================================================
         }//while(1)         
 }// main(void)


//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
void Decodify_Command(void)
{   
    int     dados;
    char    tempchar;
    int     tempint;
    float   OutPut;
    unsigned long add_24LCxxxx;
    
    
    /*
    
    unsigned char tempbyte;
    unsigned int  tempint;
    float         tempdouble;
    unsigned int  retorno;
    unsigned long add_24LCxxxx;
    */
    
    USART_to_Protocol(&usart_protocol);
    
    High(add_24LCxxxx)=(usart_protocol.value[1]);
    Lower(add_24LCxxxx)=(usart_protocol.value[2]);
    Hi(add_24LCxxxx)=(usart_protocol.value[3]);
    Lo(add_24LCxxxx)=(usart_protocol.value[4]);  
    
    
    
    
    
    
    switch(usart_protocol.command){
        /*           
        case COMMAND_RESETS:
             tempint=(EEPROM_Read_Byte(0x10)<<8)+(EEPROM_Read_Byte(0x11));
             Send_To_MB(2);             
             USART_putc(tempint);            
             break;
        */  
        case COMMAND_VERSION:
             Send_To_MB(11);             
             USART_put_string(FVERSION);            
             break;            
        case COMMAND_READ_ANALOG:                       
             #ifdef NTC_BOARD
             if(usart_protocol.value[0]==0) 
                OutPut=Temperatura0;
             else
                OutPut=Temperatura1;
             #endif       
             
             #ifdef PT100_UMIDADE
             if(usart_protocol.value[0]==0)
                OutPut=Temperatura0;
             else
                OutPut=Umidade1;
             #endif                  
             
             #ifdef VACCUM_BOARD
             if(usart_protocol.value[0]==0)
                OutPut=Tensao1;
             else
                OutPut=Vaccum0;
             #endif               
             
             OutPut*=10.0; 
             
             INTCONbits.GIE=0;
             Send_To_MB(2);             
             USART_put_int(OutPut);
             INTCONbits.GIE=1;
             break;   
             
             
        case COMMAND_SAVE_LOG:
             //Comando geral para todas as placas, não há resposta.
             High (add_24LCxxxx) =(usart_protocol.value[0]);
             Lower(add_24LCxxxx) =(usart_protocol.value[1]);
             Hi   (add_24LCxxxx) =(usart_protocol.value[2]);
             Lo   (add_24LCxxxx) =(usart_protocol.value[3]);              
             Save_Log(add_24LCxxxx);                
             Delay_Led_Tmr0=0;  //___Sincroniza as leituras
             milisegundo=0;     //        
             break;
        
        #ifdef NTC_BOARD     
        case COMMAND_GLOBAL_HOT:
             if(usart_protocol.value[0]==0)
                flag_global_hot=0;
             else
                flag_global_hot=1;                          
             break;
        #endif     
        
        case COMANDO_QUEM_EH_VOCE: // Envia um inteiro    
             Send_To_MB(2); 
             #ifdef VACCUM_BOARD
             USART_put_int(Vaccum_Board);
             #endif             

             #ifdef PT100_UMIDADE
             USART_put_int(PT100_Board);
             #endif

             #ifdef NTC_BOARD
             USART_put_int(NTC_Board);
             #endif
             break;         
        
//        case COMMAND_PING:
//             Send_To_MB(1);
//             USART_put_int(COMMAND_PING);
//             break; 
        
        #ifdef NTC_BOARD     
        case COMMAND_LOAD_WORK:
             Send_To_MB(3);
             Send_Reply_OK();
             Load_Work();
             break;
        #endif     
                        
        //---------------------- EEPROM INTERNA --------------------------------
        case COMMAND_IEE_W_BYTE:
             EEPROM_Write_Byte(usart_protocol.value[0],
                               usart_protocol.value[1]);
             Send_To_MB(3);
             Send_Reply_OK();
             break;
        case COMMAND_IEE_R_BYTE:
             tempchar=EEPROM_Read_Byte(usart_protocol.value[0]);            
             Send_To_MB(1);;
             USART_putc(tempchar);              
             break;
        case COMMAND_IEE_W_INT:
             {
             unsigned char add;
             add=usart_protocol.value[0];
             dados=(usart_protocol.value[1]<<8)|usart_protocol.value[2];
             EEPROM_Write_Integer(add,dados);
             Send_To_MB(3);
             Send_Reply_OK();
             }
             break;
        case COMMAND_IEE_R_INT:
             tempint=EEPROM_Read_Integer(usart_protocol.value[0]);
             Send_To_MB(2);
             USART_put_int(tempint);
             break;
             
        case COMMAND_IEE_W_STR :
             EEPROM_Write_String(usart_protocol.value[0],
                                &usart_protocol.value[1]);
             Send_To_MB(3);
             Send_Reply_OK();
             break;
        case COMMAND_IEE_R_STR:
             {
             //unsigned char texto[20];
             EEPROM_Read_String(usart_protocol.value[0],texto);
             Send_To_MB(sizeof(texto));
             USART_put_string(texto);
             break;
             }

        //---------------------- EEPROM EXTERNA --------------------------------
        case COMMAND_EEE_W_BYTE:
             EEPROM_24C1025_Write_Byte(usart_protocol.value[0],      //CHIP NUMBER
                                                  add_24LCxxxx,      //ADD of Memory                                                  
                                       usart_protocol.value[5]);     //VALUE
             Send_To_MB(3);
             Send_Reply_OK();
             break;
        case COMMAND_EEE_R_BYTE:
             tempchar=EEPROM_24C1025_Read_Byte(usart_protocol.value[0],  //CHIP NUMBER
                                                         add_24LCxxxx);  //ADD of Memory
             
             
             Send_To_MB(1);
             USART_putc(tempchar);
             break;
        case COMMAND_EEE_W_INT:
             {
             EEPROM_24C1025_Write_Int(usart_protocol.value[0],  //CHIP NUMBER
                                                 add_24LCxxxx,  //Add of memory
                             (int)(usart_protocol.value[5]<<8) |
                                    (usart_protocol.value[6]));                     
                                                      
             Send_To_MB(3);
             Send_Reply_OK();
             break;
             }
        case COMMAND_EEE_R_INT:
             tempint=EEPROM_24C1025_Read_Int(usart_protocol.value[0],    //CHIP NUMBER
                                                       add_24LCxxxx);
             Send_To_MB(2);
             USART_put_int(tempint);
             break;
             
             
        case COMMAND_EEE_R_BUF:
             {                 
             char sizedata;
             sizedata=usart_protocol.value[5];             
             EEPROM_24C1025_Read_Buffer(usart_protocol.value[0],  //CHIP NUMBER
                                        add_24LCxxxx,  //Add of memory
                                        sizedata,  //SIZEDATA
                                        buffer);  //Buffer of data 
             
            
             
             Send_To_MB(sizedata);
             USART_put_buffer(buffer,sizedata); 
             }
             break;
             
        case COMMAND_EEE_W_BUF: 
             EEPROM_24C1025_Write_Buffer(usart_protocol.value[0],     //CHIP NUMBER
                                         add_24LCxxxx,     //ADD Long
                                         usart_protocol.value[5],     //SIZEDATA
                                         &usart_protocol.value[6]);    //Inicio do Buff
             
             Send_To_MB(3);
             Send_Reply_OK();            
             break;                    

        case COMMAND_EEE_W_STR :
             EEPROM_24C1025_Write_Str(usart_protocol.value[0],   //CHIP NUMBER
                                                 add_24LCxxxx,   //ENDERECO 24LCXXXX
                                    &usart_protocol.value[5]);   //Ponteiro String
             Send_To_MB(3);
             Send_Reply_OK();
             break;
        case COMMAND_EEE_R_STR:
             {
             EEPROM_24C1025_Read_Str(usart_protocol.value[0],       //CHIP NUMBER
                                                add_24LCxxxx,   //ENDERECO 24LCXXXX
                                                      texto);
                      
             Send_To_MB(strlen(texto));
             USART_put_string(texto);
             break;
             }
        
        case COMMAND_EEE_FILL_ALL:
             EEPROM_24C1025_Fill_All(usart_protocol.value[0], //chip_add, unsigned char value){
                                     usart_protocol.value[1]);
             Send_To_MB(3);
             Send_Reply_OK();            
             break;   
        /*     
        case COMMAND_CLK_PIC_R: 
             {
             char hh[10];
             char mm[10];
             char ss[10];             
        
             itoa(hh,hora,10);
             itoa(mm,minuto,10);
             itoa(ss,segundo,10);
            
            
             strcpy(texto,"");             
             if (hh[1]==0) 
                {
                strcat(texto,"0");
                hh[2]=0;
                }
             strcat(texto,hh);             
             strcat(texto,":");
             if(mm[1]==0) 
                {
                strcat(texto,"0");
                mm[2]=0;
                }
             strcat(texto,mm);
             strcat(texto,":");
             if(ss[1]==0) 
                { 
                strcat(texto,"0");
                ss[2]=0;
                }
             strcat(texto,ss);
             Send_To_MB(strlen(texto)); 
             USART_put_string(texto);
             USART_putc(0);
             }
             break;    
             
        case COMMAND_CLK_PIC_W: 
             hora=usart_protocol.value[0];
             minuto=usart_protocol.value[1];
             segundo=usart_protocol.value[2];
             Send_To_MB(3);
             Send_Reply_OK();             
             break; 
        */     
        case COMMAND_LDC_PAGE:
             PROCULUS_Show_Screen(usart_protocol.value[0]);
             Send_To_MB(3);
             Send_Reply_OK();        
             break;
             
        case COMMAND_PROCULUS_Buzzer:
             PROCULUS_Buzzer((usart_protocol.value[0]<<8)+
                             (usart_protocol.value[1]));
             Send_To_MB(3);
             Send_Reply_OK();              
             break;
        case COMMAND_LCD_W_VP_STR:             
             PROCULUS_VP_Write_String((usart_protocol.value[0]<<8)+
                                       usart_protocol.value[1],
                                      &usart_protocol.value[2]);
             Send_To_MB(3);
             Send_Reply_OK();              
             break; 
             
             
        case COMMAND_RELAY:            
             if(usart_protocol.value[0]==0)
                { 
                if(!usart_protocol.value[1]) 
                   RELE_0=0;
                else
                   RELE_0=1; 
                }        
             else if (usart_protocol.value[0]==1)
                     {
                      if(!usart_protocol.value[1]) 
                         RELE_1=0;
                      else
                         RELE_1=1;                               
                     } 
             Send_To_MB(3);
             Send_Reply_OK();              
             break; 
                                    
    }//case
}//function

//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX


void Send_To_MB(unsigned char size){     
     //USART_put_int(HEADER_SLAVE);         //HEADER FIX - nao envia INT
     USART_putc(0xAA);                      //Header 
     USART_putc(0xBB);                      //ORIGEM
     USART_putc(Board_Number);              //ORIGEM
     USART_putc(0x00);                      //DESTINO
     USART_putc(usart_protocol.command);    //COMANDO
     USART_putc(size);                      //TAMANHO
}


void Send_Reply_OK(void){
     USART_put_string("OK");           //Origem do Dipswitch
   
 }
 
    





#ifdef NTC_BOARD
void DelayRele(char canal){
     my_delay_ms((canal+2*Board_Number-2)*10);    
}
#endif




//==============================================================================
#ifdef NTC_BOARD
void mediatemperaturaNTC(unsigned char canal)
     {  
     float Temp;
     char i; 
     
     ADCON0bits.CHS=canal;
     my_delay_ms(1);     
     
     Temp=0;
     //flag_led_memory=1; //fix desativar
     for(i=1;i<15;i++)
         {   
         Temp+=ADC_Read_NTC(canal);         
         if(Package_Usart_is_for_me()==TRUE) break;  
         
         if(flag_global_hot)
           { 

           if((TempoCNT_0==0)&&
              (RL0Status==SOBE)&&
              (Status0==1)) Rele0Ligado();

           if((TempoCNT_1==0)&&
              (RL1Status==SOBE)&&
              (Status1==1)) Rele1Ligado();
             
           }
         }
     if(i<10)
       {
       //flag_led_memory=0; //fix desativar  
       return;
       }
     if(i==15)
        Temp/=(i-1);
     else
        Temp/=(i);
     
     if(canal==0)
        {
        Temperatura0=Temp; 
        }
     else
        { 
        Temperatura1=Temp; 
        }           
     //flag_led_memory=0; //fix desativar
     }  
#endif
//==============================================================================


#ifdef PT100_UMIDADE
void mediaLeituraPt100Umidadde(unsigned char canal)
     {  
     float Temp;
     float tensao;
     char i;              
     
     ADCON0bits.CHS=canal;
     my_delay_ms(1);
     Temp=0;
     //flag_led_memory=1; //fix desativar
     for(i=1;i<15;i++)
        {   
        Temp+=ADC_Media_10bits(canal);         
        if(Package_Usart_is_for_me()==TRUE) break;            
        }
     if(i<10)
        {
        //flag_led_memory=0; //fix desativar 
        return;
        }


     if(i==15)
       Temp/=(i-1);
     else
       Temp/=(i);  

     
     
     //Este calculo antes da selecão de canais permite escolher entre canal 0
     //e canal 1 e ter a mesma leitura apesar da variavel se chamar Umidade1.
     tensao=(5.0/1023.0);
     Temp*=tensao; 
     Temp*=40.0;
     Temp-=110.0;


     if(canal==0)
        {
        Temperatura0=Temp;                 
        }
     else
        {
        Umidade1=Temp;
        }
     //flag_led_memory=0; //fix desativar
     }  
#endif






//==============================================================================
#ifdef VACCUM_BOARD
void MediaPlacaVaccum(unsigned char canal){
     float Temp,Valor;
     unsigned int i; 
     
     
     
     if(canal==0) //---------------Sensor de Vacuo------------------------------
       {  
       ADCON1bits.VCFG0=1; //Ref+
       ADCON1bits.VCFG1=1; //Ref-
       ADCON0bits.CHS=canal;
       my_delay_ms(1);
       Temp=0;
       //flag_led_memory=1; //fix apagar 
       for(i=1;i<15;i++)
           { 
           Temp+=ADC_Media_10bits(canal);         
           if(Package_Usart_is_for_me()==TRUE) break;            
           }
       if(i<10)
         {
         //flag_led_memory=0; //fix apagar  
         return;
         }

       if(i==15)
         Temp/=(i-1);
       else
         Temp/=(i);       
             
       Vaccum0 =1023.0-Temp; 
       Vaccum0*=Vaccum0;
       Vaccum0*=0.00222;
       Vaccum0+=150;
       
       if(Vaccum0>2000)Vaccum0=2000;          
       //flag_led_memory=0; //fix apagar 
       }     
     else //------------------ SENSOR DE TENSÃO---------------------------------
       {  
       ADCON1bits.VCFG0=0; //Ref+
       ADCON1bits.VCFG1=0; //Ref-
       ADCON0bits.CHS=canal;
       my_delay_ms(1);
       Temp=0;
       //flag_led_memory=1; //fix apagar 
       for(i=1;i<15;i++)
           {  
           Valor=ADC_Max_10Bits(canal);
           if(Valor>Temp) Temp=Valor; 
           if(Package_Usart_is_for_me()==TRUE) break;        
           }  
        if(i<10)
          {
          //flag_led_memory=0; //fix apagar   
          return;
          }
        Tensao1 =Temp*0.318296;
        if(Tensao1<15) Tensao1=0;
        //flag_led_memory=0; //fix apagar 
       }
}     
#endif
//==============================================================================

 

#ifdef NTC_BOARD
void Auto_Relay0(void){
             if(Status0==0)
                 {
                 Rele0Desligado();
                 }  
             else        
                 {
                //================ CONTROLE DE TEMPO DO RELE 0 =================
                 if (Temperatura0<=(SetPoint0-Histerese0)) //Temperatura baixa
                    {                    
                    RL0Status=SOBE;
                    Rele0Ligado(); 
                    }  
                 else if (Temperatura0<=SetPoint0) //Temperatura Intermediária
                          {                      
                          if(RL0Status==SOBE)
                              {  
                              Rele0Ligado();
                              }
                          else
                              {
                              Rele0Desligado();
                              }
                          }
                 else if (Temperatura0>SetPoint0) //Temperatura maior que setpoint
                          {                      
                          RL0Status=DESCE; 
                          Rele0Desligado();
                          }
                 }

            //-----------------------------------------------------------------    
}           
//------------------------------------------------------------------------------


void Auto_Relay1(void){
             if(Status1==0)
                 {
                 Rele1Desligado();
                 }                   
             else
                 {
                //================ CONTROLE DE TEMPO DO RELE 0 =================
                 if (Temperatura1<=(SetPoint1-Histerese1)) //Temperatura baixa
                    {                    
                    RL1Status=SOBE;
                    Rele1Ligado(); 
                    }  
                 else if (Temperatura1<=SetPoint1) //Temperatura Intermediária
                          {                      
                          if(RL1Status==SOBE)
                              {  
                              Rele1Ligado();
                              }
                          else
                              {
                              Rele1Desligado();
                              }
                          }
                 else if (Temperatura1>SetPoint1) //Temperatura maior que setpoint
                          {                      
                          RL1Status=DESCE; 
                          Rele1Desligado();
                          }
                 }
       
             //-----------------------------------------------------------------    
}         


void Rele0Desligado(void){
     RELE_0=0;
}

void Rele1Desligado(void){
     RELE_1=0;
}


void Rele0Ligado(void){
        //--------------------------RELE 0 LIGADO-------------------------------
           
        if(TempoCNT_0==0)
            {  
            //my_delay_ms((0+2*Board_Number-2)*10);DelayRele(0);
            //Load_Work();
            if(RELE_0==0)
              {  
              TempoCNT_0=TempoON_0;               
              RELE_0=1;            
              }
            else 
              {  
              TempoCNT_0=TempoOFF_0;               
              RELE_0=0;  
              }
            }               
}    


void Rele1Ligado(void){
        //--------------------------RELE 0 LIGADO-------------------------------
           
        if(TempoCNT_1==0)
            {  
            //my_delay_ms((0+2*Board_Number-2)*10);DelayRele(0);
            //Load_Work();
            if(RELE_1==0)
              {  
              TempoCNT_1=TempoON_1;               
              RELE_1=1;            
              }
            else 
              {  
              TempoCNT_1=TempoOFF_1;               
              RELE_1=0;  
              }
            }               
} 





void Load_Work(void){ 
     //EEPROM_Write_Integer(7,234);   
     // O R I G I N A L
     SetPoint0 = EEPROM_Read_Integer(0);  // 0 e 1
     SetPoint0/=10;
     TempoON_0 = EEPROM_Read_Byte(2);
     TempoOFF_0= EEPROM_Read_Byte(3);
     Histerese0= EEPROM_Read_Byte(4);  
     Status0   = EEPROM_Read_Integer(5); //5 e 6
     
     SetPoint1 = EEPROM_Read_Integer(7); // 7 e 8                                          
     SetPoint1/=10;
     TempoON_1 = EEPROM_Read_Byte(9);
     TempoOFF_1= EEPROM_Read_Byte(10);
     Histerese1= EEPROM_Read_Byte(11);     
     Status1   = EEPROM_Read_Integer(12); //12 e 13
     
     statuspower.bits=EEPROM_Read_Byte(14); 
}
#endif


#ifdef NTC_BOARD
void Save_Log(unsigned long add_datalog){
     EEPROM_24C1025_Write_Int(0x00, add_datalog, (int) Temperatura0*10);   
     EEPROM_24C1025_Write_Int(0x01, add_datalog, (int) Temperatura1*10);
 
     //EEPROM_24C1025_Write_Int(0x00, add_datalog+2, 0xFFFF);   
     //EEPROM_24C1025_Write_Int(0x01, add_datalog+2, 0xFFFF);     
}
#endif



#ifdef VACCUM_BOARD
void Save_Log(unsigned long add_datalog){
     EEPROM_24C1025_Write_Int(0x00, add_datalog, (int) (Tensao1*10));   
     EEPROM_24C1025_Write_Int(0x01, add_datalog, (int) (Vaccum0*10));  
     
     //EEPROM_24C1025_Write_Int(0x00, add_datalog+2, 0xFFFF);   
     //EEPROM_24C1025_Write_Int(0x01, add_datalog+2, 0xFFFF);      
}
#endif


#ifdef PT100_UMIDADE
void Save_Log(unsigned long add_datalog){
     EEPROM_24C1025_Write_Int(0x00, add_datalog, (int) Temperatura0*10);
     EEPROM_24C1025_Write_Int(0x01, add_datalog, (int) Umidade1);  
     
     //EEPROM_24C1025_Write_Int(0x00, add_datalog+2, 0xFFFF);   
     //EEPROM_24C1025_Write_Int(0x01, add_datalog+2, 0xFFFF);     
}
#endif

