/**
 * @file rt_assert.h
 * @author Pavel Marek
 * @date Mar 29, 2018
 *
 * @brief This file contains definition for run-time assert.
 */

#ifndef RT_ASSERT_H_
#define RT_ASSERT_H_

#include <stdio.h>
#include "stm3210c_eval.h"
#include "stm32f1xx_hal.h"

#define rt_assert(condition, comment) \
{ \
	if ( !(condition) ) {\
		printf("%s:%d:%s\n", __FILE__, __LINE__, comment); \
		BSP_LED_Init(LED_RED); \
		for (;;){ \
			BSP_LED_Toggle(LED_RED); \
			HAL_Delay(500); \
		} \
	}\
}

#endif /* RT_ASSERT_H_ */
