/*
 * physics.c
 *
 *  Created on: Oct 24, 2024
 *      Author: ASUS
 */

#include "physics.h"

CLCD_I2C_Name LCD_t;
LED_RGB led_t;

void InitPhysics(I2C_HandleTypeDef* i2c, TIM_HandleTypeDef* tim_rgb){
	led_t.tim = tim_rgb;

	CLCD_I2C_Init(&LCD_t, i2c, 0x4e, 20, 4);

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

void LCD_DisplayPage1(){
	CLCD_I2C_Clear(&LCD_t);
	CLCD_I2C_SetCursor(&LCD_t, 4, 0);
	CLCD_I2C_WriteString(&LCD_t, "WEATHER");
	CLCD_I2C_SetCursor(&LCD_t, 4, 1);
	CLCD_I2C_WriteString(&LCD_t, "STATION");
}

void LCD_DisplayPage2(float temp, float humid){
	char buff[16];
	CLCD_I2C_Clear(&LCD_t);
	sprintf(buff, "Temp :%0.1f", temp);
	CLCD_I2C_SetCursor(&LCD_t, 0, 0);
	CLCD_I2C_WriteString(&LCD_t, buff);
	sprintf(buff, "Humid:%0.1f", humid);
	CLCD_I2C_SetCursor(&LCD_t, 0, 1);
	CLCD_I2C_WriteString(&LCD_t, buff);
}

void LCD_DisplayPage3(float airpr, float lightins){
	char buff[16];
	CLCD_I2C_Clear(&LCD_t);
	sprintf(buff, "Airpr   :%0.1f", airpr);
	CLCD_I2C_SetCursor(&LCD_t, 0, 0);
	CLCD_I2C_WriteString(&LCD_t, buff);
	sprintf(buff, "LightIts:%0.1f", lightins);
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



