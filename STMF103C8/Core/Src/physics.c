/*
 * physics.c
 *
 *  Created on: Oct 24, 2024
 *      Author: ASUS
 */

#include "physics.h"

char msg1[128];
char msg2[128];
char msg3[128];
char msg4[128];
char msg5[128] = "------ Start Send Data ------\n";
char msg6[128] = "-----------------------------\n";


CLCD_I2C_Name LCD_t;
LED_RGB led_t;
BMP180_Typedef BMP180_t;
BH1750_Typedef BH1750_t;
DHT20_Typedef DHT20_t;
unsigned char flagUpdateData = 0;

const char* LightIntensityFeed = "TrungNam/feeds/lightintensity";
const char* TemperatureFeed = "TrungNam/feeds/temperature";
const char* AirPressureFeed = "TrungNam/feeds/airpressure";
const char* HumidityFeed = "TrungNam/feeds/humidity";

char LightIntensityStr[10];
char HumidityStr[10];
char TemperatureStr[10];
char AirPressureStr[10];

const char* pSSID = "271104E";
const char* PassWord = "1234567890";
const char* UserID = "TrungNam";
//const char* Key = "aio_SYkH32u5VJE2tSuf8UDveyVNiOdh";
//const char* Server = "io.adafruit.com";
const uint16_t Port = 1883;

static unsigned char flagConnect =  false;

void InitPhysics(){
	led_t.tim = &htim1;
	CLCD_I2C_Init(&LCD_t, &hi2c1, 0x4e, 20, 4);
	init_ledRGB(&htim1);
	init_button();
	BMP180_Init(&hi2c1, &BMP180_t);
	BH1750_Init(&hi2c1, &BH1750_t);
	DHT20_Init(&hi2c1, &DHT20_t);
}

void turnOffLED(void){
	set_codeRGB(led_t.tim, 0x000000);
}

void turnOnGreen(void){
	set_codeRGB(led_t.tim, 0x00FF00);
}

void turnOnBlue(void){
	set_codeRGB(led_t.tim, 0x0000FF);
}

void turnOnRed(void){
	set_codeRGB(led_t.tim, 0xFF0000);
}

void LCD_DisplaySetting(void){
	CLCD_I2C_SetCursor(&LCD_t, 0, 0);
	CLCD_I2C_WriteString(&LCD_t, "1.TimeOut");
	CLCD_I2C_SetCursor(&LCD_t, 0, 1);
	CLCD_I2C_WriteString(&LCD_t, "2.Send Data");
}

void LCD_DisplayPage1(){
	CLCD_I2C_SetCursor(&LCD_t, 4, 0);
	CLCD_I2C_WriteString(&LCD_t, "WEATHER");
	CLCD_I2C_SetCursor(&LCD_t, 4, 1);
	CLCD_I2C_WriteString(&LCD_t, "STATION");
}

void LCD_DisplayPage2(char idx){
	LCD_Clear();
	HAL_Delay(1);
	char buff[16];
	float tempF = (DHT20_t.Temperature * 1.8) + 32;


	if(idx){
		sprintf(buff, "Temp :%0.1fF", tempF);
	}
	else{
		sprintf(buff, "Temp :%0.1fC", DHT20_t.Temperature);
	}
	CLCD_I2C_SetCursor(&LCD_t, 0, 0);
	CLCD_I2C_WriteString(&LCD_t, buff);

	sprintf(buff, "Humid:%0.1f%%", DHT20_t.Humidity);
	CLCD_I2C_SetCursor(&LCD_t, 0, 1);
	CLCD_I2C_WriteString(&LCD_t, buff);
}

void LCD_Clear(void){
	CLCD_I2C_Clear(&LCD_t);
}

