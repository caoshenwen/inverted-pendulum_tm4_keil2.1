/*
 * PWM_Model.c
 *
 *  Created on: 2018年1月29日
 *      Author: bobo
 */
#include <stdint.h>
#include <stdbool.h>
#include "inc/hw_memmap.h"
#include "driverlib/pin_map.h"
#include "inc/hw_types.h"
#include "driverlib/gpio.h"
#include "driverlib/pwm.h"
#include "driverlib/sysctl.h"
#include "PWM_Model.h"

void PWM_Config(void);
bool PWMDutyCycleSet(int32_t Val);
void PWMOUTSet(int16_t Duty1);
#define PWM_DUTY        1     // pwm duty, PWM_DUTY%.


//   Motor1      Motor2
// ENA IN1 IN2 IN3 IN4 ENB
// B6  /A4  /A3  /A5  /A6  B7
//     0   1   0   1       正转
//     1   0   1   0       反转
//     0   0   0   0       制动
//     1   1   1   1       制动

void PWM_Config(void)
{
   

//  	 SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOC);
//     SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOF);
//     SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOA);

//     GPIOPinTypeGPIOOutput(GPIO_PORTA_BASE, GPIO_PIN_5|GPIO_PIN_6);      //A7即M1
//     GPIOPinTypeGPIOOutput(GPIO_PORTA_BASE, GPIO_PIN_3|GPIO_PIN_4);

//     GPIOPinWrite(GPIO_PORTA_BASE, GPIO_PIN_5|GPIO_PIN_6, 0x0);
//     GPIOPinWrite(GPIO_PORTA_BASE, GPIO_PIN_3|GPIO_PIN_4, 0x0);
     //
	
	
	
	
	
	
     SysCtlPWMClockSet(SYSCTL_PWMDIV_2);  //SysClock/8
     //
     // Enable the PWM0 peripheral
     //
     SysCtlPeripheralEnable(SYSCTL_PERIPH_PWM0);
     SysCtlPeripheralEnable(SYSCTL_PERIPH_PWM1);
    //
    // Wait for the PWM0 module to be ready.
    //
    while (!SysCtlPeripheralReady(SYSCTL_PERIPH_PWM0));

    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOB);  //用GPIOB
    GPIOPinConfigure(GPIO_PB6_M0PWM0);  // Gen 0
    GPIOPinConfigure(GPIO_PB7_M0PWM1);  // Gen 0


    GPIOPinTypePWM(GPIO_PORTB_BASE, GPIO_PIN_6|GPIO_PIN_7);
 //   GPIOPinTypePWM(GPIO_PORTF_BASE, GPIO_PIN_1);
    //
    // Configure the PWM generator for count down mode with immediate updates
    // to the parameters.
    //
    PWMGenConfigure(PWM0_BASE, PWM_GEN_0,
                    PWM_GEN_MODE_UP_DOWN | PWM_GEN_MODE_NO_SYNC);
    PWMGenConfigure(PWM0_BASE, PWM_GEN_1,
                       PWM_GEN_MODE_UP_DOWN | PWM_GEN_MODE_NO_SYNC);

    PWMGenPeriodSet(PWM0_BASE, PWM_GEN_0, PWMPERIOD);  //
    PWMGenPeriodSet(PWM0_BASE, PWM_GEN_1,PWMPERIOD );//
     //
    // Start the timers in generator 0.
    //

    // Enable the outputs.
    //
    PWMOutputState(PWM0_BASE,PWM_OUT_0_BIT, true);
    PWMOutputState(PWM0_BASE,PWM_OUT_1_BIT, true);

 //   PWMOutputState(PWM1_BASE,PWM_OUT_5_BIT , true);

    PWMGenEnable(PWM0_BASE, PWM_GEN_0);
    PWMGenEnable(PWM0_BASE, PWM_GEN_1);

    PWMPulseWidthSet(PWM0_BASE, PWM_OUT_0,1);  //%0  //B6
    PWMPulseWidthSet(PWM0_BASE, PWM_OUT_1,1);  //%0  //B7
//    PWMPulseWidthSet(PWM1_BASE, PWM_OUT_5,(PWMGenPeriodGet(PWM1_BASE, PWM_GEN_2)*PWM_DUTY) / 100);
//    PWMPulseWidthSet(PWM0_BASE, PWM_OUT_0,PWMGenPeriodGet(PWM0_BASE)/100);  //%1  //B6
//    PWMPulseWidthSet(PWM0_BASE, PWM_OUT_1,PWMGenPeriodGet(PWM0_BASE)/100);  //%1  //B7
    //
}


//占空比设置
//int32_t Duty ：-10000<Val<10000
//PB6输出的占空比 Val/10000; PB7输出的占空比 自定义
//eg：PWMDutyCycleSet(5000) => PB6占空比为50%
// Val>0 : return true
// Val<0 : return false
//频率 ： 1Khz
bool PWMDutyCycleSet(int32_t Val)
{
    int32_t pulse_width = Val;

    if(pulse_width < 0)
    {
        pulse_width = -pulse_width;
    }
    else if(pulse_width == 0)
    {
        // Can't be zero
        pulse_width = 1;
    }

    pulse_width = pulse_width > PWMPERIOD ? PWMPERIOD : pulse_width ;

    if(Val>=0)
    {


        return true;   //正方向标识
    }
    else
    {

        return false;   //反方向标识
    }

}

//12/21 / 18/38
//* -100<=Duty<=100 占空比
void PWMOUTSet(int16_t Duty1)
{
    //PWM test
    int16_t PWMVal1;
    if(Duty1>PWMPERIOD)          //对Duty1和Duty2进行限幅
    {
        Duty1=PWMPERIOD;
    }else if(Duty1<-PWMPERIOD)
    {
        Duty1=-PWMPERIOD;
    }else if(Duty1==0)
        Duty1=1;

//    PWMVal1= (int)(Duty1*(PWMPERIOD/100));
//    PWMVal2= (int)(100*(Duty2/PWMPERIOD));
    PWMVal1= (int16_t)Duty1;

    if(PWMDutyCycleSet(PWMVal1))
    {

        PWMPulseWidthSet(PWM0_BASE,PWM_OUT_0,PWMVal1);
		PWMPulseWidthSet(PWM0_BASE,PWM_OUT_1,1);  //%0  //B7
    }
    else if( !PWMDutyCycleSet(PWMVal1) )
    {
        PWMPulseWidthSet(PWM0_BASE,PWM_OUT_0,1);
		PWMPulseWidthSet(PWM0_BASE, PWM_OUT_1,-PWMVal1);  //%0  //B7

     }



}

