/*
 * uart.h
 *
 *  Created on: Oct 21, 2024
 *      Author: ASUS
 */

#ifndef INC_UART_H_
#define INC_UART_H_

#include "main.h"
#include "usart.h"
#include "physics.h"

extern uint8_t receive_buffer1;
extern uint8_t receive_buffer2;
extern unsigned char buffer1_flag;
extern unsigned char buffer2_flag;
extern uint8_t buffer1[];
extern uint8_t buffer2[];
extern uint8_t index_buffer1;
extern uint8_t index_buffer2;




void Uart_SendByte(UART_HandleTypeDef* huart, uint8_t* byte, uint16_t size);
void Uart_SendString(UART_HandleTypeDef* huart, uint8_t* str);
void Uart_StartReceive(UART_HandleTypeDef* huart);

void Uart_InitESP();
HAL_StatusTypeDef Uart_TestESP();
void Uart_ESPSendByte(uint8_t* byte, uint16_t size);
void Uart_ESPSendString(uint8_t* str);
void HAL_UART_RxCpltCallback(UART_HandleTypeDef* huart);



#endif /* INC_UART_H_ */
