#include <xc.h>
#include "global.h"
#include "proculus.h"
//#include "database.h"
#include "usart.h"
#include <stdlib.h>

extern unsigned int  tempodecorrido;
volatile extern unsigned char usart_buffer[USART_BUFFER_SIZE];

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

void PROCULUS_Control_Register_Write(unsigned char *vetor, unsigned char size){
     USART_put_int(PROCULUS_Header);
     USART_putc((unsigned char)(1+size));
     USART_putc(Control_Register_Write);
     for(unsigned char i=0;i<size;i++)
         USART_putc(vetor[i]);
     PROCULUS_Delay(TIME_AFTER_SEND_PROCULUS_COMMAND);  
}

void PROCULUS_Control_Register_Read(unsigned char reg, unsigned char size, unsigned char *retorno){
     USART_put_int(PROCULUS_Header);
     USART_putc((unsigned char)(3+size));
     USART_putc(Control_Register_Read);
     USART_putc(reg);
     USART_putc(size);

     if(USART_input_buffer()){
        for(unsigned char i=0;i<USART_BUFFER_SIZE;i++)
            retorno[i] = usart_buffer[(unsigned char)(6+i)];        
     }      
     PROCULUS_Delay(TIME_AFTER_SEND_PROCULUS_COMMAND);   
}


// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -


void PROCULUS_VP_Write(unsigned int vp, char *vetor, char size){
     USART_put_int(PROCULUS_Header);
     USART_putc((unsigned char)(3+size));
     USART_putc(VP_Write);
     USART_put_int(vp);
     for(unsigned char i=0;i<size;i++)        
         USART_putc(vetor[i]);
        
     PROCULUS_Delay(TIME_AFTER_SEND_PROCULUS_COMMAND);
}


void PROCULUS_VP_Read(unsigned int vp, char *vetor, char size){         
     USART_put_int(PROCULUS_Header);     
     USART_putc(4);
     USART_putc(VP_Read);
     USART_put_int(vp);
     USART_putc((unsigned char)(size>>1));  // size = size / 2
     
     if(USART_input_buffer()){        
        for(unsigned char i=0;i<size;i++)            
            (vetor[i]) = usart_buffer[(unsigned char)(7+i)];            
        }
     PROCULUS_Delay(TIME_AFTER_SEND_PROCULUS_COMMAND);
}



//------------------------------- VP WRITE -------------------------------------


void PROCULUS_VP_Write_Byte(unsigned int vp, unsigned char value){
     char vetor[1];
     vetor[0]=value;
     PROCULUS_VP_Write(vp,vetor,1);
}
unsigned char PROCULUS_VP_Read_Byte(unsigned int vp){
     char vetor[1];
     PROCULUS_VP_Read(vp,vetor,1);
     return vetor[0];
}



//---------------------------  I  N  T  1  6  ----------------------------------
void PROCULUS_VP_Write_UInt16(unsigned int vp, unsigned int value){
     char vetor[2];
     unsigned char *pt;
     pt=(unsigned char *)(&value);
     vetor[1]=(char)(value>>8);
     vetor[0]=(char)(value);      
     PROCULUS_VP_Write(vp,vetor,2);
}
unsigned int PROCULUS_VP_Read_UInt16(unsigned int vp){
     char vetor[2];
     PROCULUS_VP_Read(vp,vetor,2);
     return (unsigned int)((vetor[0]<<8)+vetor[1]);
}



//--------------------------------  L  O  N  G  --------------------------------
void PROCULUS_VP_Write_Long32(unsigned int vp,long value){
     char vetor[4];
     unsigned char *pt;
     pt=(unsigned char *)(&value);
     vetor[3]=(char)(*(pt++)); 
     vetor[2]=(char)(*(pt++));    
     vetor[1]=(char)(*(pt++));
     vetor[0]=(char)(*(pt++));    
     PROCULUS_VP_Write(vp,vetor,4);
}
long PROCULUS_VP_Read_Long32(unsigned int vp){
     char vetor[4];
     PROCULUS_VP_Read(vp,vetor,4);
     return ((long)*(vetor+0)<<24)|
            ((long)*(vetor+1)<<16)|
            ((long)*(vetor+2)<<8) |
             (long)*(vetor+3);            
}
 


/*xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx
 *                       P O N T O      F L U T U A N T E                       
 xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx*/

