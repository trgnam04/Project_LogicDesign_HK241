/*
 * fsm_sensor.c
 *
 *  Created on: Oct 28, 2024
 *      Author: ASUS
 */


#include "fsm_sensor.h"

void fsm_ReadData(void){
	HAL_GPIO_TogglePin(task2_GPIO_Port, task2_Pin);

	// read pressure data
	ReadPressure();

	// read light intensity data
	SendRequestForLTdata();
	SCH_Add_Task(ReadLightIntensity, 0, 18);

}
