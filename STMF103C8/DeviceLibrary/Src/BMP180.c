/*
 * BMP180.c
 *
 *  Created on: Oct 18, 2024
 *      Author: ASUS
 */

#include "BMP180.h"

// EEPROM VALUEs-----------------------------------------------
int16_t AC1;
int16_t AC2;
int16_t AC3;
int16_t AC4;
int16_t AC5;
int16_t AC6;
int16_t B1;
int16_t B2;
int16_t MB;
int16_t MC;
int16_t MD;
//------------------------------------------------------------
int32_t tX1, tX2, tB5;
int32_t pB6,pX1,pX2,pB3,pB4,pB7,pX3;
int16_t unCompTemp; // raw data
uint32_t unCompPresure; // raw data
float presureATM;
//------------------------------------------------------------

HAL_StatusTypeDef BMP180_Init(I2C_HandleTypeDef* I2C_Name, BMP180_Typedef* BMP180_Name){
	BMP180_Name->Pressure = 0;
	BMP180_Name->PressureATM = 0;
	BMP180_Name->Temperature = 0;
	BMP180_Name->I2C_Name = I2C_Name;

	if(BMP180_Get_Calibration_Values(BMP180_Name) != HAL_OK){
		return HAL_ERROR;
	}


	return HAL_OK;
}

HAL_StatusTypeDef BMP180_Get_Calibration_Values(BMP180_Typedef* BMP180_Name){
	uint8_t calibrationBuff[BMP180_CALIBRATION_VALUE_LENGTH] = {0};
	uint8_t status = HAL_OK;
	status = HAL_I2C_Mem_Read(BMP180_Name->I2C_Name, BMP180_DEVICE_READ_REGISTER_ADDRESS, BMP180_CALIBRATION_START_ADDRESS, 1, calibrationBuff, BMP180_CALIBRATION_VALUE_LENGTH, 10000);

	/*shifting operations*/
	AC1 = calibrationBuff[0]<<8 | calibrationBuff[1]; //8 bit MSB shifting left(15,14,13..), 8 bit LSB stay(7,6,5,..0)
	AC2 = calibrationBuff[2]<<8 | calibrationBuff[3];
	AC3 = calibrationBuff[4]<<8 | calibrationBuff[5];
	AC4 = calibrationBuff[6]<<8 | calibrationBuff[7];
	AC5 = calibrationBuff[8]<<8 | calibrationBuff[9];
	AC6 =calibrationBuff[10]<<8 | calibrationBuff[11];
	B1 = calibrationBuff[12]<<8 | calibrationBuff[13];
	B2 = calibrationBuff[14]<<8 | calibrationBuff[15];
	MB = calibrationBuff[16]<<8 | calibrationBuff[17];
	MC = calibrationBuff[18]<<8 | calibrationBuff[19];
	MD = calibrationBuff[20]<<8 | calibrationBuff[21];

	/*CONTROL CALIBRATION VALUEs COMING FROM to EEPROM, IF THERE IS A PROBLEM, ORANGE LED WILL TURN ON*/
	/*IF ORANGE LED DOESN'T WORK, EVERYTHINK IS OK*/

	if(AC1 == 0x0000 || AC1 == 0xFFFF)
	{
		return HAL_ERROR;
	}

	if(AC2 == 0x0000 || AC2 == 0xFFFF)
	{
		return HAL_ERROR;
	}
	if(AC3 == 0x0000 || AC3 == 0xFFFF)
	{
		return HAL_ERROR;
	}
	if(AC4 == 0x0000 || AC4 == 0xFFFF)
	{
		return HAL_ERROR;
	}
	if(AC5 == 0x0000 || AC5 == 0xFFFF)
	{
		return HAL_ERROR;
	}
	if(AC6 == 0x0000 || AC6 == 0xFFFF)
	{
		return HAL_ERROR;
	}
	if(B1 == 0x0000 || B1 == 0xFFFF)
	{
		return HAL_ERROR;
	}
	if(B2 == 0x0000 || B2 == 0xFFFF)
	{
		return HAL_ERROR;
	}
	if(MB == 0x0000 || MB == 0xFFFF)
	{
		return HAL_ERROR;
	}
	if(MC == 0x0000 || MC == 0xFFFF)
	{
		return HAL_ERROR;
	}
	if(MD == 0x0000 || MD == 0xFFFF)
	{
		return HAL_ERROR;
	}

	return status;

}

