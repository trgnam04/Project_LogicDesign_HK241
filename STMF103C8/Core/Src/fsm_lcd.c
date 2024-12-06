/*
 * fsm_lcd.c
 *
 *  Created on: Oct 28, 2024
 *      Author: ASUS
 */
#include "fsm_lcd.h"

uint8_t unitDisplay_temperature = 0;
uint8_t unitDisplay_pressure = 0;
static unsigned char flagReceivedData = 0;
static unsigned char tempState1 = 0;
static unsigned char tempState2 = 0;
static unsigned char tempState3 = 0;
static WeatherStation_t mobj;

void init_System(void){
	InitPhysics();
	mobj.activate_state = IDLE;
	mobj.activate_substate.settingState = INIT;
	mobj.activate_substate.uartState = INIT_UART;
	mobj.activate_substate.unitState = INIT_UNIT;

}

void fsm_readSensor(void){
	flagReceivedData = 1;
	// read pressure data
	SendRequestDHT20();
	SCH_Add_Task(ReadDHT20, 0, 10);
	ReadPressure();

	// read light intensity data
	SendRequestForLTdata();
	SCH_Add_Task(ReadLightIntensity, 0, 18);

}

void settingUart_handler(void){
	switch(mobj.activate_substate.uartState){
	case INIT_UART:{
		mobj.activate_substate.uartState = ACTIVATE_ENTRY;
		break;
	}
	case ACTIVATE_ENTRY:{
		LCD_Clear();
		LCD_displaySettingUart(0);
		mobj.activate_substate.uartState = ACTIVATE;
		break;
	}
	case ACTIVATE:{
		flagUpdateData = 1;
		if(is_button_press(0)){
			tempState1 = 1;
		}
		if(tempState1 && !is_button_press(0)){
			tempState1 = 0;
			mobj.activate_substate.uartState = INACTIVATE_ENTRY;
		}
		break;
	}
	case INACTIVATE_ENTRY:{
		LCD_Clear();
		LCD_displaySettingUart(1);
		mobj.activate_substate.uartState = INACTIVATE;
		break;
	}
	case INACTIVATE:{
		flagUpdateData = 0;
		if(is_button_press(0)){
			tempState1 = 1;
		}
		if(tempState1 && !is_button_press(0)){
			tempState1 = 0;
			mobj.activate_substate.uartState = ACTIVATE_ENTRY;
		}
		break;
	}
	}
}

void settingUnit_handler(void){
	switch(mobj.activate_substate.unitState){
	case INIT_UNIT:{
		SetCursor(0, 0);
		LCD_putChar('>');
		LCD_displaySettingTemp(unitDisplay_temperature);
		mobj.activate_substate.unitState = SETTING_TEMP;
		break;
	}
	case SETTING_TEMP:{
		if(is_button_press(0)){
			tempState1 = 1;
		}
		if(tempState1 && !is_button_press(0)){
			tempState1 = 0;
			unitDisplay_temperature = (unitDisplay_temperature + 1) % 2;
			LCD_displaySettingTemp(unitDisplay_temperature);
			break;
		}


		if(is_button_press(1)){
			tempState2 = 1;
		}
		if(tempState2 && !is_button_press(1)){
			tempState2 = 0;
			SetCursor(0, 0);
			LCD_putChar(' ');
			SetCursor(0, 1);
			LCD_putChar('>');
			LCD_displaySettingPress(unitDisplay_pressure);
			mobj.activate_substate.unitState = SETTING_PRESS;
		}
		break;
	}
	case SETTING_PRESS:{
		if(is_button_press(0)){
			tempState1 = 1;
		}
		if(tempState1 && !is_button_press(0)){
			tempState1 = 0;
			unitDisplay_pressure = (unitDisplay_pressure + 1) % 2;
			LCD_displaySettingPress(unitDisplay_pressure);
			break;
		}

		if(is_button_press(1)){
			tempState2 = 1;
		}
		if(tempState2 && !is_button_press(1)){
			tempState2 = 0;
			SetCursor(0, 1);
			LCD_putChar(' ');
			mobj.activate_substate.unitState = INIT_UNIT;
		}
		break;
	}
	}
}

