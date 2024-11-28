/*
 * fsm_lcd.h
 *
 *  Created on: Oct 28, 2024
 *      Author: ASUS
 */

#ifndef INC_FSM_LCD_H_
#define INC_FSM_LCD_H_

#include "physics.h"

typedef enum{
	IDLE,
	HOME,
	PAGE1,
	PAGE2,
	SETTING
} WeatherStation_state_t;

typedef enum{
	INIT,
	SETTING_UART,
	SETTING_UNIT,
	UNIT_PROCESS,
	UART_PROCESS
} Setting_state_t;

typedef enum{
	INIT_UART,
	ACTIVATE_ENTRY,
	ACTIVATE,
	INACTIVATE_ENTRY,
	INACTIVATE
} Uart_state_t;

typedef enum{
	INIT_UNIT,
	SETTING_TEMP,
	SETTING_PRESS
} Unit_state_t;

typedef struct{
	Setting_state_t settingState;
	Uart_state_t uartState;
	Unit_state_t unitState;
} WeatherStation_substate_t;

typedef struct{
	WeatherStation_state_t activate_state;
	WeatherStation_substate_t activate_substate;
} WeatherStation_t;



void init_System(void);
void fsm_display(void);
void fsm_readSensor(void);
void settingMode_handler(void);
void settingUart_handler(void);
void settingUnit_handler(void);
void page1_handler(void);
void page2_handler(void);
void WeatherStation(void);




#endif /* INC_FSM_LCD_H_ */
