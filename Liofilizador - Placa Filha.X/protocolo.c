#include "global.h"
#include "usart.h"
#include "protocolo.h"

extern volatile char usart_buffer[USART_BUFFER_SIZE];
extern char Board_Number;

char Package_Usart_is_for_me(){
     char retorno=FALSE;
     if(flag_usart_rx==1) 
        { 
        if((usart_buffer[3]==Board_Number)||(usart_buffer[3]==TODOS))     
            retorno=TRUE;
        else
            retorno=FALSE;
        }
     else
        { 
        retorno=FALSE;    
        }
     return retorno;
}

