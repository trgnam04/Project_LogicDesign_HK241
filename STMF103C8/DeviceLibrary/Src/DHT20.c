/*
 * DHT20.c
 *
 *  Created on: Oct 2, 2024
 *      Author: ASUS
 */

#include "DHT20.h"

void DHT20_Init(I2C_HandleTypeDef* i2c, DHT20_Typedef* DHT_Name){
	HAL_Delay(100);
	DHT_Name->Humidity = 0;
	DHT_Name->Temperature = 0;
	DHT_Name->I2C_Name = i2c;
}

unsigned char DHT20_CheckCRC8(DHT20_Typedef* DHT_Name ,unsigned char* pData, unsigned char length){
	unsigned char crc = 0xff;
	for(int i = 0; i < length; i++){
		crc = crc ^ (*pData);
		for(int j = 0; j < 8; j++){
			if(crc & 0x80)
				crc = (crc << 1) ^ 0x31;
			else
				crc <<= 1;
		}
		pData++;
	}
	return crc;
}

uint8_t DHT20_ReadRegister(DHT20_Typedef* DHT_Name, uint8_t pRegister){ // Request for read data
		HAL_StatusTypeDef status = HAL_OK;
		uint8_t value = 0;
		status = HAL_I2C_Mem_Read(DHT_Name->I2C_Name, DHT20_SLAVE_ADDRESS << 1, (uint16_t)pRegister, I2C_MEMADD_SIZE_8BIT, &value, 1, 100);

		if(status != HAL_OK){
			// implement handler
		}
		// else implement handler

		return value;
}

Status_Trigger_DHT20 DHT20_ReceiveData(DHT20_Typedef* DHT_Name){
	Status_Trigger_DHT20 status = DHT20_OK;

//	HAL_Delay(100); // thời gian cần ổn định sau khi bật nguồn, scl sẽ ở high trong suốt quá trình này
	uint8_t status_init = DHT20_ReadRegister(DHT_Name, 0x71);
	if((status_init & 0x18) == 0x18){
		// Trigger measurement
		HAL_Delay(10);
		uint8_t data_t[3];
		data_t[0] = 0xAC;
		data_t[1] = 0x33;
	    data_t[2] = 0x00;
	    HAL_I2C_Master_Transmit(DHT_Name->I2C_Name, DHT20_SLAVE_ADDRESS << 1, (uint8_t *)data_t, 3, 100);



	    HAL_Delay(80);

	    // Data is ready to read
	    uint8_t buffer[7];
	    uint32_t data_read = 0;

	    HAL_I2C_Master_Receive(DHT_Name->I2C_Name, DHT20_SLAVE_ADDRESS << 1, buffer, 7, 100);

	    if((buffer[0] & 0x80) == 0x00){
	    	if(DHT20_CheckCRC8(DHT_Name, &buffer[0], 6)){
	    		data_read = buffer[1];
	    		data_read = data_read << 8;
	    		data_read += buffer[2];
	    		data_read = data_read << 8;
	    		data_read += buffer[3];
	    		data_read = data_read >> 4;

	    		DHT_Name->Humidity = (float)data_read * 100 / 1048576;

	    		data_read = buffer[3] & 0x0f;
	    		data_read = data_read << 8;
	    		data_read += buffer[4];
	    		data_read = data_read << 8;
	    		data_read += buffer[5];

	    		DHT_Name->Temperature = ((float)data_read * 200 / 1048576) - 50;
	    	}
	    	else{
	    		status = DHT20_ERROR_CRC;
	    	}
	    }
	    else{
	    	status = DHT20_BUSY;
	    }

	}
	else{
		status = DHT20_ERROR_INIT;
	}

	return status;
}

Status_Trigger_DHT20 DHT20_SendRequest(DHT20_Typedef* DHT_Name, uint8_t pRegister){
	HAL_StatusTypeDef status_init = HAL_OK;
	uint8_t value = 0;
	status_init = HAL_I2C_Mem_Read(DHT_Name->I2C_Name, DHT20_SLAVE_ADDRESS << 1, (uint16_t)pRegister, I2C_MEMADD_SIZE_8BIT, &value, 1, 100);
	if(status_init != HAL_OK){
		return DHT20_ERROR_INIT;
	}
	Status_Trigger_DHT20 status = DHT20_OK;
	if((value & 0x18) == 0x18){
		HAL_Delay(10);
		uint8_t data_t[3];
		data_t[0] = 0xAC;
		data_t[1] = 0x33;
		data_t[2] = 0x00;
		HAL_I2C_Master_Transmit(DHT_Name->I2C_Name, DHT20_SLAVE_ADDRESS << 1, (uint8_t *)data_t, 3, 100);
	}
	else{
		status = DHT20_ERROR_INIT;
	}

	return status;
}
Status_Trigger_DHT20 DHT20_ReadData(DHT20_Typedef* DHT_Name){
	Status_Trigger_DHT20 status = DHT20_OK;
	uint8_t buffer[7];
	uint32_t data_read = 0;

	HAL_I2C_Master_Receive(DHT_Name->I2C_Name, DHT20_SLAVE_ADDRESS << 1, buffer, 7, 100);

	if((buffer[0] & 0x80) == 0x00){
		if(DHT20_CheckCRC8(DHT_Name, &buffer[0], 6)){
			data_read = buffer[1];
			data_read = data_read << 8;
			data_read += buffer[2];
			data_read = data_read << 8;
			data_read += buffer[3];
			data_read = data_read >> 4;

			DHT_Name->Humidity = (float)data_read * 100 / 1048576;

			data_read = buffer[3] & 0x0f;
			data_read = data_read << 8;
			data_read += buffer[4];
			data_read = data_read << 8;
			data_read += buffer[5];

			DHT_Name->Temperature = ((float)data_read * 200 / 1048576) - 50;
		}
		else{
			status = DHT20_ERROR_CRC;
		}
	}
	else{
		status = DHT20_BUSY;
	}

	return status;

}






