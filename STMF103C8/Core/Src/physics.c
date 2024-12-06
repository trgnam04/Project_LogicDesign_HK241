/*
 * physics.c
 *
 *  Created on: Oct 24, 2024
 *      Author: ASUS
 */

#include "physics.h"

char msg[128];

CLCD_I2C_Name LCD_t;
LED_RGB led_t;
BMP180_Typedef BMP180_t;
BH1750_Typedef BH1750_t;
DHT20_Typedef DHT20_t;
unsigned char flagUpdateData = 0;

const char* LightIntensityFeed = "";
const char* TemperatureFeed = "";
const char* AirPressureFeed = "";
const char* HumidityFeed = "";

char LightIntensityStr[10];
char HumidityStr[10];
char TemperatureStr[10];
char AirPressureStr[10];

const char* pSSID = "271104E";
const char* PassWord = "1234567890";
const char* UserID = "TrungNam";
const char* Key = "";
const char* Server = "";
const uint16_t Port = 1883;

static unsigned char flagConnect =  false;
static unsigned char flagConfig =  false;
static unsigned char flagConnectWifi =  false;

void InitPhysics(){
	led_t.tim = &htim1;
	CLCD_I2C_Init(&LCD_t, &hi2c1, 0x4e, 20, 4);
	InitESP();
	init_ledRGB(&htim1);
	init_button();
	BMP180_Init(&hi2c1, &BMP180_t);
	BH1750_Init(&hi2c1, &BH1750_t);
	DHT20_Init(&hi2c1, &DHT20_t);
}

void SendRequestDHT20(void){
	DHT20_SendRequest(&DHT20_t, 0x71);
}

