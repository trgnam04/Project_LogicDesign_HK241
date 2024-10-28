/*
 * uart.c
 *
 *  Created on: Oct 21, 2024
 *      Author: ASUS
 */

#include "uart.h"

#define BUFFER_SIZE 30

uint8_t receive_buffer1;
uint8_t receive_buffer2;
unsigned char buffer1_flag = 0;
unsigned char buffer2_flag = 0;
uint8_t buffer1[BUFFER_SIZE];
uint8_t buffer2[BUFFER_SIZE];
uint8_t index_buffer1 = 0;
uint8_t index_buffer2 = 0;

// low level

void Uart_StartReceive(UART_HandleTypeDef* huart){
	if(huart->Instance == USART1){
		HAL_UART_Receive_IT(huart, &receive_buffer1, 1);
	}

	if(huart->Instance == USART2){
		HAL_UART_Receive_IT(huart, &receive_buffer2, 1);
	}
}

void Uart_SendByte(UART_HandleTypeDef* huart, uint8_t* byte, uint16_t size){
	HAL_UART_Transmit(huart, byte, size, 100);
}
void Uart_SendString(UART_HandleTypeDef* huart, uint8_t* str){
	HAL_UART_Transmit(huart, str, sizeof(str), 100);
}

// high level

void Uart_InitESP();

HAL_StatusTypeDef Uart_TestESP(){
	Uart_ESPSendByte((void*) '*', 1);
	HAL_Delay(10); // đợi esp gửi lại phản hồi
	if(buffer1_flag){
		turnOnBlue();
		buffer1_flag = 0;
		if(buffer1[index_buffer1 - 1] == '#'){
			// display led
			turnOnGreen();
			HAL_Delay(100);
			// reset led in active mode
			return HAL_OK;
		}
		else{
			//display error
			return HAL_ERROR;
		}
	}
	else{
		turnOnRed();
		//display error
		return HAL_ERROR;
	}
}

void Uart_ESPSendByte(uint8_t* byte, uint16_t size){
	Uart_SendByte(&huart1, byte, 1);
}
void Uart_ESPSendString(uint8_t* str){
	Uart_ESPSendByte(str, sizeof(str));
}

// call back

void HAL_UART_RxCpltCallback(UART_HandleTypeDef* huart){
	if(huart->Instance == USART1){
		buffer1_flag = 1;
		buffer1[index_buffer1++] = receive_buffer1;
		if(index_buffer1 > (BUFFER_SIZE - 1)) index_buffer1 = 0;
		HAL_UART_Receive_IT(huart, &receive_buffer1, 1);
	}

	if(huart->Instance == USART2){
		buffer2_flag = 1;
		buffer2[index_buffer2++] = receive_buffer2;
		if(index_buffer2 > (BUFFER_SIZE - 1)) index_buffer2 = 0;
		HAL_UART_Receive_IT(huart, &receive_buffer2, 1);

	}
}
