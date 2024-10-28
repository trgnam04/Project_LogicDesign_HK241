/*
 * fsm_lcd.c
 *
 *  Created on: Oct 28, 2024
 *      Author: ASUS
 */
#include "fsm_lcd.h"

typedef enum {
	LCD_INIT,
	DISPLAY_PAGE1,
	DISPLAY_PAGE2,
	DISPLAY_PAGE3
} LCD_STATE;

LCD_STATE lcd_state = LCD_INIT;

unsigned char temp = 0;


void fsm_lcd(void){
	switch(lcd_state){
	case LCD_INIT:{

		lcd_state = DISPLAY_PAGE1;
		break;
	}
	case DISPLAY_PAGE1:{

		LCD_DisplayPage1();

		if(is_button_press(0)){
			temp = 1;
		}
		if(!is_button_press(0) && temp){
			LCD_Clear();
			temp = 0;
			lcd_state = DISPLAY_PAGE2;
		}
		break;
	}
	case DISPLAY_PAGE2:{
		LCD_DisplayPage2(0, 0);

		if(is_button_press(0)){
			temp = 1;
		}
		if(!is_button_press(0) && temp){
			LCD_Clear();
			temp = 0;
			lcd_state = DISPLAY_PAGE3;
		}
		break;
	}
	case DISPLAY_PAGE3:{
		LCD_DisplayPage3(0, 0);


		if(is_button_press(0)){
			temp = 1;
		}
		if(!is_button_press(0) && temp){
			LCD_Clear();
			temp = 0;
			lcd_state = DISPLAY_PAGE1;
		}
		break;
	}
	default:{
		break;
	}
	}

}