void ReadDHT20(void){
	DHT20_ReadData(&DHT20_t);
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
	if(flagConnectWifi){
		CLCD_I2C_SetCursor(&LCD_t, 0, 0);
		CLCD_I2C_WriteChar(&LCD_t, 'v');
	}
	HAL_Delay(1);
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
			CLCD_I2C_WriteString(&LCD_t, " Esp");
			CLCD_I2C_SetCursor(&LCD_t, 0, 1);
			CLCD_I2C_WriteString(&LCD_t, "Connecting...");
			break;
		case 1:
			CLCD_I2C_Clear(&LCD_t);
			CLCD_I2C_SetCursor(&LCD_t, 0, 0);
			CLCD_I2C_WriteString(&LCD_t, " Esp");
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
		CLCD_I2C_WriteString(&LCD_t, " SEND DATA: ON ");
		break;
	}
	case 1:{
		CLCD_I2C_WriteString(&LCD_t, " SEND DATA: OFF");
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

void LCD_DisplayConnectWifi(void){
	CLCD_I2C_SetCursor(&LCD_t, 0, 0);
	CLCD_I2C_WriteString(&LCD_t, "ESP Connecting");
	CLCD_I2C_SetCursor(&LCD_t, 0, 1);
	CLCD_I2C_WriteString(&LCD_t, "Wifi...");
}

void LCD_DisplayConnectedWifi(void){
	CLCD_I2C_SetCursor(&LCD_t, 0, 0);
	CLCD_I2C_WriteString(&LCD_t, "ESP Connected");
	CLCD_I2C_SetCursor(&LCD_t, 0, 1);
	CLCD_I2C_WriteString(&LCD_t, "Wifi...");
}

void LCD_DisplayConfigServer(void){
	CLCD_I2C_SetCursor(&LCD_t, 0, 0);
	CLCD_I2C_WriteString(&LCD_t, "ESP Config Server");
}

void LCD_DisplayConnectServerBroker(void){
	CLCD_I2C_SetCursor(&LCD_t, 0, 0);
	CLCD_I2C_WriteString(&LCD_t, "Connect Server");
	CLCD_I2C_SetCursor(&LCD_t, 0, 1);
	CLCD_I2C_WriteString(&LCD_t, "Broker");
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
	if(flagUpdateData){
		sprintf(msg, "Humidity:%0.1f;Temperature:%0.1f;AirPressure:%0.1f;LightIntensity:%d\r\n", DHT20_t.Humidity, DHT20_t.Temperature, BMP180_t.Pressure, BH1750_t.Value);
		HAL_UART_Transmit(&huart1, (uint8_t*)msg, sizeof(msg), 10);
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
	int count = 0;
	LCD_DisplayConnectEsp(0);
	turnOnBlue();
	HAL_Delay(1000);
	while(!ESP_Init(&hEsp)){
		turnOnRed();
		HAL_Delay(1000);
		ESP_Init(&hEsp);
	}
	LCD_Clear();
	HAL_Delay(3);
	LCD_DisplayConnectWifi();

	ESP_WifiInit(&hEsp);
	HAL_Delay(100);
	ESP_WifiMode(&hEsp, ESP_WIFIMODE_STATION, 1);
	HAL_Delay(100);
//	if(!ESP_WifiStationIsConnect(&hEsp)){
	while(!ESP_WifiStationConnect(&hEsp, pSSID, PassWord, NULL, 10000)){
		turnOnRed();
		HAL_Delay(5000);
		ESP_WifiStationConnect(&hEsp, pSSID, PassWord, NULL, 10000);
	}
	LCD_Clear();
	HAL_Delay(1);
	LCD_DisplayConnectedWifi();
//	}
	turnOnBlue();
	HAL_Delay(1000);
	LCD_Clear();
	HAL_Delay(1);
	LCD_DisplayConfigServer();
	while(!ESP_MQTTConfig(&hEsp, UserID, Key)){
		turnOnRed();
		if(count == 2) ResetESP32();
		HAL_Delay(5000);
		ESP_MQTTConfig(&hEsp, UserID, Key);
		count++;
	}
	count = 0;
	turnOnBlue();
	HAL_Delay(5000);
	LCD_Clear();
	HAL_Delay(1);
	LCD_DisplayConnectServerBroker();

	while(!ESP_MQTTConnect(&hEsp, Server, Port)){
		if(count == 2) ResetESP32();
		turnOnRed();
		HAL_Delay(10000);
		ESP_MQTTConnect(&hEsp, Server, Port);
		count++;
	}
	turnOnBlue();
	HAL_Delay(5000);

	flagConnect = true;
	flagConfig = true;
	turnOnGreen();
	LCD_DisplayConnectEsp(1);
	HAL_Delay(1000);
	LCD_Clear();
}

unsigned char Publish_Temperature_Task(void){
	if(flagConnect){
		sprintf(TemperatureStr, "%0.1f", DHT20_t.Temperature);
		return ESP_MQTTPublish(&hEsp, TemperatureFeed, TemperatureStr);
	}
	return 0;
}

unsigned char Publish_Humidity_Task(void){
	if(flagConnect){
		sprintf(HumidityStr, "%0.1f", DHT20_t.Humidity);
		return ESP_MQTTPublish(&hEsp, HumidityFeed, HumidityStr);
	}
	return 0;
}

unsigned char Publish_LightIntensity_Task(void){
	if(flagConnect){
		sprintf(LightIntensityStr, "%d", BH1750_t.Value);
		return ESP_MQTTPublish(&hEsp, LightIntensityFeed, LightIntensityStr);
	}
	return 0;
}

unsigned char Publish_AirPressure_Task(void){
	if(flagConnect){
		sprintf(AirPressureStr, "%0.1f", BMP180_t.Pressure);
		return ESP_MQTTPublish(&hEsp, AirPressureFeed, AirPressureStr);
	}
	return 0;
}

void Publish_Task(void){
	switch(flagConnect){
	case 1:{
		do{
			if(!Publish_AirPressure_Task()){
				break;
			}
			if(!Publish_Humidity_Task()){
				break;
			}
			if(!Publish_Temperature_Task()){
				break;
			}
			if(!Publish_LightIntensity_Task()){
				break;
			}

			return;
		}
		while(0);
		flagConnect = 0;
		return;
	}
	case 0:{
		SCH_Add_Task(ResetESP32, 0, 0);
		SCH_Add_Task(Reconncet_Server, 0, 400);
		SCH_Add_Task(Reconnect_MQTTBroker, 0, 1000);
	}
	}

	flagConnect = false;
}

void Check_ServerConnect_Task(void){
	if(!flagConnect){

	}
}

void Reconncet_Server(void){
	if(ESP_MQTTConfig(&hEsp, UserID, Key)){
		flagConfig = true;
	}
}
void Reconnect_MQTTBroker(void){
	if(ESP_MQTTConnect(&hEsp, Server, Port)){
		flagConnect = true;
	}
}

void ResetESP32(void){
	ESP_Restart(&hEsp);
}

