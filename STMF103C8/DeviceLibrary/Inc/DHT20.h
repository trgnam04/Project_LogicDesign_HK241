/*
 * DHT20.h
 *
 *  Created on: Oct 2, 2024
 *      Author: ASUS
 */

#ifndef INC_DHT20_H_
#define INC_DHT20_H_

#include "main.h"

#define DHT20_SLAVE_ADDRESS 0x38
#define INIT_TIME 100

typedef struct {
	float Humidity;
	float Temperature;
	I2C_HandleTypeDef* I2C_Name;
} DHT20_Typedef;

typedef enum {
	DHT20_OK = 0x00u,
	DHT20_ERROR_INIT = 0x01u,
	DHT20_ERROR_CRC = 0x02u,
	DHT20_BUSY = 0x03u
} Status_Trigger_DHT20;


void DHT20_Init(I2C_HandleTypeDef* i2c, DHT20_Typedef* DHT_Name); // gán giá trị và delay 100ms cho sensor nhận tín hiệu
unsigned char DHT20_CheckCRC8(DHT20_Typedef* DHT_Name ,unsigned char* pData, unsigned char length);
uint8_t DHT20_ReadRegister(DHT20_Typedef* DHT_Name, uint8_t pRegister);
Status_Trigger_DHT20 DHT20_ReceiveData(DHT20_Typedef* DHT_Name); // change humid data and temp data
uint8_t DHT20_SendRequest(DHT20_Typedef* DHT_Name, uint8_t pRegister);
uint8_t DHT20_ReadData(DHT20_Typedef* DHT_Name);




#endif /* INC_DHT20_H_ */
