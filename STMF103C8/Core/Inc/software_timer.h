/*
 * software_timer.h
 *
 *  Created on: Sep 10, 2024
 *      Author: ASUS
 */

#ifndef INC_SOFTWARE_TIMER_H_
#define INC_SOFTWARE_TIMER_H_

#include "main.h"
#include "scheduler_O(1).h"

void timerInit(void);
void setTimer(int idx, int DURATION);
void timerRun(void);






#endif /* INC_SOFTWARE_TIMER_H_ */
