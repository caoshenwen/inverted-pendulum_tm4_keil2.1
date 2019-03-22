/*
 * PID.h
 *
 *  Created on: 2018��2��8��
 *      Author: admin
 */

#ifndef PID_PID_H_
#define PID_PID_H_

#include "stdint.h"
#include "stdbool.h"

typedef struct tsPID
{
    float Target_Value; //Ŀ��ֵ
    float Actual_Value; //ʵ��ֵ
    float Err;          //����ƫ��ֵ
    float Err_Last;     //��һ��ƫ��ֵ
    float Err_2Last;    //����һ��ƫ��ֵ
    float Integral;     //��ÿһ��ƫ����͵õ��Ļ���ֵ
    float Kp, Ki, Kd;   //����,�Ʒ�,΢��ϵ��
    float Output;       //���ֵ
}tsPID;


extern tsPID PIDBalance;/*ƽ�⻷*/
extern tsPID PIDSpeed;  /*�ٶȻ�*/
extern tsPID PIDTurn;   /*ת��*/


extern void  PID_Balance_Init(void);
extern float PIDBalance_Realize(float Actual, float Target);

extern void  PID_Speed_Init(void);
extern float PIDSpeed_Realize(uint16_t Actual, uint16_t Target);
extern int16_t Xianfu_Pwm(int16_t PWM);



#endif /* PID_PID_H_ */
