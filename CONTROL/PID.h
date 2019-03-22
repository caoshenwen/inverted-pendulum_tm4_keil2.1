/*
 * PID.h
 *
 *  Created on: 2018年2月8日
 *      Author: admin
 */

#ifndef PID_PID_H_
#define PID_PID_H_

#include "stdint.h"
#include "stdbool.h"

typedef struct tsPID
{
    float Target_Value; //目标值
    float Actual_Value; //实际值
    float Err;          //本次偏差值
    float Err_Last;     //上一次偏差值
    float Err_2Last;    //上上一次偏差值
    float Integral;     //对每一次偏差求和得到的积分值
    float Kp, Ki, Kd;   //比例,计分,微分系数
    float Output;       //输出值
}tsPID;


extern tsPID PIDBalance;/*平衡环*/
extern tsPID PIDSpeed;  /*速度环*/
extern tsPID PIDTurn;   /*转向环*/


extern void  PID_Balance_Init(void);
extern float PIDBalance_Realize(float Actual, float Target);

extern void  PID_Speed_Init(void);
extern float PIDSpeed_Realize(uint16_t Actual, uint16_t Target);
extern int16_t Xianfu_Pwm(int16_t PWM);



#endif /* PID_PID_H_ */
