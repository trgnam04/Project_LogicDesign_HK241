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
#include <stdio.h>

void InitPhysics(I2C_HandleTypeDef* i2c, TIM_HandleTypeDef* tim_rgb);

// LED RGB
void turnOffLED(void);
void turnOnGreen(void);
void turnOnBlue(void);
void turnOnRed(void);

// LCD
void LCD_DisplayPage1(void);
void LCD_DisplayPage2(float temp, float humid);
void LCD_DisplayPage3(float airpr, float lightins);
void LCD_DisplayConnectEsp(uint8_t state);







#endif /* INC_PHYSICS_H_ */
