///*
// * scheduler.h
// *
// *  Created on: Sep 23, 2024
// *      Author: ASUS
// */
//
//#ifndef INC_SCHEDULER_H_
//#define INC_SCHEDULER_H_
//
//#include "main.h"
//#include <string.h>
//
//#define SCH_MAX_TASK 10
//
//typedef struct{
//	void(*pTask)(void);
//	uint16_t Delay;
//	uint16_t Period;
//	uint8_t RunMe;
//} sTask;
//
//sTask SCH_Task_G[SCH_MAX_TASK];
//
//void SCH_Init(void);
//void SCH_Update(void);
//uint8_t SCH_Add_Tasks(void(* pTask)(), const uint16_t delay, const uint16_t period);
//void SCH_Delete_Tasks(const uint8_t TaskID);
//void SCH_Dispatch_Tasks();
//
//#endif /* INC_SCHEDULER_H_ */
