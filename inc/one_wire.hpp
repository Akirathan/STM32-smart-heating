/*
 * one_wire.h
 *
 *  Created on: 26. 12. 2016
 *      Author: Mayfa
 */

#ifndef ONE_WIRE_H_
#define ONE_WIRE_H_

#ifdef __cplusplus
	extern "C" {
#endif

#include "stm32f1xx_hal.h"
#include <temp_sensor.hpp> //DEBUG

/* Public functions */
uint32_t one_wire_Init();
int one_wire_InitCommunication();
void one_wire_WriteByte(uint8_t byte);
uint8_t one_wire_ReadByte();
uint8_t one_wire_ReadBit();
void one_wire_Reset();

/* Testing functions */
uint32_t one_wire_TimeTest(uint32_t microsecs);
void one_wire_debug();

#ifdef __cplusplus
	}
#endif

#endif /* ONE_WIRE_H_ */
