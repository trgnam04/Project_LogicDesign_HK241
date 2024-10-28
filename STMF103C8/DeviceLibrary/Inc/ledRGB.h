/*
 * ledRGB.h
 *
 *  Created on: Sep 20, 2024
 *      Author: ASUS
 */

#ifndef INC_LEDRGB_H_
#define INC_LEDRGB_H_

#include "main.h"

typedef struct{
	uint8_t red;
	uint8_t green;
	uint8_t blue;
	TIM_HandleTypeDef* tim;
} LED_RGB;

HAL_StatusTypeDef init_ledRGB(TIM_HandleTypeDef* htim_rgb);
void set_codeRGB(TIM_HandleTypeDef* htim_rgb,uint32_t codeRGB);



#endif /* INC_LEDRGB_H_ */
