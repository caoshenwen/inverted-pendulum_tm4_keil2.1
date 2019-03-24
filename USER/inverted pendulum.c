
#include "my_include.h"

bool			UART1_RX_IntFlag = 0;


int 			flag1 = 0;
int 			flag2 = 0;
int 			flag3 = 1;
char			rx[20] ={0, 0, 0, 0, 0, 0, 0, 0, 0, 0};


//tsPID PIDBalance;/*平衡环*/
//tsPID PIDSpeed;  /*速度环*/
//tsPID PIDTurn;   /*转向环*/
int16_t 		velocity_A = 0;
int16_t 		moto = 1;
int16_t 		PWM = 0;
float			BalancePWM = 0;
float			SpeedPWM = 0;



void MODE1(void) //moto=16时基础1问，moto=25是基础二问

{

	if (1 == flag2)
	{
		PWMOUTSet(5000);
		delayms(300);
		PWMOUTSet(1);
		PWMOUTSet(-5000);
		delayms(300);
		PWMOUTSet(1);
		flag2				= 0;
	}

	//	  if(1==flag1)
	//	  {
	//		  moto=(int16_t)PIDSpeed_Realize(velocity_A, 0);
	//		  moto=Xianfu_Pwm((int16_t) moto);
	//		  PWMOUTSet(moto);
	//	  }
	if (1 == flag1)
	{
		if ((angle <181)&&(angle> 0))
		{
			//			  moto = moto+500;
			PWMOUTSet(-moto);
		}
		else if (angle > 181 || angle < 0)
		{
			//			  moto = moto+500;
			PWMOUTSet(moto);
		}


	}

	//		  if(1==flag1)
	//		  {
	//			  if((angle>100)&&(angle<181))
	//			  {
	//				  moto1 = moto1+500;
	//				  PWMOUTSet(-moto1);
	//			  }else if(angle>181||angle<-100)
	//			  {
	//				  moto2 = moto2+500;
	//				  PWMOUTSet(moto2);
	//			  }
	//
	//
	//		  }
}


void MODE2(void)
{

	BalancePWM			= PIDBalance_Realize(angle, 180);
	BalancePWM			= Xianfu_Pwm((int16_t) BalancePWM);
	SpeedPWM			= PIDSpeed_Realize(velocity_A, 0);
	SpeedPWM			= Xianfu_Pwm((int16_t) SpeedPWM);
	PWM 				= (int16_t) (BalancePWM + SpeedPWM);
	PWMOUTSet(PWM);

	if (angle > 200 || angle < 160)
	{
		PWMOUTSet(1);
	}
}


/*
void inthandler_uart1(void)//串口1接受中断，把接受的数据存到一个数组里
{
	uint8_t i = 0;
	uint32_t ui32Status_Int = UARTIntStatus(UART1_BASE, true);//获取中断状态
	UARTIntClear(UART1_BASE, ui32Status_Int);//清除中断标志位

	
	while(UARTCharsAvail(UART1_BASE))
	{
		rx[i] = (char)UARTCharGetNonBlocking(UART1_BASE);//接受串口的数据，存到数组里
		i++;
	}

	UART1_RX_IntFlag=1;
}
*/
void IntHandler_Timer0(void) //定时器中断，10ms进入一次中断
{
	static uint32_t Timer0A_Count = 0;

	TimerIntClear(TIMER0_BASE, TIMER_TIMA_TIMEOUT);
	Timer0A_Count++;

	if (Timer0A_Count == 1) //10ms打印一次转速
	{
		Timer0A_Count		= 0;
		velocity_A			= qei_data_array[0].velocity;

		//if(1==flag1)	MODE1();
		//if(1==flag3) MODE2();
		MODE2();

	}
}



