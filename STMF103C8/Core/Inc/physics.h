/*
 * physics.h
 *
 *  Created on: Oct 24, 2024
 *      Author: ASUS
 */

#ifndef INC_PHYSICS_H_
#define INC_PHYSICS_H_

#include "main.h"
#include "uart.h"
#include "CLCD_I2C.h"
#include "ledRGB.h"
#include "button.h"
#include "BMP180.h"
#include "BH1750.h"
#include "DHT20.h"
#include "i2c.h"
#include "gpio.h"
#include "tim.h"
#include "esp-at.h"
#include <stdio.h>
#include "scheduler_O(1).h"

extern BMP180_Typedef BMP180_t;
extern BH1750_Typedef BH1750_t;
extern DHT20_Typedef DHT20_t;
extern unsigned char flagUpdateData;

void InitPhysics();

// LED RGB
void turnOffLED(void);
void turnOnGreen(void);
void turnOnBlue(void);
void turnOnRed(void);

// LCD
void LCD_DisplayPage1(void);
void LCD_DisplayPage2(char idx);
void LCD_DisplayPage3(char idx);
void LCD_Clear(void);
void LCD_displaySettingMode(void);
void LCD_displaySettingUart(char idx);
void LCD_displaySettingUnit(void);
void LCD_displaySettingTemp(uint8_t idx);
void LCD_displaySettingPress(uint8_t idx);
void LCD_DisplayConnectEsp(uint8_t state);
void TurnOnCursor(void);
void TurnOffCursor(void);
void SetCursor(int x, int y);
void LCD_putChar(char character);

// SENSOR
void ReadPressure(void);
void SendRequestForLTdata(void);
void ReadLightIntensity(void);

// UART
void Uart_SendData(void);

void Connect_AdafruitServer(void);
void Publish_Task(void);
void Publish_Temperature_Task(void);
void Publish_Humidity_Task(void);
void Publish_LightIntensity_Task(void);
void Publish_AirPressure_Task(void);
void Check_ServerConnect_Task(void);

#endif /* INC_PHYSICS_H_ */
