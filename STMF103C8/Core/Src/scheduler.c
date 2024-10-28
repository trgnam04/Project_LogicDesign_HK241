/*
 * scheduler.c
 *
 *  Created on: Sep 23, 2024
 *      Author: ASUS
 */

#include "scheduler.h"

//void SCH_Init(void){
//	for(int i =  0; i < SCH_MAX_TASK; i++){
//		SCH_Task_G[i].pTask = 0;
//		SCH_Task_G[i].Period = 0;
//		SCH_Task_G[i].Delay = 0;
//		SCH_Task_G[i].RunMe = 0;
//	}
//}
//
//void SCH_Update(void){
//	for(int i = 0; i < SCH_MAX_TASK; i++){
//		if(SCH_Task_G[i].pTask){
//			SCH_Task_G[i].Delay--;
//			if(SCH_Task_G[i].Delay <= 0){
//				SCH_Task_G[i].Delay = SCH_Task_G[i].Period;
//				SCH_Task_G[i].RunMe += 1;
//			}
//		}
//	}
//}
//
//uint8_t SCH_Add_Tasks(void(* pFunction)(), const uint16_t delay, const uint16_t period){
//	uint8_t index = 0;
//	for(index = 0; index < SCH_MAX_TASK; index++){
//		if(SCH_Task_G[index].pTask == 0){
//			SCH_Task_G[index].pTask = pFunction;
//			SCH_Task_G[index].Delay = delay;
//			SCH_Task_G[index].Period = period;
//			SCH_Task_G[index].RunMe = 0;
//		}
//	}
//
//	return index;
//}
//
//void SCH_Delete_Tasks(const uint8_t TaskID){
//	for(int i = 0; i < TaskID; i++){
//		SCH_Task_G[i].pTask = 0;
//		SCH_Task_G[i].Delay = 0;
//		SCH_Task_G[i].Period = 0;
//		SCH_Task_G[i].RunMe = 0;
//	}
//}
//
//void SCH_Dispatch_Tasks(){
//	for(int i = 0; i < SCH_MAX_TASK; i++){
//		if(SCH_Task_G[i].pTask && SCH_Task_G[i].RunMe > 0){
//			(*SCH_Task_G[i].pTask)();
//			SCH_Task_G[i].RunMe -= 1;
//			if(SCH_Task_G[i].Period == 0){
//				SCH_Delete_Tasks(i);
//			}
//		}
//	}
//
//	//  can include report status
//	//  can go to sleep
//}
//
//
