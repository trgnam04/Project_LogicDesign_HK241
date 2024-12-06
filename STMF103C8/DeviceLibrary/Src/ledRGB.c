/*
 * ledRGB.c
 *
 *  Created on: Sep 20, 2024
 *      Author: ASUS
 */
#include "ledRGB.h"

HAL_StatusTypeDef init_ledRGB(TIM_HandleTypeDef* htim_rgb){
	if(HAL_TIM_PWM_Start(htim_rgb, TIM_CHANNEL_1) != HAL_OK){
		return HAL_ERROR;
	}
	else if(HAL_TIM_PWM_Start(htim_rgb, TIM_CHANNEL_2) != HAL_OK){
		return HAL_ERROR;
	}
	else if(HAL_TIM_PWM_Start(htim_rgb, TIM_CHANNEL_3) != HAL_OK){
		return HAL_ERROR;
	}
	return HAL_OK;
}

void set_codeRGB(TIM_HandleTypeDef* htim_rgb, uint32_t codeRGB) {
    uint8_t red = (((codeRGB >> 16) & 0xFF) * 100) / 255;
    uint8_t green = (((codeRGB >> 8) & 0xFF) * 100) / 255;
    uint8_t blue = ((codeRGB & 0xFF) * 100) / 255;

    __HAL_TIM_SET_COMPARE(htim_rgb, TIM_CHANNEL_1, red);
    __HAL_TIM_SET_COMPARE(htim_rgb, TIM_CHANNEL_2, green);
    __HAL_TIM_SET_COMPARE(htim_rgb, TIM_CHANNEL_3, blue);
}