/*
//--------------------------------- F L O A T ----------------------------------
void PROCULUS_VP_Write_Float24(unsigned int vp, float value){
     char vetor[4];
     unsigned char *pt;
     pt=(unsigned char *)(&value);
     vetor[2]=(char)(*(pt++));    
     vetor[1]=(char)(*(pt++));
     vetor[0]=(char)(*(pt++));      
     PROCULUS_VP_Write(vp,vetor,3);
}
 float PROCULUS_VP_Read_Float24(unsigned int vp){
     char vetor[4];
     PROCULUS_VP_Read(vp,vetor,4);
     return ((int)*(vetor+0)<<16)|
            ((int)*(vetor+1)<<8) |
             (int)*(vetor+2);      
}
*/

void PROCULUS_VP_Write_Float32(unsigned int vp, float value){
     char vetor[4];
     unsigned char *pt;
     pt=(unsigned char *)(&value);
     vetor[3]=(char)(*(pt++)); 
     vetor[2]=(char)(*(pt++));    
     vetor[1]=(char)(*(pt++));
     vetor[0]=(char)(*(pt++));      
     PROCULUS_VP_Write(vp,vetor,4);
}
 float PROCULUS_VP_Read_Float32(unsigned int vp){
     char vetor[4];
     PROCULUS_VP_Read(vp,vetor,4);
     return 0; // fix (vetor[0]<<24)+(vetor[1]<<16)+(vetor[2]<<8)+(vetor[3]);
}




//------------------------------ D O U B L E -----------------------------------
/*
void PROCULUS_VP_Write_Double24(unsigned int vp, double value){ //Não testada
     char  vetor[3];
     unsigned  char  *pnt;
     
     pnt=(unsigned char *)(&value);
     vetor[0]=*(pnt++);
     vetor[1]=*(pnt++);
     vetor[2]=*(pnt++);
     PROCULUS_VP_Write(vp,vetor,3);          
}
double PROCULUS_VP_Read_Double24(unsigned int vp){
     char vetor[3];
     PROCULUS_VP_Read(vp,vetor,3);
     return (vetor[0]<<16)+(vetor[1]<<8)+(vetor[2]);
}
*/
 
 
void PROCULUS_VP_Write_Double32(unsigned int vp, double value){
     char vetor[4];
     /*
     vetor[0]=(double)(*(value))>>32;
     vetor[1]=(char) *value>>16;  //fix  
     vetor[2]=(char) *value>>8;        
     vetor[3]=(char) *value;
     */ 
     PROCULUS_VP_Write(vp,vetor,4);
}     
double PROCULUS_VP_Read_Double32(unsigned int vp){
     char vetor[4];
     PROCULUS_VP_Read(vp,vetor,4);
     return 0;// fix(vetor[0]<<24)+(vetor[1]<<16)+(vetor[2]<<8)+(vetor[3]);
}     


//--------------------------- S T R I N G --------------------------------------

void PROCULUS_VP_Write_String(unsigned int vp, char *text){
     PROCULUS_VP_Write(vp, text, strlen(text)+1);
}
void PROCULUS_VP_Read_String(unsigned int vp, char *text){
     PROCULUS_VP_Read(vp, text, NAME_TASK_SIZE);
}



//------------------------------------------------------------------------------


unsigned char PROCULUS_Read_Version(void){
    unsigned char retorno[2];
    PROCULUS_Control_Register_Read(VERSION_INFO,2,retorno);
    return retorno[0];
}

void PROCULUS_Buzzer(unsigned int time_ms_x_10){     
     unsigned char vetor[2];
     time_ms_x_10 /= 10.0;
     vetor[0]=ACTIVE_BUZZER;
     vetor[1]=(unsigned char) time_ms_x_10;
     PROCULUS_Control_Register_Write(vetor,2); 
}

void PROCULUS_Reset(void){
     unsigned char vetor[3];
     vetor[0]=0xEE;
     vetor[1]=0x5A;
     vetor[2]=0xA5;
     PROCULUS_Control_Register_Write(vetor,3); 
}




//---------------------------------------------------------------------
void PROCULUS_Show_Screen(unsigned int screen){
     unsigned char vetor[3];
     vetor[0] = PIC_ID;
     vetor[1] = (char)(screen>>8);  
     vetor[2] = (char) screen;              
     PROCULUS_Control_Register_Write(vetor,3);     
}




