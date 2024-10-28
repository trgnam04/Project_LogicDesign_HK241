/*
 * scheduler_O(1).h
 *
 *  Created on: Oct 1, 2024
 *      Author: ASUS
 */

#ifndef INC_SCHEDULER_O_1__H_
#define INC_SCHEDULER_O_1__H_

#include "main.h"
#include "stdlib.h"
#include "software_timer.h"

typedef struct S_Task {
	int Delay;
	int Period;
	void (*pTask)();
	struct S_Task* next;
} S_Task;

typedef struct Container{
	unsigned char emptySlot;
	unsigned char numTask;
	S_Task* tail;
} Container;

extern Container* container;

S_Task* addNode(void(*pFunction)(), int delay, int period);
void SCH_Init(void);
void SCH_Add_Task(void(*pFunction)(), int period, int delay);
void SCH_Delete_Task(void);
void SCH_Update_Task(void);
void SCH_Dispatch_Task(void);






#endif /* INC_SCHEDULER_O_1__H_ */
