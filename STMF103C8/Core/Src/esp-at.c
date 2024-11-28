/*
 * esp-at.c
 *
 *  Created on: Nov 25, 2024
 *      Author: ASUS
 */


#include "esp-at.h"


static const char* esp_ok = "OK\r\n";
//static const char* esp_error = "ERROR\r\n";
//static const char* esp_busy = "busy p...\r\n";


//ATC_Handletypedef hAtc;
ESP_HandleTypeDef hEsp;

// UART timeout (ms)
#define UART_TIMEOUT 1000

void InitESP(void){
	ATC_Init(&hEsp.hAtc, &huart3);
}


// ESP
bool ESP_Init(ESP_HandleTypeDef* hEsp){
	bool answer = false;
	do{
		if(!ATC_Send(&hEsp->hAtc, "AT\r\n", 100)){

			break;
		}
		if(!ATC_Receive(&hEsp->hAtc, esp_ok, 100)){

			break;
		}

		answer = true;
	}
	while(0);

	// debug log if needed

	return answer;
}

bool ESP_Restart(ESP_HandleTypeDef* hEsp){
	bool answer = false;
	do{
		if(!ATC_Send(&hEsp->hAtc, "AT+RST\r\n", 100)){
			break;
		}
		if(!ATC_Receive(&hEsp->hAtc, esp_ok, 100)){
			break;
		}
		answer = true;
	}
	while(0);

	// debug log if needed

	return answer;
}

bool ESP_FactoryReset(ESP_HandleTypeDef* hEsp){
	bool answer = false;
	do{
		if(!ATC_Send(&hEsp->hAtc, "AT+RESTORE\r\n", 100)){
			break;
		}
		if(!ATC_Receive(&hEsp->hAtc, esp_ok, 100)){
			break;
		}
		answer = true;
	}
	while(0);

	// debug log if needed

	return answer;
}

bool ESP_GetVer(ESP_HandleTypeDef* hEsp, char* pStr){
	bool answer = false;
	do
	{
		if(!ATC_Send(&hEsp->hAtc, "AT+GMR\r\n", 100)){
			break;
		}
		if(!ATC_Receive(&hEsp->hAtc, esp_ok, 100)){
			break;
		}
		if(pStr == NULL) break;

		char str[10] = {0};
		if(hEsp->hAtc.pRxbuff != NULL)
		{
		  sscanf((char*)hEsp->hAtc.pRxbuff, "AT version:%11[^\r\n(]", str);
		}
		for(int i = 0; i < 10; i++){
			pStr[i] = str[i];
		}
		answer = true;

	} while (0);

	return answer;
}

bool ESP_WifiInit(ESP_HandleTypeDef* hEsp){
	bool answer = false;
	do{
		if(!ATC_Send(&hEsp->hAtc, "AT+CWINIT=1\r\n", 100)){
			break;
		}
		if(!ATC_Receive(&hEsp->hAtc, esp_ok, 100)){
			break;
		}
		answer = true;
	}
	while(0);


	return answer;
}

bool ESP_WifiDeInit(ESP_HandleTypeDef* hEsp){
	bool answer = false;
	do{
		if(!ATC_Send(&hEsp->hAtc, "AT+CWINIT=0\r\n", 100)){
			break;
		}
		if(!ATC_Receive(&hEsp->hAtc, esp_ok, 100)){
			break;
		}
		answer = true;
	}
	while(0);

	return answer;
}

bool ESP_WifiStationConnect(ESP_HandleTypeDef* hEsp, const char* pSSID, const char* pPass, const char* pMac, uint16_t Timeout){
	bool answer = false;

	char str_at[256];
	memset(str_at, 0, sizeof(str_at));
	do
	{
		sprintf(str_at, "AT+CWJAP=\"%s\",\"%s\"", pSSID, pPass);
//		if (pMac != NULL) // if needed
//		{
//			strcat(str_at, ",\"");
//			strcat(str_at, pMac);
//			strcat(str_at, "\",");
//			strcat(str_at, ",,,,");
////			sprintf(str_timeout, "%d", Timeout);
//			strcat(str_at, "10000");
//		}
		strcat(str_at, "\r\n");
		if(!ATC_Send(&hEsp->hAtc, str_at, 1000)){
			break;
		}
		if(!ATC_Receive(&hEsp->hAtc, esp_ok, 1000)){
			break;
		}
		answer = true;
	} while (0);

	return answer;
}

bool ESP_WifiStationDisconnect(ESP_HandleTypeDef* hEsp){
	bool answer = false;
	do{
		if(!ATC_Send(&hEsp->hAtc, "AT+CWQAP\r\n", 100)){
			break;
		}
		if(!ATC_Receive(&hEsp->hAtc, esp_ok, 100)){
			break;
		}
		answer = true;
	}
	while(0);

	return answer;
}

bool ESP_WifiMode(ESP_HandleTypeDef* hEsp, ESP_WifiModeTypeDef WifiMode, bool StationAutoConnect){
	bool answer = false;
	char str_at[256];
	memset(str_at, 0, sizeof(str_at));
	do
	{
		sprintf(str_at, "AT+CWMODE=%d,%d\r\n", WifiMode, StationAutoConnect);
		if (!ATC_Send(&hEsp->hAtc, str_at, 100))
		{
		  break;
		}
		if (!ATC_Receive(&hEsp->hAtc, esp_ok, 100))
		{
		  break;
		}
		answer = true;

	} while (0);

	return answer;
}


