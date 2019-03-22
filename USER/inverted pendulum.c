#include "my_include.h"

bool     UART1_RX_IntFlag = 0;


int flag1=0;
int flag2=0;
int flag3=0;
char rx[20]={0,0,0,0,0,0,0,0,0,0};
//tsPID PIDBalance;/*ƽ�⻷*/
//tsPID PIDSpeed;  /*�ٶȻ�*/
//tsPID PIDTurn;   /*ת��*/
int16_t velocity_A=0;
int16_t moto = 1;
int16_t PWM = 0;
float    BalancePWM=0;
float    SpeedPWM=0;



void MODE1(void)//moto=16ʱ����1�ʣ�moto=25�ǻ�������
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

void IntHandler_Timer0(void)//��ʱ���жϣ�10ms����һ���ж�
{
	static uint32_t Timer0A_Count = 0;
    TimerIntClear(TIMER0_BASE, TIMER_TIMA_TIMEOUT);
    
    Timer0A_Count++;

    if(Timer0A_Count == 1)//10ms��ӡһ��ת��
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
//    u1();//����1��ʼ��
    PF123_RGB_Output();//LED�Ƴ�ʼ��
    PWM_Config();//PWM��ʼ��
    KEY_init();//����1��ʼ��
    PID_Balance_Init();//����ǶȻ��Ĳ���
    PID_Speed_Init();//����λ�û��Ĳ���
    QEI_InitConfig(qei_data_array+0);  //����QEI 0
    IntPrioritySet(INT_QEI0, 1<<3);//���ڱ������õ�����жϣ������Щ�ж�����ִ�д����ж����ȼ�����
    //Timer0A_Init(10);//��ʼ����ʱ����������10ms��һ����
    ADC_Init();//ADC��ʼ��
    IntMasterEnable();//�ܵ��жϿ��ش�
    //TimerEnable(TIMER0_BASE, TIMER_A);//ʹ�ܶ�ʱ���ж�
    while(1)
    {
        RED1;//�����
        BLUE0;//������
        while(1)//
        {
            ADC_Trig();// Trigger a ADC sample interrupt.
            while(true != bDataReady) ;// Wait for sample finish.
            bDataReady = false;// Reset the flag after sample finish.
            key_scan();
            if(UART1_RX_IntFlag==1)//�����ݴ���
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
            if(true == KeyPress)//���°������˳�����1
            {
                KeyPress = false;
                flag1 = 0;
                flag2 = 0;
                delayms(500);
                break;
            }
        }
        RED0;//�����
        BLUE1;//������
        while(1)//�������ʵ�ֵ���
        {
            ADC_Trig();// Trigger a ADC sample interrupt.
            while(true != bDataReady) ;// Wait for sample finish.
            bDataReady = false;// Reset the flag after sample finish.
            key_scan();
            if(UART1_RX_IntFlag==1)//�����ݴ���
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
            if(true == KeyPress)//���°������˳�����2
            {
                KeyPress = false;
                flag3 = 0;
                delayms(500);
                break;
            }
        }

    }

}
