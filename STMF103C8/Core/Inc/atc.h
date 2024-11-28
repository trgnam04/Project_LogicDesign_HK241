/*
 * atc.h
 *
 *  Created on: Nov 27, 2024
 *      Author: ASUS
 */

#ifndef INC_ATC_H_
#define INC_ATC_H_

#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include "main.h"
#include "usart.h"

#define ATC_RESP_NOT_FOUND        -4
#define ATC_RESP_TX_BUSY          -3
#define ATC_RESP_SENDING_TIMEOUT  -2
#define ATC_RESP_SENDING_ERROR    -1
#define ATC_OK					   0



#define BUFFER_SIZE 512

typedef struct{
	UART_HandleTypeDef* hUart;
	uint8_t pRxbuff[BUFFER_SIZE];
	uint8_t pReadBuff;
	uint32_t RxIdx;
	uint32_t Size;
} ATC_Handletypedef;

void	ATC_Init(ATC_Handletypedef* hAtc, UART_HandleTypeDef* hUart);
bool	ATC_TxRaw(ATC_Handletypedef* hAtc, const uint8_t* pData, uint16_t len); // transmit dma
bool	ATC_TxBusy(ATC_Handletypedef* hAtc); // check whether uart is busy or not
bool	ATC_TxWait(ATC_Handletypedef* hAtc, uint32_t TIMEOUT); // wait for response data if timeout runout abort transmit
void	ATC_RxFlush(ATC_Handletypedef* hAtc); // clear receive buffer
void	ATC_CheckError(ATC_Handletypedef* hAtc); // check whether uart is error or not if is, reinitialize uart
int		ATC_SendReceive(ATC_Handletypedef* hAtc, const char* pCmd, uint32_t TxTimeout, uint32_t RxTimeout);
bool	ATC_Send(ATC_Handletypedef* hAtc, const char* pCmd, uint32_t TxTimeout);
bool	ATC_Receive(ATC_Handletypedef* hAtc, const char* expectedResponse, uint32_t RxTimeout);


#endif /* INC_ATC_H_ */