void LCD_DisplayPage3(char idx){
	LCD_Clear();
	char buff[16];
	HAL_Delay(1);
	if(idx){
		sprintf(buff, "Airpr :%0.1fAtm", BMP180_t.PressureATM);
	}
	else{
		sprintf(buff, "Airpr:%0.1fhPa", BMP180_t.Pressure);
	}
	CLCD_I2C_SetCursor(&LCD_t, 0, 0);
	CLCD_I2C_WriteString(&LCD_t, buff);

	sprintf(buff, "LightIts:%dLux", BH1750_t.Value);
	CLCD_I2C_SetCursor(&LCD_t, 0, 1);
	CLCD_I2C_WriteString(&LCD_t, buff);
}

void LCD_DisplayConnectEsp(uint8_t state){
	switch (state) {
		case 0:
			CLCD_I2C_Clear(&LCD_t);
			CLCD_I2C_SetCursor(&LCD_t, 0, 0);
			CLCD_I2C_WriteString(&LCD_t, "Esp");
			CLCD_I2C_SetCursor(&LCD_t, 0, 1);
			CLCD_I2C_WriteString(&LCD_t, "Connecting...");
			break;
		case 1:
			CLCD_I2C_Clear(&LCD_t);
			CLCD_I2C_SetCursor(&LCD_t, 0, 0);
			CLCD_I2C_WriteString(&LCD_t, "Esp");
			CLCD_I2C_SetCursor(&LCD_t, 0, 1);
			CLCD_I2C_WriteString(&LCD_t, "Connected");

			break;
	}
}

void LCD_displaySettingMode(void){
	char buff1[] = "SETTING|  1.UART";
	char buff2[] = "MODE   |  2.UNIT";
	CLCD_I2C_SetCursor(&LCD_t, 0, 0);
	CLCD_I2C_WriteString(&LCD_t, buff1);
	CLCD_I2C_SetCursor(&LCD_t, 0, 1);
	CLCD_I2C_WriteString(&LCD_t, buff2);
}


void LCD_displaySettingUart(char idx){
	CLCD_I2C_SetCursor(&LCD_t, 0, 0);
	switch(idx){
	case 0:{
		CLCD_I2C_WriteString(&LCD_t, "SEND DATA: ON ");
		break;
	}
	case 1:{
		CLCD_I2C_WriteString(&LCD_t, "SEND DATA: OFF");
		break;
	}
	}
}

void LCD_displaySettingUnit(void){
	CLCD_I2C_SetCursor(&LCD_t, 0, 0);
	CLCD_I2C_WriteString(&LCD_t, "  Temperature:");
	CLCD_I2C_SetCursor(&LCD_t, 0, 1);
	CLCD_I2C_WriteString(&LCD_t, " AirPressure:");
}

void LCD_displaySettingTemp(uint8_t idx){
	switch(idx){
	case 0:{
		CLCD_I2C_SetCursor(&LCD_t, 14, 0);
		CLCD_I2C_WriteString(&LCD_t, "C");
		break;
	}
	case 1:{
		CLCD_I2C_SetCursor(&LCD_t, 14, 0);
		CLCD_I2C_WriteString(&LCD_t, "F");
		break;
	}
	}
}

void LCD_displaySettingPress(uint8_t idx){
	switch(idx){
	case 0:{
		CLCD_I2C_SetCursor(&LCD_t, 13, 1);
		CLCD_I2C_WriteString(&LCD_t, "hPa");
		break;
	}
	case 1:{
		CLCD_I2C_SetCursor(&LCD_t, 13, 1);
		CLCD_I2C_WriteString(&LCD_t, "Atm");
		break;
	}
	}
}

void LCD_putChar(char character){
	CLCD_I2C_WriteChar(&LCD_t, character);
}

void SetCursor(int x, int y){
	CLCD_I2C_SetCursor(&LCD_t, x, y);
}



void ReadPressure(void){
	BMP180_Get_Presure(&BMP180_t);
}

void SendRequestForLTdata(void){
	BH1750_TriggerReceiveData(&BH1750_t);
}

void ReadLightIntensity(void){
	BH1750_ReadData(&BH1750_t);
}


