/*
 * BMP180.h
 *
 *  Created on: Oct 18, 2024
 *      Author: ASUS
 */

#ifndef INC_BMP180_H_
#define INC_BMP180_H_

#include "main.h"

//BMP180 DEVICE ADDRESS
#define BMP180_DEVICE_WRITE_REGISTER_ADDRESS 0xEE
#define BMP180_DEVICE_READ_REGISTER_ADDRESS 0xEF
//-----------------------------------------------------------
//BMP180 CALIBRATION VALUE LENGTH
#define BMP180_CALIBRATION_VALUE_LENGTH 22 /*11x2, 8 bit per word, but we have 16 bits*/
//-------------------------------------------------------------
//BMP180 CALIBRATION START ADDRESS
#define BMP180_CALIBRATION_START_ADDRESS 0xAA
//------------------------------------------------------
//OBJECT
typedef struct{
	float Temperature;
	float Pressure;
	float PressureATM;
	I2C_HandleTypeDef* I2C_Name;
} BMP180_Typedef;
//FUNCTION
HAL_StatusTypeDef BMP180_Init(I2C_HandleTypeDef* I2C_Name, BMP180_Typedef* BMP180_Name);
HAL_StatusTypeDef BMP180_Get_Calibration_Values(BMP180_Typedef* BMP180_Name);
HAL_StatusTypeDef BMP180_Get_Uncompansated_Temperature(BMP180_Typedef* BMP180_Name);
HAL_StatusTypeDef BMP180_Get_Temperature(BMP180_Typedef* BMP180_Name);
HAL_StatusTypeDef BMP180_Get_Uncompansated_Presure(BMP180_Typedef* BMP180_Name);
HAL_StatusTypeDef BMP180_Get_Presure(BMP180_Typedef* BMP180_Name);



#endif /* INC_BMP180_H_ */
