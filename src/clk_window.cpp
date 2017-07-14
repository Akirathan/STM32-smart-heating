/*
 * clk_window.c
 *
 *  Created on: 4.2.2017
 *      Author: Mayfa
 */

#include "clk_window.hpp"

/* Private variables. */
static clk_window_window_t selected = HOURS;
static uint8_t hours = 12;
static uint8_t minutes = 30;

/* Private function prototypes */
static void nextWindow();
static void previousWindow();
static void redrawSelected();
static void redrawTime();
static void selectHours(uint16_t Color);
static void selectMinutes(uint16_t Color);
static void selectOK(uint16_t Color);
static void increase();
static void decrease();

/**
 * Show the clock-selection user window and initializes
 * the JOY in GPIO mode.
 */
AppStatus_TypeDef clk_window_Show() {
	/* Initialize joystick peripheral */
	if (BSP_JOY_Init(JOY_MODE_GPIO) != IO_OK) {
		return APP_ERROR;
	}

	/* Clear the display and init LCD peripheral */
	if (BSP_LCD_Init() != LCD_OK) {
		return APP_ERROR;
	}
	BSP_LCD_Clear(LCD_COLOR_BLACK);

	/* Draw header */
	BSP_LCD_SetFont(&Font24);
	BSP_LCD_SetTextColor(LCD_COLOR_LIGHTBLUE);
	BSP_LCD_FillRect(0, 0, BSP_LCD_GetXSize(), 40);
	BSP_LCD_SetTextColor(LCD_COLOR_GREEN);
	BSP_LCD_SetBackColor(LCD_COLOR_LIGHTBLUE);
	BSP_LCD_DisplayStringAt(0, 10, (uint8_t *)"Time setting", CENTER_MODE);

	/* Draw hours and minutes buttons. */
	redrawTime();

	/* Draw OK button */
	BSP_LCD_DisplayStringAt(0, BSP_LCD_GetYSize() - 35, (uint8_t *)"OK", CENTER_MODE);

	return APP_OK;
}

/**
 * Main function for the clock window.
 */
 AppStatus_TypeDef clk_window_ReadUserInput(RTC_TimeTypeDef* RTC_Time) {
	JOYState_TypeDef joyState = JOY_NONE;

	while (1) {
		HAL_Delay(100);
		joyState = BSP_JOY_GetState();
		switch (joyState) {
		case JOY_LEFT:
			previousWindow();
			redrawSelected();
			break;
		case JOY_RIGHT:
			nextWindow();
			redrawSelected();
			break;
		case JOY_UP:
			if (selected == HOURS || selected == MINUTES) {
				increase();
				redrawTime();
			}
			break;
		case JOY_DOWN:
			if (selected == HOURS || selected == MINUTES) {
				decrease();
				redrawTime();
			}
			break;
		case JOY_NONE:
			break;
		case JOY_SEL:
			if (selected == OK) {
				/* User confirmed the time. */
				RTC_Time->Hours = hours;
				RTC_Time->Minutes = minutes;
				RTC_Time->Seconds = 0;
				return APP_OK;
			}
			break;
		}
	}
}

static void nextWindow() {
	if (selected == OK) {
		selected = HOURS;
	}
	else {
		//selected++; //TODO
	}
}

static void previousWindow() {
	if (selected == HOURS) {
		selected = OK;
	}
	else {
		//selected--; //TODO
	}
}

static void redrawSelected() {
	/* Reset (unselect) all the buttons. */
	selectHours(LCD_COLOR_WHITE);
	selectMinutes(LCD_COLOR_WHITE);
	selectOK(LCD_COLOR_WHITE);

	switch(selected) {
	case HOURS:
		selectHours(LCD_COLOR_RED);
		break;
	case MINUTES:
		selectMinutes(LCD_COLOR_RED);
		break;
	case OK:
		selectOK(LCD_COLOR_RED);
		break;
	}
}

static void redrawTime() {
	/* Convert the time to string. */
	char text[6];
	sprintf(text, "%02u:%02u", hours, minutes);

	/* Print the string. */
	/* TODO color selection outside function? */
	/* TODO clear before printing the string? */
	BSP_LCD_SetTextColor(LCD_COLOR_BLACK);
	BSP_LCD_SetBackColor(LCD_COLOR_WHITE);
	BSP_LCD_DisplayStringAt(0, BSP_LCD_GetYSize()/2, (uint8_t *)text, CENTER_MODE);
}

static void selectHours(uint16_t Color) {
	BSP_LCD_SetTextColor(Color);
	BSP_LCD_DrawCircle(BSP_LCD_GetXSize()/2 - 35, BSP_LCD_GetYSize()/2 + 10, 20);
}

static void selectMinutes(uint16_t Color) {
	BSP_LCD_SetTextColor(Color);
	BSP_LCD_DrawCircle(BSP_LCD_GetXSize()/2 + 20, BSP_LCD_GetYSize()/2 + 10, 20);
}

static void selectOK(uint16_t Color) {
	BSP_LCD_SetTextColor(Color);
	BSP_LCD_DrawCircle(BSP_LCD_GetXSize()/2 - 5, BSP_LCD_GetYSize() - 25, 20);
}

static void increase() {
	if (selected == HOURS) {
		if (hours == 23) {
			/* Hours overflow. */
			hours = 0;
		}
		else {
			hours++;
		}
	}
	else if (selected == MINUTES) {
		if (minutes == 59) {
			/* Minutes overflow. */
			minutes = 0;
		}
		else {
			minutes++;
		}
	}
}

static void decrease() {
	if (selected == HOURS) {
		if (hours == 0) {
			hours = 23;
		}
		else {
			hours--;
		}
	}
	else if (selected == MINUTES) {
		if (minutes == 0) {
			minutes = 59;
		}
		else {
			minutes--;
		}
	}
}
