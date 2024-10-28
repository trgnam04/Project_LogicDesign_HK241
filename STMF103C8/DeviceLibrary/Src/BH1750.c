/*
 * BH1750.c
 *
 *  Created on: Oct 17, 2024
 *      Author: ASUS
 */

#include "BH1750.h"

uint8_t buffer[2];

HAL_StatusTypeDef BH1750_Init(I2C_HandleTypeDef* I2C_Name, BH1750_Typedef* BH1750_Name){
	BH1750_Name->I2C_Name = I2C_Name;
	BH1750_Name->Address_r = BH1750_GROUND_ADDR_READ;
	BH1750_Name->Address_w = BH1750_GROUND_ADDR_WRITE;
	BH1750_Name->Mode = CMD_H_RES_MODE2;
	BH1750_Name->Value = 0;

	return HAL_OK;
}

HAL_StatusTypeDef BH1750_SendCommand(BH1750_Typedef* BH1750_Name, uint8_t cmd){
	if(HAL_I2C_Master_Transmit(BH1750_Name->I2C_Name, BH1750_Name->Address_w, &cmd, sizeof(cmd), HAL_MAX_DELAY) != HAL_OK){
		return HAL_ERROR;
	}
	return HAL_OK;
}


HAL_StatusTypeDef BH1750_ReadData(BH1750_Typedef* BH1750_Name){
	// read and covert data
	if(HAL_I2C_Master_Receive(BH1750_Name->I2C_Name, BH1750_Name->Address_r, buffer, 2, HAL_MAX_DELAY) != HAL_OK){
		return HAL_ERROR;
	}
	BH1750_Name->Value = buffer[0];
	BH1750_Name->Value = (BH1750_Name->Value << 8) | buffer[1];

	BH1750_Name->Value /= 1.2;


	return HAL_OK;
}
HAL_StatusTypeDef BH1750_TriggerReceiveData(BH1750_Typedef* BH1750_Name){
	if(BH1750_SendCommand(BH1750_Name, CMD_POWER_ON) != HAL_OK){
		return HAL_ERROR;
	}
	if(BH1750_SendCommand(BH1750_Name, CMD_RESET) != HAL_OK){
		return HAL_ERROR;
	}
	if(BH1750_SendCommand(BH1750_Name, CMD_H_RES_MODE2) != HAL_OK){
		return HAL_ERROR;
	}

	return HAL_OK;
}
HAL_StatusTypeDef BH1750_ReceiveData(BH1750_Typedef* BH1750_Name){
	// include delay
	if(BH1750_TriggerReceiveData(BH1750_Name) != HAL_OK){
		return HAL_ERROR;
	}
	HAL_Delay(180);
	if(BH1750_ReadData(BH1750_Name) != HAL_OK){
		return HAL_ERROR;
	}

	return HAL_OK;
}