void Uart_SendData(void){
	HAL_GPIO_TogglePin(task2_GPIO_Port, task2_Pin);
	if(flagUpdateData){
		sprintf(msg1, "Humidity       : %0.1f\n", DHT20_t.Humidity);
		sprintf(msg2, "Temperature    : %0.1f\n", DHT20_t.Temperature);
		sprintf(msg3, "Air Pressure   : %0.1f\n", BMP180_t.Pressure);
		sprintf(msg4, "Light Intensity: %d\n", BH1750_t.Value);

		HAL_UART_Transmit(&huart1, (uint8_t*)msg5, sizeof(msg5), 10);
		HAL_UART_Transmit(&huart1, (uint8_t*)msg1, sizeof(msg1), 10);
		HAL_UART_Transmit(&huart1, (uint8_t*)msg2, sizeof(msg2), 10);
		HAL_UART_Transmit(&huart1, (uint8_t*)msg3, sizeof(msg3), 10);
		HAL_UART_Transmit(&huart1, (uint8_t*)msg4, sizeof(msg4), 10);
		HAL_UART_Transmit(&huart1, (uint8_t*)msg6, sizeof(msg6), 10);
	}
}

void TurnOnCursor(void){
	CLCD_I2C_CursorOn(&LCD_t);
	CLCD_I2C_BlinkOn(&LCD_t);
}

void TurnOffCursor(void){
	CLCD_I2C_CursorOff(&LCD_t);
}


// AT API

void Connect_AdafruitServer(void){
//	turnOnBlue();

	while(!ESP_Init(&hEsp)){
//		turnOnRed();
		HAL_Delay(1000);
		ESP_Init(&hEsp);
	}

//	turnOnBlue();

	ESP_WifiInit(&hEsp);
	ESP_WifiMode(&hEsp, ESP_WIFIMODE_STATION, 0);
	while(!ESP_WifiStationConnect(&hEsp, pSSID, PassWord, NULL, 10000)){
//		turnOnRed();
		HAL_Delay(1000);
		ESP_WifiStationConnect(&hEsp, pSSID, PassWord, NULL, 10000);
	}

//	turnOnBlue();

	while(!ESP_MQTTConfig(&hEsp, UserID, Key)){
//		turnOnRed();
		HAL_Delay(3000);
		ESP_MQTTConfig(&hEsp, UserID, Key);
	}

//	turnOnBlue();

	while(!ESP_MQTTConnect(&hEsp, Server, Port)){
//		turnOnRed();
		HAL_Delay(5000);
		ESP_MQTTConnect(&hEsp, Server, Port);
	}

	flagConnect = true;

//	turnOnGreen();

}

void Publish_Temperature_Task(void){
	if(flagConnect){
		sprintf(TemperatureStr, "%0.1f", DHT20_t.Temperature);
		ESP_MQTTPublish(&hEsp, TemperatureFeed, TemperatureStr);
	}
}

void Publish_Humidity_Task(void){
	if(flagConnect){
		sprintf(HumidityStr, "%0.1f", DHT20_t.Humidity);
		ESP_MQTTPublish(&hEsp, HumidityFeed, HumidityStr);
	}


}

void Publish_LightIntensity_Task(void){
	if(flagConnect){
		sprintf(LightIntensityStr, "%d", BH1750_t.Value);
		ESP_MQTTPublish(&hEsp, LightIntensityFeed, LightIntensityStr);
	}

}

void Publish_AirPressure_Task(void){
	if(flagConnect){
		sprintf(AirPressureStr, "%0.1f", BMP180_t.Pressure);
		ESP_MQTTPublish(&hEsp, AirPressureFeed, AirPressureStr);
	}
}

void Publish_Task(void){
	Publish_AirPressure_Task();
//	Publish_Humidity_Task();
//	Publish_AirPressure_Task();
//	Publish_LightIntensity_Task();

	SCH_Add_Task(Publish_Humidity_Task, 0, 5);
	SCH_Add_Task(Publish_AirPressure_Task, 0, 10);
	SCH_Add_Task(Publish_LightIntensity_Task, 0, 15);
}

void Check_ServerConnect_Task(void){
	return; // for dummy
}