// MQTT
bool ESP_MQTTConfig(ESP_HandleTypeDef* hEsp, const char* Username, const char* Key){
	bool answer = false;
	char cmd[256];
	snprintf(cmd, sizeof(cmd), "AT+MQTTUSERCFG=0,1,\"\",\"%s\",\"%s\",0,0,\"\"\r\n", Username, Key);
	do
	{
		if (!ATC_Send(&hEsp->hAtc, cmd, 1000))
		{
		  break;
		}
		if (!ATC_Receive(&hEsp->hAtc, esp_ok, 1000))
		{
		  break;
		}


		answer = true;

	} while (0);

	return answer;
}

bool ESP_MQTTConnect(ESP_HandleTypeDef* hEsp, const char* Server, uint16_t Port){
	bool answer = false;
	char cmd[256];
	snprintf(cmd, sizeof(cmd), "AT+MQTTCONN=0,\"%s\",%d,0\r\n", Server, Port);
	do
	{
		if (!ATC_Send(&hEsp->hAtc, cmd, 1000))
		{
		  break;
		}
		if (!ATC_Receive(&hEsp->hAtc, esp_ok, 1000))
		{
		  break;
		}


		answer = true;

	} while (0);

	return answer;
}

bool ESP_MQTTPublish(ESP_HandleTypeDef* hEsp, const char* Topic, const char* Message){
	bool answer = false;
	char cmd[256];
	snprintf(cmd, sizeof(cmd), "AT+MQTTPUB=0,\"%s\",\"%s\",0,0\r\n", Topic, Message);
	do{
		if (!ATC_Send(&hEsp->hAtc, cmd, 100))
		{
		  break;
		}
		if (!ATC_Receive(&hEsp->hAtc, esp_ok, 100))
		{
		  break;
		}
	}
	while(0);
	return answer;
}

bool ESP_MQTTSubscribe(ESP_HandleTypeDef* hEsp, const char* Topic){
	bool answer = false;
	char cmd[256];
	snprintf(cmd, sizeof(cmd), "AT+MQTTSUB=0,\"%s\",0\r\n", Topic);
	do{
		if (!ATC_Send(&hEsp->hAtc, cmd, 100))
		{
		  break;
		}
		if (!ATC_Receive(&hEsp->hAtc, esp_ok, 100))
		{
		  break;
		}
	}
	while(0);
	return answer;
}

bool ESP_MQTTReceive(ESP_HandleTypeDef* hEsp, const char* Topic, char* Message){
	return true; // for dummy
}

bool ESP_MQTTDisconnect(ESP_HandleTypeDef* hEsp){
	bool answer = false;
	do{
		if(!ATC_Send(&hEsp->hAtc, "AT+MQTTCLEAN=0\r\n", 100)){
			break;
		}
		if(!ATC_Receive(&hEsp->hAtc, esp_ok, 100)){
			break;
		}
		answer = true;
	}
	while(0);

	return answer;
}


void ESP_Callback(UART_HandleTypeDef* huart){
	if(hEsp.hAtc.hUart == huart){
		hEsp.hAtc.pRxbuff[hEsp.hAtc.RxIdx++] = hEsp.hAtc.pReadBuff;
		if(hEsp.hAtc.pReadBuff > (BUFFER_SIZE - 1)) hEsp.hAtc.pReadBuff = 0;
		HAL_UART_Receive_IT(huart, &hEsp.hAtc.pReadBuff, 1);
	}
}

bool ESP_MQTTIsConnected(ESP_HandleTypeDef* hEsp){
	bool answer = false;
	do{
		if (!ATC_Send(&hEsp->hAtc, "AT+MQTTSTATUS?\r\n", 1000)) {
			break;
		}

		if (!ATC_Receive(&hEsp->hAtc, "+MQTTSTATUS:0,3", 1000)) {
			break;
		}

		answer = true;
	}
	while(0);


	return answer;

}

//void ATC_IdleLineCallback(ATC_Handletypedef* hAtc, uint16_t Len){ // put it in callback
//	if (Len > hAtc->Size - hAtc->RxIdx)
//	{
//		Len = hAtc->Size - hAtc->RxIdx;
//	}
//	memcpy(&hAtc->pReadBuff[hAtc->RxIdx], hAtc->pRxbuff, Len);
//	HAL_UART_Transmit(&huart3, (uint8_t*)hAtc->pReadBuff, Len, 1000);
//	hAtc->RxIdx += Len;
//	if (HAL_UARTEx_ReceiveToIdle_DMA(hAtc->hUart, hAtc->pRxbuff, hAtc->Size) == HAL_OK)
//	{
////		__HAL_DMA_DISABLE_IT(hAtc->hUart->hdmarx, DMA_IT_HT);
//	}
//	else
//	{
//		HAL_UART_AbortReceive(hAtc->hUart);
//		HAL_UARTEx_ReceiveToIdle_DMA(hAtc->hUart, hAtc->pRxbuff, hAtc->Size);
//		__HAL_DMA_DISABLE_IT(hAtc->hUart->hdmarx, DMA_IT_HT);
//	}
//}
//
//void HAL_UARTEx_RxEventCallback(UART_HandleTypeDef *huart, uint16_t Size)
//{
//	HAL_GPIO_TogglePin(Debug_Led_GPIO_Port, Debug_Led_Pin);
//	if(huart == hAtc.hUart){
//
//		ATC_IdleLineCallback(&hAtc, Size);
//	}
//
//}
//
//
//
