/*
 * button.c
 *
 *  Created on: Jul 10, 2023
 *      Author: 84935
 *  Chu y: Cau hinh cac chan vao Button la Input_pullup
 *
 */

#include "button.h"

/*button press => logic 0*/
static uint8_t BUTTON_ReadPin(BUTTON_Name* Button)
{
	return HAL_GPIO_ReadPin(Button->BUTTON_PORT, Button->BUTTON_Pin);
}
static void BUTTON_DelayMs(uint16_t Time)
{
	HAL_Delay(Time);
}

void BUTTON_Init(BUTTON_Name* Button, GPIO_TypeDef* BUTTON_PORT, uint16_t BUTTON_Pin)
{
	Button->BUTTON_PORT = BUTTON_PORT;
	Button->BUTTON_Pin = BUTTON_Pin;
}

BUTTON_STATE BUTTON_Read(BUTTON_Name* Button)
{
	Button->State = NO_CLICK;
	while(BUTTON_ReadPin(Button) == 0)
	{
		Button->timePress++; // dem thoi gian button dang duoc nhan giu
		Button->isPress = 1;
		BUTTON_DelayMs(1);
	}
	if(Button->isPress)
	{
		while(BUTTON_ReadPin(Button) == 1)
		{
			Button->timeDouble++; // dem thoi gian nha giua 2 lan nhan
			BUTTON_DelayMs(1);
			if(Button->timeDouble > DOUBLE_CLICK_TIME) 	// neu vuot qua thoi gian cho phep de xem tt do la double click
														//thi xet tiep thoi gian gian nhan de xem do la single click hay long click
			{
				if(Button->timePress > DEBOUND_TIME && Button->timePress <= SINGLE_CLICK_TIME)
				{
					Button->isPress = 0;
					Button->timePress = 0;
					Button->timeDouble = 0;
					Button->State = SINGLE_CLICK;
					return Button->State;
				}
				else if(Button->timePress  > SINGLE_CLICK_TIME)
				{
					Button->isPress = 0;
					Button->timePress = 0;
					Button->timeDouble = 0;
					Button->State = LONGCLICK_1S;
					return Button->State;
				}
			}
		}
		while(BUTTON_ReadPin(Button) == 0)
		{
			Button->State = DOUBLE_CLICK;
			Button->isPress = 0;
			Button->timePress = 0;
			Button->timeDouble = 0;
			return Button->State;
		}
	}
	return NO_CLICK;
}
