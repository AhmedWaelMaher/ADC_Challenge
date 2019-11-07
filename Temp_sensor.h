/*
 * Temp_sensor.h
 *
 *  Created on: Nov 7, 2019
 *      Author: ahmed
 */

#ifndef TEMP_SENSOR_H_
#define TEMP_SENSOR_H_

#include "Timer.h"
#include "ADC.h"

void TempSensor_init(void);
void TempCal(void);
void Temp_setCallBack(void(*ptr)(void));


#endif /* TEMP_SENSOR_H_ */
