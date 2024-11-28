/*
 * atc.c
 *
 *  Created on: Nov 27, 2024
 *      Author: ASUS
 */


#include "atc.h"


static const char* esp_ok = "OK\r\n";
static const char* esp_error = "ERROR\r\n";
static const char* esp_busy = "busy p...\r\n";



void ATC_Init(ATC_Handletypedef* hAtc, UART_HandleTypeDef* hUart){
	hAtc->RxIdx = 0;
	hAtc->hUart = hUart;
	hAtc->Size = BUFFER_SIZE;
	memset(hAtc->pRxbuff, 0, BUFFER_SIZE);

	// init DMA UART
	HAL_UART_Receive_IT(hAtc->hUart, &hAtc->pReadBuff, 1);
}

bool ATC_TxRaw(ATC_Handletypedef* hAtc, const uint8_t* pData, uint16_t len){
	bool answer = false;
	do
	{
		if (HAL_UART_Transmit_DMA(hAtc->hUart, pData, len) != HAL_OK)
		{
			break;
		}
		answer = true;

	} while (0);

	return answer;
}

bool ATC_TxBusy(ATC_Handletypedef* hAtc){
	if ((HAL_UART_GetState(hAtc->hUart) == HAL_UART_STATE_BUSY_TX) || (HAL_UART_GetState(hAtc->hUart) == HAL_UART_STATE_BUSY_TX_RX))
		return true;
	else
		return false;
}

bool ATC_TxWait(ATC_Handletypedef* hAtc, uint32_t TIMEOUT){
	bool answer = false;
	uint32_t start_time = HAL_GetTick();
	while (1)
	{
		HAL_Delay(1);
		if ((HAL_UART_GetState(hAtc->hUart) == HAL_UART_STATE_BUSY_RX) || (HAL_UART_GetState(hAtc->hUart) == HAL_UART_STATE_READY))
		{
			answer = true;
			break;
		}
		if ((HAL_UART_GetState(hAtc->hUart) == HAL_UART_STATE_ERROR) || (HAL_UART_GetState(hAtc->hUart) == HAL_UART_STATE_TIMEOUT))
		{
			  break;
		}
		if (HAL_GetTick() - start_time >= TIMEOUT)
		{
		  HAL_UART_AbortTransmit(hAtc->hUart);
		  break;
		}
	}

	return answer;
}

void ATC_RxFlush(ATC_Handletypedef* hAtc){
	memset(hAtc->pRxbuff, 0, BUFFER_SIZE);
	hAtc->RxIdx = 0;
}

void ATC_CheckError(ATC_Handletypedef* hAtc){
	return; //for dummy
}

int ATC_SendReceive(ATC_Handletypedef* hAtc, const char* pCmd, uint32_t TxTimeout, uint32_t RxTimeout){
	return 0; // for dummy
}

bool ATC_Send(ATC_Handletypedef* hAtc, const char* pCmd, uint32_t TxTimeout){
	bool answer = false;
	do{
		if (ATC_TxBusy(hAtc) == true)
		{
		  break;
		}
		ATC_CheckError(hAtc);
		ATC_RxFlush(hAtc);
		if (ATC_TxRaw(hAtc, (const uint8_t*)pCmd, strlen((char*)pCmd)) == false)
		{
		  break;
		}
		if (ATC_TxWait(hAtc, TxTimeout) == false)
		{
		  break;
		}

		answer = true;
	}
	while(0);

	return answer;
}

bool ATC_Receive(ATC_Handletypedef* hAtc, const char* expectedResponse, uint32_t RxTimeout){
	bool answer = false;

	uint32_t start_time = HAL_GetTick();
	while (HAL_GetTick() - start_time < RxTimeout) {
	   if(strstr((char*)hAtc->pRxbuff, expectedResponse) != NULL) {
		   answer = true;
		   break;
	   }
	   if(strstr((char*)hAtc->pRxbuff, esp_ok) != NULL) {
		   answer = true;
		   break;
	   }
	   if(strstr((char*)hAtc->pRxbuff, esp_error) != NULL) {
		   break;
	   }
//	   if(strstr((char*)hAtc->pRxbuff, esp_busy) != NULL) {
//		   break;
//	   }
	}
	HAL_UART_Transmit(&huart1, (uint8_t*)hAtc->pRxbuff, hAtc->RxIdx + 1, 1000);

	return answer;
}

