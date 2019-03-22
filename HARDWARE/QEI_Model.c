/*
 * QEI_Model.c
 *
 *  Created on: 2018��1��29��
 *      Author: bobo
 */

#include <stdint.h>
#include <stdbool.h>
//#include "inc/tm4c123gh6pm.h"
#include "driverlib/sysctl.h"
#include "driverlib/gpio.h"
#include "driverlib/pin_map.h"
#include "driverlib/qei.h"
#include "inc/hw_memmap.h"
#include "inc/hw_gpio.h"
#include "driverlib/interrupt.h"
#include "inc/hw_types.h"
#include "QEI_Model.h"

// * note
// *���: 8r/s 13�� ���ٱ� 1:30
// *һȦ���������390��13*30��������
// *390x4 = 1560
#define MOTOR_QEI_VEL_FREQ      10

bool Init_flag = true;

//void QEI_InitConfig(user_qei_data_t *qei_data);
void QEI_Config(void);

user_qei_data_t qei_data_array[2] =
{
        {        //QEI0: D6,D7
                .periph = SYSCTL_PERIPH_QEI0,
                .base = QEI0_BASE,
                .swap_flag = QEI_CONFIG_SWAP,
                .Qinterrupt = INT_QEI0,

                .gpio_periph = SYSCTL_PERIPH_GPIOD,
                .gpio_port = GPIO_PORTD_BASE,
                .gpio_pin = (GPIO_PIN_6 | GPIO_PIN_7),
                .gpio_cfg_a = GPIO_PD6_PHA0,
                .gpio_cfg_b = GPIO_PD7_PHB0,

                .dir =1,         //����
                .velocity = 0,    //����
                .Init_Pos = 0,    //��ʼλ��
                .Now_Pos = 0,     //λ��
        },

        {       //QEI1��C5,C6
                .periph = SYSCTL_PERIPH_QEI1,
                .base = QEI1_BASE,
                .swap_flag = QEI_CONFIG_NO_SWAP,
                .Qinterrupt = INT_QEI1,

                .gpio_periph = SYSCTL_PERIPH_GPIOC,
                .gpio_port = GPIO_PORTC_BASE,
                .gpio_pin = (GPIO_PIN_5 | GPIO_PIN_6),
                .gpio_cfg_a = GPIO_PC5_PHA1,
                .gpio_cfg_b = GPIO_PC6_PHB1,

                .dir = 1,
                .velocity = 0,
                .Init_Pos = 0,
                .Now_Pos = 0,
        },
};


void QEI_InitConfig(user_qei_data_t *qei_data)
{
    //
    // Enable the QEI0 peripheral
    //
    SysCtlPeripheralEnable(qei_data->periph);
    //
    // Wait for the QEI0 module to be ready.
    //
    while(!SysCtlPeripheralReady(qei_data->periph));
    //
    // Configure gpio
    //
    SysCtlPeripheralEnable(qei_data->gpio_periph);
    while(!SysCtlPeripheralReady(qei_data->gpio_periph));

    if(qei_data->gpio_port == GPIO_PORTD_BASE && (qei_data->gpio_pin & GPIO_PIN_7))
    {
        //
        // unlock PF7
        //
        HWREG(GPIO_PORTD_BASE + GPIO_O_LOCK) = 0x4C4F434B;
        HWREG(GPIO_PORTD_BASE + GPIO_O_CR) |= GPIO_PIN_7;
        HWREG(GPIO_PORTD_BASE + GPIO_O_LOCK) = 0x00;
    }

    GPIOPinConfigure(qei_data->gpio_cfg_a);
    GPIOPinConfigure(qei_data->gpio_cfg_b);

    GPIOPinTypeQEI(qei_data->gpio_port, qei_data->gpio_pin);


    //1560 -> 0x618
    QEIConfigure(qei_data->base, (QEI_CONFIG_CAPTURE_A_B | QEI_CONFIG_NO_RESET | QEI_CONFIG_QUADRATURE | qei_data->swap_flag), 0x618-1);
    //
    // Enable the quadrature encoder.
    //
    QEIEnable(qei_data->base);
    //
    // Configure qei velocity ��ת��
    //
    QEIVelocityConfigure(qei_data->base, QEI_VELDIV_1, (SysCtlClockGet() /1000)* MOTOR_QEI_VEL_FREQ);
    QEIVelocityEnable(qei_data->base);

    QEIIntRegister(qei_data->base,QEI0_IRQHandler);
    QEIIntEnable(qei_data->base, QEI_INTDIR | QEI_INTTIMER);
    IntEnable(qei_data->Qinterrupt);
    //INT_QEI0
//    IntRegister(INT_QEI0,QEI0_IRQHandler);
}


void QEI_Config(void)
{

   QEI_InitConfig(qei_data_array+0);  //ֻ����QEI 0
   //SysCtlDelay(SysCtlClockGet()*5/3000);  //��ʱһ��ʱ��

}

//λ����Ϣ�Ķ�ȡ 0~1560 ==תһȦ��1560������ ==ÿ������0.2308��
//�ٶ���Ϣ�Ķ�ȡ            ==������/��
void qei_irq_handler(int32_t QEInum)
{
    uint32_t status = QEIIntStatus(qei_data_array[QEInum].base, true);
    QEIIntClear(qei_data_array[QEInum].base, status);

    if(status & QEI_INTTIMER)
    {
    qei_data_array[QEInum].velocity = QEIVelocityGet(qei_data_array[QEInum].base)  * QEIDirectionGet(qei_data_array[QEInum].base);
    //qei_data_array[QEInum].Now_Pos = QEIPositionGet(qei_data_array[QEInum].base);
    }

    if(status & QEI_INTDIR)
    {
        qei_data_array[QEInum].dir = -qei_data_array[QEInum].dir;
    }
    //��ʼλ��
    if(Init_flag)
    {
        qei_data_array[QEInum].Init_Pos = QEIPositionGet(qei_data_array[QEInum].base);
        Init_flag = false;
    }
	
	
    //��ȡλ�ú��ٶ���Ϣ
//    qei_data_array[QEInum].Now_Pos = QEIPositionGet(qei_data_array[QEInum].base);
//    qei_data_array[QEInum].velocity = QEIVelocityGet(qei_data_array[QEInum].base) * MOTOR_QEI_VEL_FREQ * QEIDirectionGet(qei_data_array[QEInum].base);
}


void QEI0_IRQHandler(void)
{
    qei_irq_handler(0);

}
//void QEI1_IRQHandler(void)
//{
//    qei_irq_handler(1);
//
//}
