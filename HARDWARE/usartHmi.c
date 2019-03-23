#include "usartHmi.h"	
#include "my_include.h"

#include <stdint.h>
#include <stdbool.h>
#include "inc/hw_memmap.h"
#include "inc/hw_types.h"
#include "inc/hw_ints.h"
#include "driverlib/sysctl.h"
#include "driverlib/gpio.h"
#include "driverlib/uart.h"
#include "driverlib/fpu.h"
#include "driverlib/pin_map.h"
#include "driverlib/interrupt.h"

#if EN_UART1_RX   //如果使能了接收
 	
uint8_t UART1_RX_BUF[UART1_REC_LEN];     //接收缓冲,最大USART_REC_LEN个字节.
//接收状态
//bit15，	接收完成标志
//bit14，	接收到0x0d
//bit13~0，	接收到的有效字节数目
uint16_t UART1_RX_STA=0;       //接收状态标记	

bool USARTHMI_CMD_STA =false;
//初始化IO 串口1 
//bound:波特率
void UART1_Init(uint32_t bound)
{
   //配置UART
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOC);
    SysCtlPeripheralEnable(SYSCTL_PERIPH_UART1);
    //配置PA0和PA1为串口0引脚
    GPIOPinConfigure(GPIO_PC4_U1RX);
    GPIOPinConfigure(GPIO_PC5_U1TX);
    GPIOPinTypeUART(GPIO_PORTC_BASE, GPIO_PIN_4 | GPIO_PIN_5);
     //配置UART1为115200,8-N-1
     UARTConfigSetExpClk(UART1_BASE, SysCtlClockGet(), bound,(UART_CONFIG_WLEN_8 |
           UART_CONFIG_STOP_ONE | UART_CONFIG_PAR_NONE));
     //
     // Set the priority of uart interrupt, lowest to highest:0xE0 - 0x0.
     // This we set it to lowest priority.
     //
     IntPrioritySet (INT_UART1, 0xE0);

    //
    // Register the interrupt handler dynamic.
    //
     UARTIntRegister(UART1_BASE, UART1IntHandler);

     //使能串口中断
     IntEnable(INT_UART1);
     UARTIntEnable(UART1_BASE, UART_INT_RX | UART_INT_RT); // 打开接收中断和接收超时中断
     //发送提示信息
     //UARTSend((uint8_t *)"AAAAAAAAAABBBBBBbbbbbbMMMM\r", sizeof("AAAAAAAAAABBBBBBbbbbbbMMMM\r"));
}

void UART1IntHandler(void) // 串口0中断服务程序
{
	uint8_t Res;
    uint32_t ulStatus;  // 接收中断和接收超时中断的标志
    uint32_t status_value = 0;
    status_value =taskENTER_CRITICAL_FROM_ISR();
    // 获取中断状态
    ulStatus = UARTIntStatus(UART1_BASE, true);
    // 清除中断标志 
    UARTIntClear(UART1_BASE, ulStatus);


    // 直到串口FIFO中没有数据时才退出循环
    while(UARTCharsAvail(UART1_BASE)) //接收中断(接收到的数据必须是0x0d 0x0a结尾)
	{
	
		Res =UARTCharGetNonBlocking(UART1_BASE);//(USART3->DR);	//读取接收到的数据
		if((UART1_RX_STA&0x8000)==0)//接收未完成
		{
			if(UART1_RX_STA&0x4000)//接收到了0xff
			{
				if(Res!=0x0a)UART1_RX_STA=0;//接收错误,重新开始
				else 
				{
					UART1_RX_STA|=0x8000;	//接收完成了

				//向主程序发送串口屏命令提取信号
					USARTHMI_CMD_STA = true;

				}					
			}
			else //还没收到0Xff
			{	
				if(Res==0x0d)UART1_RX_STA|=0x4000;
				else
				{
					UART1_RX_BUF[UART1_RX_STA&0X3FFF]=Res ;
					UART1_RX_STA++;

					if(UART1_RX_STA>(UART1_REC_LEN-1))UART1_RX_STA=0;//接收数据错误,重新开始接收	  
				}		 
			}
		}   		 
	} 
	taskEXIT_CRITICAL_FROM_ISR(status_value);
} 

#endif	

//串口屏命令提取函数,再加个结构体输出
void GetUsarthmiCmd (uint8_t UsartRxBuff[], uint8_t *pErr)
{

	uint8_t *pRxBuff = UsartRxBuff;

	if((pRxBuff[0]&(uint16_t)0x01FF) != 'p')
	{
		*pErr =0;
	}
	else 
	{
		switch(pRxBuff[1]&(uint16_t)0x01FF)
		{
            case 'L':
            GPIOPinWrite(GPIO_PORTF_BASE,GPIO_PIN_1|GPIO_PIN_2,GPIO_PIN_2|GPIO_PIN_1);
            SysCtlDelay(300*(SysCtlClockGet()/3000));
            GPIOPinWrite(GPIO_PORTF_BASE,GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3,0x00);
            SysCtlDelay(300*(SysCtlClockGet()/3000));
            GPIOPinWrite(GPIO_PORTF_BASE,GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3,GPIO_PIN_2|GPIO_PIN_3);
            SysCtlDelay(300*(SysCtlClockGet()/3000));
            GPIOPinWrite(GPIO_PORTF_BASE,GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3,0x00);
            SysCtlDelay(300*(SysCtlClockGet()/3000));

            break;

            default:
                UARTprintf("Error!No this command!");
                *pErr =1;
            break;
		}
	}
	UART1_RX_STA =0;
}

int SendByteToHmi(uint8_t data)
{
	//将要发送的字符写进UART
	//UARTCharPutNonBlocking(UART1_BASE, data);
	UARTCharPut(UART1_BASE, data);	
	return 0;
}

int SendStrToHmi(uint8_t *str)
{
 int i=0;
 while(str[i]!=0)
 {
 	SendByteToHmi(str[i]);
 	i++;
 }	
	return 0;
}

int SendNumToHmi(uint16_t num)
{
	uint16_t numTemp;
	uint16_t kilobit=0, hundreds=0, tens=0, units=0;
	
	numTemp = num;
	kilobit = numTemp/1000;
	numTemp = numTemp%1000;
	hundreds = numTemp/100;
	numTemp = numTemp%100;
	tens = numTemp/10;
	numTemp = numTemp%10;
	units = numTemp;
	SendByteToHmi('"');	
	SendByteToHmi(kilobit+48);
	SendByteToHmi(hundreds+48);
	SendByteToHmi(tens+48);
	SendByteToHmi(units+48);	
	SendByteToHmi('"');	
	return 0;	
}

int SendEnd(void)//结束符发送
{
	SendByteToHmi(0XFF);
	SendByteToHmi(0XFF);
	SendByteToHmi(0XFF);
		
	return 0;
}
