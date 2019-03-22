#ifndef  _AHRS_H_
#define  _AHRS_H_

#include <math.h>
//#include "arm_math.h"
#include "drv_imu.h"
#include "drv_compass.h"
#include "drv_baro.h"
#include "DronePara.h"

extern DroneRTInfo RT_Info;
extern volatile float q0, q1, q2, q3;

void Sensor_GetVal(float *values);
void Attitue_GetVal(void);

void AHRS_update(float gx, float gy, float gz, float ax, float ay, float az, float mx, float my, float mz);
void AHRS_update_IMU(float gx, float gy, float gz, float ax, float ay, float az);

#endif

