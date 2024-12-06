/*
 * fsm_sensor.c
 *
 *  Created on: Oct 28, 2024
 *      Author: ASUS
 */


#include "fsm_sensor.h"

void fsm_ReadData(void){
	SendRequestDHT20();

	SCH_Add_Task(ReadDHT20, 0, 10);

	// read pressure data
	ReadPressure();

	// read light intensity data
	SendRequestForLTdata();
	SCH_Add_Task(ReadLightIntensity, 0, 18);

}
