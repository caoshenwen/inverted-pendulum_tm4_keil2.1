#include "my_include.h"

bool     UART1_RX_IntFlag = 0;


int flag1=0;
int flag2=0;
int flag3=0;
char rx[20]={0,0,0,0,0,0,0,0,0,0};
//tsPID PIDBalance;/*平衡环*/
//tsPID PIDSpeed;  /*速度环*/
//tsPID PIDTurn;   /*转向环*/
int16_t velocity_A=0;
int16_t moto = 1;
int16_t PWM = 0;
float    BalancePWM=0;
float    SpeedPWM=0;



void MODE1(void)//moto=16时基础1问，moto=25是基础二问
{

    if(1==flag2)
    {
        PWMOUTSet(5000);
        delayms(300);
        PWMOUTSet(1);
        PWMOUTSet(-5000);
        delayms(300);
        PWMOUTSet(1);
        flag2=0;
    }
//    if(1==flag1)
//    {
//        moto=(int16_t)PIDSpeed_Realize(velocity_A, 0);
//        moto=Xianfu_Pwm((int16_t) moto);
//        PWMOUTSet(moto);
//    }

    if(1==flag1)
    {
        if((angle<181)&&(angle>0))
        {
//            moto = moto+500;
            PWMOUTSet(-moto);
        }else if(angle>181||angle<0)
        {
//            moto = moto+500;
            PWMOUTSet(moto);
        }


    }

//        if(1==flag1)
//        {
//            if((angle>100)&&(angle<181))
//            {
//                moto1 = moto1+500;
//                PWMOUTSet(-moto1);
//            }else if(angle>181||angle<-100)
//            {
//                moto2 = moto2+500;
//                PWMOUTSet(moto2);
//            }
//
//
//        }


}
void MODE2(void)
{
    BalancePWM = PIDBalance_Realize(angle, 0);
//    UARTprintf("BalancePWM: %4d ", (int16_t)BalancePWM);
    BalancePWM = Xianfu_Pwm((int16_t) BalancePWM);
    SpeedPWM   = PIDSpeed_Realize(velocity_A, 0) ;
    SpeedPWM   = Xianfu_Pwm((int16_t) SpeedPWM);
    PWM        = (int16_t)(BalancePWM+SpeedPWM);
    PWMOUTSet(PWM);
    if(angle>70||angle<-70)
    {
        PWMOUTSet(1);
    }
}
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

void IntHandler_Timer0(void)//定时器中断，10ms进入一次中断
{
	static uint32_t Timer0A_Count = 0;
    TimerIntClear(TIMER0_BASE, TIMER_TIMA_TIMEOUT);
    
    Timer0A_Count++;

    if(Timer0A_Count == 1)//10ms打印一次转速
    {
        Timer0A_Count = 0;
        velocity_A=qei_data_array[0].velocity;
//        UARTprintf("velocity_A = %3d  \n",velocity_A);
//        UARTprintf("angle: %4d , ch1:%4umv\n", angle, Ch1Value);
        if(1==flag1)  MODE1();
        if(1==flag3)
        {
            MODE2();

        }

    }
}


int main_1(void)
{
    float kp=0,kd=0;
//    int i1=0,f1=0;
//    int i2=0,f2=0;
    SysCtlClockSet(SYSCTL_SYSDIV_5 | SYSCTL_USE_PLL | SYSCTL_OSC_MAIN | SYSCTL_XTAL_16MHZ);
    FPUEnable();
    FPULazyStackingEnable();
//    u1();//串口1初始化
    PF123_RGB_Output();//LED灯初始化
    PWM_Config();//PWM初始化
    KEY_init();//按键1初始化
    PID_Balance_Init();//定义角度环的参数
    PID_Speed_Init();//定义位置环的参数
    QEI_InitConfig(qei_data_array+0);  //配置QEI 0
    IntPrioritySet(INT_QEI0, 1<<3);//由于本程序用到多个中断，需对这些中断区分执行次序，中断优先级管理
    //Timer0A_Init(10);//初始化定时器，并设置10ms记一次数
    ADC_Init();//ADC初始化
    IntMasterEnable();//总的中断开关打开
    //TimerEnable(TIMER0_BASE, TIMER_A);//使能定时器中断
    while(1)
    {
        RED1;//红灯亮
        BLUE0;//蓝灯灭
        while(1)//
        {
            ADC_Trig();// Trigger a ADC sample interrupt.
            while(true != bDataReady) ;// Wait for sample finish.
            bDataReady = false;// Reset the flag after sample finish.
            key_scan();
            if(UART1_RX_IntFlag==1)//有数据传入
            {
               UART1_RX_IntFlag = 0;
               flag1 = 1;
               flag2 = 1;
//               kp=(float)(((rx[1]-48)*1000+(rx[2]-48)*100+(rx[3]-48)*10+rx[4]-48)/1.00);//0000
//               PIDSpeed.Kp=kp;
//               kd=(float)(((rx[5]-48)*1000+(rx[6]-48)*100+(rx[7]-48)*10+rx[8]-48)/1.00);//00.0
//               PIDSpeed.Kd=kd;
//               UARTprintf("kp = %4d , kp = %4d\n",(int16_t)kp,(int16_t)kd);
               moto = ((rx[1]-48)*1000+(rx[2]-48)*100+(rx[3]-48)*10+rx[4]-48)/1.00;
               UARTprintf("kp = %4d \n",(int16_t)moto);
               if('s'==rx[0])
               {
                  flag1=0;
                  flag2=0;
                  PWMOUTSet(1);
               }
            }
//            MODE1();
            if(true == KeyPress)//按下按键，退出任务1
            {
                KeyPress = false;
                flag1 = 0;
                flag2 = 0;
                delayms(500);
                break;
            }
        }
        RED0;//红灯灭
        BLUE1;//蓝灯亮
        while(1)//任务二，实现倒立
        {
            ADC_Trig();// Trigger a ADC sample interrupt.
            while(true != bDataReady) ;// Wait for sample finish.
            bDataReady = false;// Reset the flag after sample finish.
            key_scan();
            if(UART1_RX_IntFlag==1)//有数据传入
            {
               UART1_RX_IntFlag = 0;
               flag3=1;
               kp=(float)(((rx[1]-48)*100+(rx[2]-48)*10+(rx[3]-48)*1)/1.00);//000.0
               PIDBalance.Kp=(double )-kp*10.0;
               kd=(float)(((rx[4]-48)*100+(rx[5]-48)*10+(rx[6]-48)*1)/1.00);//00.0
               PIDBalance.Kd=(double )-kd*10.0;
//               i1=(int)(PIDBalance.Kp);
//               f1=(int)(PIDBalance.Kp*10)-i1*10;
//               i2=(int)(PIDBalance.Kd);
//               f2=(int)(PIDBalance.Kd*100)-i2*100;
               UARTprintf("Kp = %4d  , Kd = %4d\n",(int16_t)PIDBalance.Kp,(int16_t)PIDBalance.Kd);
               if('s'==rx[0])
               {
                  flag3=0;
                  PWMOUTSet(1);
               }
            }
            if(true == KeyPress)//按下按键，退出任务2
            {
                KeyPress = false;
                flag3 = 0;
                delayms(500);
                break;
            }
        }

    }

}
