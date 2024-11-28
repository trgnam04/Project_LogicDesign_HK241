/*
 * software_timer.c
 *
 *  Created on: Sep 11, 2024
 *      Author: ASUS
 */

#include "software_timer.h"


#define MAX_TIMER 5

int timerArr[MAX_TIMER];
unsigned char timerFlag[MAX_TIMER];


void timerInit(void){
	for(uint8_t i = 0; i < MAX_TIMER; i++){
		timerArr[i] = 0;
		timerFlag[i] = 0;
	}
}

void setTimer(int idx, int DURATION){
	for(uint8_t i = 0; i < MAX_TIMER; i++){
		if(i == idx){
			timerArr[i] = DURATION;
			timerFlag[i] = 0;
		}
	}
}

void timerRun(void){
	for(uint8_t i = 0; i < MAX_TIMER; i++){
		if(timerArr[i] > 0){
			timerArr[i]--;
		}
		if(timerArr[i] == 0){
			timerFlag[i] = 1;
		}
	}
}



void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim){
	if(htim->Instance == TIM2){
		SCH_Update_Task();
	}
}




