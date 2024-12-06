/*
 * esp32-at.h
 *
 *  Created on: Nov 25, 2024
 *      Author: ASUS
 */

#ifndef INC_ESP_AT_H_
#define INC_ESP_AT_H_

#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include "main.h"
#include "usart.h"
#include "atc.h"

#define ESP_ENABLE_AUTOCONNECT	   1
#define ESP_DISABLE_AUTOCONNECT    0



typedef struct{
	ATC_Handletypedef	hAtc;
} ESP_HandleTypeDef;

typedef enum
{
  ESP_WIFIMODE_DISABLE          = 0,
  ESP_WIFIMODE_STATION,
  ESP_WIFIMODE_SOFTAP,
  ESP_WIFIMODE_SOFTAP_STATION

} ESP_WifiModeTypeDef;

extern ESP_HandleTypeDef hEsp;
//extern ATC_Handletypedef hAtc;

void InitESP(void);

// ESP API
bool	ESP_Init(ESP_HandleTypeDef* hEsp);
bool	ESP_Restart(ESP_HandleTypeDef* hEsp);
bool	ESP_FactoryReset(ESP_HandleTypeDef* hEsp);
bool	ESP_GetVer(ESP_HandleTypeDef* hEsp, char* pStr);

bool	ESP_WifiInit(ESP_HandleTypeDef* hEsp);
bool	ESP_WifiDeInit(ESP_HandleTypeDef* hEsp);
bool	ESP_WifiMode(ESP_HandleTypeDef* hEsp, ESP_WifiModeTypeDef WifiMode, bool StationAutoConnect);
bool	ESP_WifiStationConnect(ESP_HandleTypeDef* hEsp, const char* pSSID, const char* pPass, const char* pMac, uint16_t Timeout);
bool	ESP_WifiStationDisconnect(ESP_HandleTypeDef* hEsp);
bool	ESP_WifiStationIsConnect(ESP_HandleTypeDef* hEsp);

// MQTT
bool	ESP_MQTTConfig(ESP_HandleTypeDef* hEsp, const char* Username, const char* Key);
bool	ESP_MQTTConnect(ESP_HandleTypeDef* hEsp, const char* Server, uint16_t Port);
bool	ESP_MQTTPublish(ESP_HandleTypeDef* hEsp, const char* Topic, const char* Message);
bool	ESP_MQTTSubscribe(ESP_HandleTypeDef* hEsp, const char* Topic);
bool	ESP_MQTTReceive(ESP_HandleTypeDef* hEsp, const char* Topic, char* Message);
bool	ESP_MQTTDisconnect(ESP_HandleTypeDef* hEsp);
bool	ESP_MQTTIsConnected(ESP_HandleTypeDef* hEsp);
//bool	ESP_ReconnectServer(ESP_HandleTypeDef* hEsp);
//bool	ESP_ReconnectMQTTBroker(ESP_HandleTypeDef* hEsp);

void	ESP_Callback(UART_HandleTypeDef* huart);

#endif /* INC_ESP_AT_H_ */
