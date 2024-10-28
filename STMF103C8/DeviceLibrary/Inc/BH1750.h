/*
 * BH1750.h
 *
 *  Created on: Oct 17, 2024
 *      Author: ASUS
 */

#ifndef INC_BH1750_H_
#define INC_BH1750_H_

#include "main.h"

#define BH1750_SLAVE_ADDRESS 0x23

// address if sensor set to ground or not
#define BH1750_NO_GROUND_ADDR_WRITE     (0xB9 + 0)
#define BH1750_NO_GROUND_ADDR_READ      (0xB9 + 1)
#define BH1750_GROUND_ADDR_WRITE        (0x46 + 0)
#define BH1750_GROUND_ADDR_READ     (0x46 + 1)

// define cmd
#define CMD_POWER_DOWN          0x00
#define CMD_POWER_ON            0x01
#define CMD_RESET               0x03
#define CMD_H_RES_MODE          0x10
#define CMD_H_RES_MODE2         0x11
#define CMD_L_RES_MODE          0x13
#define CMD_ONE_H_RES_MODE      0x20
#define CMD_ONE_H_RES_MODE2     0x21
#define CMD_ONE_L_RES_MODE      0x23
#define CMD_CNG_TIME_HIGH       0x30    // 3 LSB set time
#define CMD_CNG_TIME_LOW        0x60    // 5 LSB set time


typedef struct {
	uint16_t Value;
	uint8_t  Address_r;
	uint8_t Address_w;
	uint8_t Mode;
	I2C_HandleTypeDef* I2C_Name;
} BH1750_Typedef;


HAL_StatusTypeDef BH1750_SendCommand(BH1750_Typedef* BH1750_Name, uint8_t cmd);
HAL_StatusTypeDef BH1750_Init(I2C_HandleTypeDef* I2C_Name, BH1750_Typedef* BH1750_Name);
HAL_StatusTypeDef BH1750_ReadData(BH1750_Typedef* BH1750_Name); // read and covert data
HAL_StatusTypeDef BH1750_TriggerReceiveData(BH1750_Typedef* BH1750_Name);
HAL_StatusTypeDef BH1750_ReceiveData(BH1750_Typedef* BH1750_Name); // include delay











#endif /* INC_BH1750_H_ */
