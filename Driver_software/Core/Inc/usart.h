#ifndef __usart_H
#define __usart_H
#ifdef __cplusplus
extern "C" {
#endif

#include "main.h"


extern UART_HandleTypeDef huart1;
extern UART_HandleTypeDef huart3;

extern uint8_t uart_input_time_flag;
extern uint8_t uart_input_flag;



void MX_USART1_UART_Init(void);
void MX_USART3_UART_Init(void);


uint8_t message_interpret(const uint8_t *buffer, uint8_t length);

#ifdef __cplusplus
}
#endif
#endif /*__ usart_H */