HAL_StatusTypeDef BMP180_Get_Uncompansated_Temperature(BMP180_Typedef* BMP180_Name){
	uint8_t wData[1];
	uint8_t rData[2]={0};
	wData[0]=0x2E;
	if(HAL_I2C_Mem_Write(BMP180_Name->I2C_Name, BMP180_DEVICE_WRITE_REGISTER_ADDRESS, 0xF4, 1, wData, 1, 100000) != HAL_OK){
		return HAL_ERROR;
	}
	HAL_Delay(5);
	if(HAL_I2C_Mem_Read(BMP180_Name->I2C_Name, BMP180_DEVICE_READ_REGISTER_ADDRESS, 0xF6, 1, rData, 2, 100000) != HAL_OK){
		return HAL_ERROR;
	}
	unCompTemp = (int16_t)((rData[0]<<8) | rData[1]); //8 bit MSB shifting left(15,14,13..), 8 bit LSB stay(7,6,5,..0)
	return HAL_OK;
}

HAL_StatusTypeDef BMP180_Get_Temperature(BMP180_Typedef* BMP180_Name){
	BMP180_Get_Uncompansated_Temperature(BMP180_Name);
	//DATASHEET CALCULATING

	tX1 = ((int32_t)unCompTemp - (int32_t)AC6) * (int32_t)AC5 / 32768;
	tX2 = ((int32_t)MC * 2048) / (tX1 + (int32_t)MD);

	if(tX1==0 && MD==0) // CONTROL tx1 and md FOR 0
	{
		return HAL_ERROR;
	}


	tB5 = tX1 + tX2;
	BMP180_Name->Temperature = (tB5 + 8) / 16 * 0.1;

	return HAL_OK;
}

HAL_StatusTypeDef BMP180_Get_Uncompansated_Presure(BMP180_Typedef* BMP180_Name){
	uint8_t rData[3]={0};
	uint8_t wData[1];
	wData[0]=0x34 | (0x01<<6); //oversampling_setting is standard, it's mean 0x03 and shifting 6 bit to left
	if(HAL_I2C_Mem_Write(BMP180_Name->I2C_Name, BMP180_DEVICE_WRITE_REGISTER_ADDRESS, 0xF4, 1, wData, 1, 100000) != HAL_OK){
		return HAL_ERROR;
	}
	HAL_Delay(8); // 8 ms delay because we selected standard(oversampling_settings)

	if(HAL_I2C_Mem_Read(BMP180_Name->I2C_Name, BMP180_DEVICE_READ_REGISTER_ADDRESS, 0xF6, 1, rData, 3, 100000) != HAL_OK){
		return HAL_ERROR;
	}
	unCompPresure = (rData[0]<<16 | rData [1]<<8 | rData[2]) >> (8 - (uint8_t)(0x03));
	// shifting operation -> unCompPresure = (MSB<<16 + LSB<<8 + XLSB) >> (8-oversample_settings)
	return HAL_OK;

}

HAL_StatusTypeDef BMP180_Get_Presure(BMP180_Typedef* BMP180_Name){
	if(BMP180_Get_Uncompansated_Presure(BMP180_Name) != HAL_OK){
		return HAL_ERROR;
	}

	// DATASHEET CALCULATING
	pB6 = tB5 - 4000;
	pX1 = (B2 * (pB6 * pB6 / 4096)) / 2048;
	pX2 = AC2 * pB6 / 2048;
	pX3 = pX1 + pX2;
	pB3 = (((AC1 * 4 + pX3) << (uint8_t)0x03) + 2) / 4;
	pX1 = AC3 * pB6 / 8192;
	pX2 = (B1 * (pB6 * pB6 / 4096)) / 65536;
	pX3 = ((pX1 + pX2) + 2) /4;
	pB4 = AC4 * (unsigned long)(pX3 + 32768) / 32768;
	pB7 = ((unsigned long)unCompPresure - pB3) * (50000 >> (uint8_t)0x03);

	if(pB7 < 0x8000000)
		BMP180_Name->Pressure = (pB7 * 2) / pB4;
	else
		BMP180_Name->Pressure = (pB7 / pB4) * 2;

	pX1 = (BMP180_Name->Pressure / 256) * (BMP180_Name->Pressure / 256);
	pX1 = (pX1 * 3038) / 65536;
	pX2 = (-7357 * BMP180_Name->Pressure) / 65537;
	BMP180_Name->Pressure = BMP180_Name->Pressure + (pX1 + pX2 + 3791) / 16; //HECTOPASCAL
	BMP180_Name->PressureATM = BMP180_Name->Pressure * 0.00000986923267; //HECTOPASCAL TO ATM

	return HAL_OK;

}