void PROCULUS_Buffer_to_Proculus(t_proculus *proculus){
     proculus->header      =(*(usart_buffer+0)<<8)+*(usart_buffer+1);
     proculus->size        = *(usart_buffer+2);
     proculus->function    = *(usart_buffer+3);
     proculus->VP          =(*(usart_buffer+4)<<8)+*(usart_buffer+5);
     proculus->length      = *(usart_buffer+6);
     for(unsigned char i=0;i<PROCULUS_DATA_SIZE;i++) 
         proculus->data[i] = usart_buffer[7+i];
}




void PROCULUS_Delay(unsigned int tempo_ms){     
    for(unsigned int i=0;i<tempo_ms;i++){
        __delay_ms(1);        
    }
}



//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX

/*

//------------------------- Read Version of Display ---------------------
unsigned int PROCULUS_Version(void){
    unsigned char result;
    
    USART_put_int(PROCULUS_Header);
    USART_put_int(PROCULUS_Reg_Read);
    USART_put_int(0x0001);
    
    if(USART_input_buffer())
       result = *(&usart_buffer[6]);
    else
       result = 0;
    
    PROCULUS_Delay_Tx(TIME_AFTER_SEND_PROCULUS_COMMAND);     
    
    return result;

}


//----------------------------- RESET the LCD -------------------------
void PROCULUS_Reset(void){
     USART_put_int(PROCULUS_Header);
     USART_put_int(0x0480);
     USART_put_int(0xEE5A);
     USART_putc(0xA5);
     
     PROCULUS_Delay_Tx(TIME_AFTER_SEND_PROCULUS_COMMAND);      
}


//------------------------------ BUZZER -------------------------------
void PROCULUS_Buzzer(unsigned int tempo){
     unsigned int value;    
     tempo /= 10.0;
     value=(0x0200|tempo);
     USART_put_int(PROCULUS_Header);
     USART_put_int(PROCULUS_Reg_Write);         
     USART_put_int(value);   
     
     PROCULUS_Delay_Tx(TIME_AFTER_SEND_PROCULUS_COMMAND);      
}


//---------------------------------------------------------------------
void PROCULUS_Show_Screen(unsigned int screen){
     USART_put_int(PROCULUS_Header);
     USART_put_int(0x0480);
     USART_put_int(0x0300);
     USART_putc(screen);
     
     PROCULUS_Delay_Tx(TIME_AFTER_SEND_PROCULUS_COMMAND);      
}



//------------------------------------------------------------------------------
void PROCULUS_Send_Char(unsigned char caracter, unsigned char size, unsigned int vp){
     unsigned char byte_count; 
    
     byte_count=3+size; 
    
     USART_put_int(PROCULUS_Header);
     USART_putc(byte_count);
     USART_put_int(PROCULUS_VP_Write);
     USART_putc((char)(vp>>8)); 
     USART_putc((char)vp); 
     for(unsigned char i=0;i<size;i++)
         USART_putc(caracter);   
    
     PROCULUS_Delay_Tx(TIME_AFTER_SEND_PROCULUS_COMMAND); 
} 





//--------------------------------------------------------------------
void PROCULUS_Write_VP_String(unsigned char *buffer, unsigned int vp){
     unsigned char byte_count; 
    
     byte_count=3+(strlen(buffer)+1); 
    
     USART_put_int(PROCULUS_Header);
     USART_putc(byte_count);
     USART_put_int(PROCULUS_VP_Write);
     USART_putc((char)(vp>>8)); 
     USART_putc((char)vp); 
     for(unsigned char i=0;i<strlen(buffer);i++)
         USART_putc(*(buffer+i));  
     USART_putc(0);
     
     PROCULUS_Delay_Tx(TIME_AFTER_SEND_PROCULUS_COMMAND);
} 


//------------------------------------------------------------------------------
unsigned char PROCULUS_Read_VP_String(unsigned int vp, unsigned char size, unsigned char *output){
    unsigned char result=FALSE;
    
    USART_put_int(PROCULUS_Header);
    USART_put_int(0x0483);
    USART_put_int(vp);
    USART_putc(size); 
    
    if(USART_input_buffer()){ //dados vem no usart_buffer
       result=TRUE;
       for(unsigned char i=0; i<size; i++)
           output[i]=usart_buffer[7+i];          
       }    
    return result;   
}


//--------------------------------------------------------------------
unsigned char PROCULUS_Read_VP_Long24(unsigned int vp, unsigned char size, unsigned long *output){
    unsigned char result=FALSE;
   
    USART_put_int(PROCULUS_Header);
    USART_put_int(0x0483);
    USART_put_int(vp);
    USART_putc(size); 
   
    if(USART_input_buffer()){ //dados vem no usart_buffer
       result=TRUE;
       
       *(output)=(usart_buffer[7]<<24)+
                 (usart_buffer[8]<<16)+
                 (usart_buffer[9]<<8)+
                 (usart_buffer[10]); 
       } 
         
    return result;   
}



//----------------------------------------------------------------------
void PROCULUS_Write_VP_Long24(unsigned long value, unsigned int vp){
     unsigned char byte_count;
    
     byte_count=3+4;
    
     USART_put_int(PROCULUS_Header);
     USART_putc(byte_count);
     USART_put_int(PROCULUS_VP_Write);
     USART_putc(vp>>8);
     USART_putc(vp);
     USART_put_long(value);
     
     PROCULUS_Delay_Tx(TIME_AFTER_SEND_PROCULUS_COMMAND);

} 


//------------------------------------------------------------------------------
unsigned int PROCULUS_Read_Int(unsigned int vp){
    unsigned char output;
    t_proculus proculus;
    unsigned char *pt;
    unsigned char result=FALSE;
    
    proculus.header=PROCULUS_Header;
    proculus.function=0x1883;
    proculus.VP=vp;
    proculus.length=2; 
    
    if(USART_input_buffer()){ //dados vem no usart_buffer
       proculus.header=usart_buffer[0]<<8+usart_buffer[1]<<0;
       proculus.function=usart_buffer[2]<<8+usart_buffer[3]<<0;
       proculus.VP=usart_buffer[4]<<8+usart_buffer[5]<<5;
       proculus.length=usart_buffer[6];
       strcpy(proculus.data,&usart_buffer[7]);
       output=proculus.data;
       result=TRUE;          
       }    
    return usart_buffer[7]<<8+usart_buffer[6];
}

//------------------------------------------------------------------------------
unsigned long PROCULUS_Read_Long(unsigned int vp){
    unsigned char output;
    t_proculus proculus;
    unsigned char *pt;
    unsigned char result=FALSE;
    
    proculus.header=PROCULUS_Header;
    proculus.function=0x1883;
    proculus.VP=vp;
    proculus.length=2; 
    
    if(USART_input_buffer()){ //dados vem no usart_buffer
       proculus.header=usart_buffer[0]<<8+usart_buffer[1]<<0;
       proculus.function=usart_buffer[2]<<8+usart_buffer[3]<<0;
       proculus.VP=usart_buffer[4]<<8+usart_buffer[5]<<5;
       proculus.length=usart_buffer[6];
       strcpy(proculus.data,&usart_buffer[7]);
       output=proculus.data;
       result=TRUE;          
       }    
    return usart_buffer[8]<<8+usart_buffer[7]<<8+usart_buffer[6];
}


void PROCULUS_Buffer_to_Proculus(t_proculus *proculus){
     proculus->header      =(*(usart_buffer+0)<<8)+*(usart_buffer+1);
     proculus->function    =(*(usart_buffer+2)<<8)+*(usart_buffer+3);
     proculus->VP          =(*(usart_buffer+4)<<8)+*(usart_buffer+5);
     proculus->length      = *(usart_buffer+6);
     for(unsigned char i=0;i<PROCULUS_DATA_SIZE;i++) 
         proculus->data[i] = usart_buffer[7+i];
}






unsigned char USART_put_Proculus(t_proculus *proculus){
    unsigned char result=FALSE;
    USART_put_int(proculus->header);
    USART_put_int(proculus->function);
    USART_put_int(proculus->VP);
    USART_putc(proculus->length);
}





//----------------------------------------------------------------------
void PROCULUS_Send_Int(unsigned int value, unsigned int vp){
     unsigned char byte_count;
     byte_count=3+4;    
     USART_put_int(PROCULUS_Header);
     USART_put_int(0x0483);
     USART_put_int(vp);
     USART_putc(2); //size  
     
     PROCULUS_Delay_Tx(TIME_AFTER_SEND_PROCULUS_COMMAND);
} 




//-----------------------------------------------------------------------
void PROCULUS_Send_Float24(unsigned long value, unsigned int vp){
     unsigned char byte_count;
    
     byte_count=3+3;
    
     USART_put_int(PROCULUS_Header);
     USART_putc(byte_count);
     USART_put_int(PROCULUS_VP_Write);
     USART_putc(vp>>8);
     USART_putc(vp);
     USART_put_float24(value);
     
     PROCULUS_Delay_Tx(TIME_AFTER_SEND_PROCULUS_COMMAND);
     
} 





*/


/*xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx
 * 
 * 
 * 
 xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx*/


