

/*
* PID.c
*
*  Created on: 2018年2月8日
*	   Author: admin
*/
#include "PID.h"

tsPID			PIDBalance; /*平衡环*/
tsPID			PIDSpeed; /*速度环*/
tsPID			PIDTurn; /*转向环*/


void PID_Balance_Init()
{
	PIDBalance.Target_Value = 0.0;
	PIDBalance.Actual_Value = 0.0;
	PIDBalance.Err		= 0.0;
	PIDBalance.Err_Last = 0.0;
	PIDBalance.Err_2Last = 0.0;
	PIDBalance.Integral = 0.0;
	PIDBalance.Kp		= 10.0; 					//float
	PIDBalance.Ki		= 0.0;						//float
	PIDBalance.Kd		= 0.0;						//float
	PIDBalance.Output	= 0.0;						//float
}



float PIDBalance_Realize(float Actual, float Target) //角度环，Actual = pitch; Target = 0
{
	//	  static volatile bool Err_Flag;
	//	  static volatile bool Err_LastFlagStatus;
	static float	D_Bias, Last_Bias;

	PIDBalance.Target_Value = Target;
	PIDBalance.Actual_Value = Actual;
	PIDBalance.Err		= PIDBalance.Target_Value - PIDBalance.Actual_Value;
	D_Bias				= PIDBalance.Err - Last_Bias;

	//	  if(PIDBalance.Err > 0)
	//		  Err_Flag = 1;
	//	  else
	//		  Err_Flag = 0;
	//	  if(Err_LastFlagStatus != Err_Flag)
	//	  {
	//		  PIDBalance.Output = 0;
	//		  GyroX = 0;
	//	  }
	//	  else
	//		  ;
	//	  Err_LastFlagStatus = Err_Flag;
	PIDBalance.Output	= PIDBalance.Kp * PIDBalance.Err + PIDBalance.Kd * D_Bias;
	Last_Bias			= PIDBalance.Err;

	//	  if(PIDBalance.Output < -99)	  //
	//		  PIDBalance.Output = -99;	  //
	//	  else if(PIDBalance.Output > 99) //对输出进行限幅
	//		  PIDBalance.Output = 99;	  //
	//	  else							   //
	//		  ; 						   //
	return PIDBalance.Output;
}



void PID_Speed_Init(void)
{
	PIDSpeed.Target_Value = 0.0;
	PIDSpeed.Actual_Value = 0.0;
	PIDSpeed.Err		= 0.0;
	PIDSpeed.Err_Last	= 0.0;
	PIDSpeed.Err_2Last	= 0.0;
	PIDSpeed.Integral	= 0.0;
	PIDSpeed.Kp 		= 0.0;
	PIDSpeed.Ki 		= 0.0;
	PIDSpeed.Kd 		= 0.0;
	PIDSpeed.Output 	= 0.0;
}


//float PIDSpeed_Realize(uint16_t Actual, uint16_t Target, float roll)
//{
//	  static float Speed_LPF;
//
//	  PIDSpeed.Target_Value = Target;
//	  PIDSpeed.Actual_Value = roll>0?Actual:-Actual;
//	  PIDSpeed.Err			= PIDSpeed.Target_Value - PIDSpeed.Actual_Value;
//
//	  Speed_LPF *= 0.7; 			//
//	  Speed_LPF += PIDSpeed.Err*0.3;//对速度偏差进行低通滤波
//
//	  PIDSpeed.Integral += Speed_LPF;
//
//	  PIDSpeed.Output = PIDSpeed.Kp*Speed_LPF + PIDSpeed.Ki*PIDSpeed.Integral;
//
//	 return PIDSpeed.Output;
//}
//float PIDSpeed_Realize(uint16_t Actual, uint16_t Target, float roll)
//{
//	  static float Speed_LPF;
//
//	  PIDSpeed.Target_Value = Target;
//	  PIDSpeed.Actual_Value = roll>0?Actual:-Actual;
//	  PIDSpeed.Err			= PIDSpeed.Target_Value - PIDSpeed.Actual_Value;
//
//	  Speed_LPF *= 0.7; 			//
//	  Speed_LPF += PIDSpeed.Err*0.3;//对速度偏差进行低通滤波
//
//	  PIDSpeed.Integral += Speed_LPF;
//
//	  PIDSpeed.Output = PIDSpeed.Kp*Speed_LPF + PIDSpeed.Ki*PIDSpeed.Integral;
//
//	  return PIDSpeed.Output;
//}
float PIDSpeed_Realize(uint16_t Actual, uint16_t Target) //位置环
{
	static float	Speed_LPF, Last_Position;

	PIDSpeed.Target_Value = Target;

	//	  PIDSpeed.Actual_Value = pitch>0?-Actual:Actual;
	PIDSpeed.Actual_Value = Actual;
	PIDSpeed.Err		= PIDSpeed.Actual_Value - PIDSpeed.Target_Value;

	Speed_LPF			*= (double) 0.8;			//一阶低通滤波器
	Speed_LPF			+= (double)
	PIDSpeed.Err * 0.2; 							//一阶低通滤波器

	PIDSpeed.Integral	= Speed_LPF - Last_Position;
	Last_Position		= Speed_LPF;
	PIDSpeed.Output 	= PIDSpeed.Kp * Speed_LPF + PIDSpeed.Kd * PIDSpeed.Integral;

	//	  if(0==Actual) PIDSpeed.Integral = 0;
	return PIDSpeed.Output;


}


int16_t Xianfu_Pwm(int16_t PWM)
{
	int 			Amplitude = 9999;

	if (PWM == 0)
		PWM = 1;

	if (PWM < -Amplitude)
		PWM = -Amplitude;

	if (PWM > Amplitude)
		PWM = Amplitude;

	return PWM;
}