void settingMode_handler(void){
	switch(mobj.activate_substate.settingState){
	case INIT:{
		LCD_displaySettingMode();
		SetCursor(9, 0);
		LCD_putChar('>');
		SetCursor(9, 1);
		LCD_putChar(' ');
		mobj.activate_substate.settingState = SETTING_UART;
		break;
	}
	case SETTING_UART:{
		if(is_button_press(0)){
			tempState1 = 1;
		}
		if(tempState1 && !is_button_press(0)){
			tempState1 = 0;
			if(mobj.activate_substate.uartState == ACTIVATE){
				mobj.activate_substate.uartState = ACTIVATE_ENTRY;
			}
			else{
				mobj.activate_substate.uartState = INACTIVATE_ENTRY;
			}
			mobj.activate_substate.settingState = UART_PROCESS;
			break;
		}


		if(is_button_press(1)){
			tempState2 = 1;
		}
		if(tempState2 && !is_button_press(1)){
			tempState2 = 0;
			SetCursor(9, 1);
			LCD_putChar('>');
			SetCursor(9, 0);
			LCD_putChar(' ');
			mobj.activate_substate.settingState = SETTING_UNIT;
		}
		break;
	}
	case UART_PROCESS:{
		settingUart_handler();
		if(is_button_press1s(2)){
			mobj.activate_substate.settingState = INIT;
		}
		break;
	}
	case SETTING_UNIT:{
		if(is_button_press(1)){
			tempState2 = 1;
		}
		if(tempState2 && !is_button_press(1)){
			tempState2 = 0;
			mobj.activate_substate.settingState = INIT;
			break;
		}

		if(is_button_press(0)){
			tempState1 = 1;
		}
		if(tempState1 && !is_button_press(0)){
			tempState1 = 0;
			LCD_Clear();
			LCD_displaySettingUnit();
			mobj.activate_substate.unitState = INIT_UNIT;
			mobj.activate_substate.settingState = UNIT_PROCESS;
		}
		break;
	}
	case UNIT_PROCESS:{
		settingUnit_handler();

		if(is_button_press1s(2)){
			LCD_Clear();
			mobj.activate_substate.settingState = INIT;
		}
		break;
	}
	}
}

void WeatherStation(void){
	switch(mobj.activate_state){
	case IDLE:{
		LCD_DisplayPage1();
		mobj.activate_state = HOME;
		break;
	}
	case HOME:{
		if(is_button_press(0)){
			tempState1 = 1;
		}
		if(tempState1 && !is_button_press(0)){
			tempState1 = 0;
			LCD_Clear();
			mobj.activate_state = SETTING;
			mobj.activate_substate.settingState = INIT;
			mobj.activate_substate.unitState = INIT_UNIT;
			break;
		}

		if(is_button_press(1)){
			tempState2 = 1;
		}
		if(tempState2 && !is_button_press(1)){
			tempState2 = 0;
			LCD_Clear();
			LCD_DisplayPage2(unitDisplay_temperature);
			mobj.activate_state = PAGE1;
		}
		break;
	}
	case PAGE1:{
		if(flagReceivedData){
			LCD_DisplayPage2(unitDisplay_temperature);
			flagReceivedData = 0;
		}


		if(is_button_press(1)){
			tempState2 = 1;
		}
		if(tempState2 && !is_button_press(1)){
			tempState2 = 0;
			LCD_Clear();
			LCD_DisplayPage3(unitDisplay_pressure);
			mobj.activate_state = PAGE2;
		}
		break;
	}
	case PAGE2:{

		if(flagReceivedData){
			LCD_DisplayPage3(unitDisplay_pressure);
			flagReceivedData = 0;
		}

		if(is_button_press(1)){
			tempState2 = 1;
		}
		if(tempState2 && !is_button_press(1)){
			tempState2 = 0;
			LCD_Clear();
			mobj.activate_state = IDLE;
		}
		break;
	}
	case SETTING:{
		settingMode_handler();

		if(is_button_press1s(2)){
			LCD_Clear();
			mobj.activate_substate.settingState = INIT;
			mobj.activate_state = IDLE;
		}
		break;
	}
	}
}

