
#include "my_include.h"

bool			UART1_RX_IntFlag = 0;


int 			flag1 = 0;
int 			flag2 = 0;
int 			flag3 = 1;
char			rx[20] ={0, 0, 0, 0, 0, 0, 0, 0, 0, 0};


//tsPID PIDBalance;/*ƽ�⻷*/
//tsPID PIDSpeed;  /*�ٶȻ�*/
//tsPID PIDTurn;   /*ת��*/
int16_t 		velocity_A = 0;
int16_t 		moto = 1;
int16_t 		PWM = 0;
float			BalancePWM = 0;
float			SpeedPWM = 0;



void MODE1(void) //moto=16ʱ����1�ʣ�moto=25�ǻ�������

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
void inthandler_uart1(void)//����1�����жϣ��ѽ��ܵ����ݴ浽һ��������
{
	uint8_t i = 0;
	uint32_t ui32Status_Int = UARTIntStatus(UART1_BASE, true);//��ȡ�ж�״̬
	UARTIntClear(UART1_BASE, ui32Status_Int);//����жϱ�־λ

	
	while(UARTCharsAvail(UART1_BASE))
	{
		rx[i] = (char)UARTCharGetNonBlocking(UART1_BASE);//���ܴ��ڵ����ݣ��浽������
		i++;
	}

	UART1_RX_IntFlag=1;
}
*/
void IntHandler_Timer0(void) //��ʱ���жϣ�10ms����һ���ж�
{
	static uint32_t Timer0A_Count = 0;

	TimerIntClear(TIMER0_BASE, TIMER_TIMA_TIMEOUT);
	Timer0A_Count++;

	if (Timer0A_Count == 1) //10ms��ӡһ��ת��
	{
		Timer0A_Count		= 0;
		velocity_A			= qei_data_array[0].velocity;

		//if(1==flag1)	MODE1();
		//if(1==flag3) MODE2();
		MODE2();

	}
}



