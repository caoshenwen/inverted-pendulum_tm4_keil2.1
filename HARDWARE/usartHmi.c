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

#if EN_UART1_RX   //���ʹ���˽���
 	
uint8_t UART1_RX_BUF[UART1_REC_LEN];     //���ջ���,���USART_REC_LEN���ֽ�.
//����״̬
//bit15��	������ɱ�־
//bit14��	���յ�0x0d
//bit13~0��	���յ�����Ч�ֽ���Ŀ
uint16_t UART1_RX_STA=0;       //����״̬���	

bool USARTHMI_CMD_STA =false;
//��ʼ��IO ����1 
//bound:������
void UART1_Init(uint32_t bound)
{
   //����UART
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOC);
    SysCtlPeripheralEnable(SYSCTL_PERIPH_UART1);
    //����PA0��PA1Ϊ����0����
    GPIOPinConfigure(GPIO_PC4_U1RX);
    GPIOPinConfigure(GPIO_PC5_U1TX);
    GPIOPinTypeUART(GPIO_PORTC_BASE, GPIO_PIN_4 | GPIO_PIN_5);
     //����UART1Ϊ115200,8-N-1
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

     //ʹ�ܴ����ж�
     IntEnable(INT_UART1);
     UARTIntEnable(UART1_BASE, UART_INT_RX | UART_INT_RT); // �򿪽����жϺͽ��ճ�ʱ�ж�
     //������ʾ��Ϣ
     //UARTSend((uint8_t *)"AAAAAAAAAABBBBBBbbbbbbMMMM\r", sizeof("AAAAAAAAAABBBBBBbbbbbbMMMM\r"));
}

void UART1IntHandler(void) // ����0�жϷ������
{
	uint8_t Res;
    uint32_t ulStatus;  // �����жϺͽ��ճ�ʱ�жϵı�־
    uint32_t status_value = 0;
    status_value =taskENTER_CRITICAL_FROM_ISR();
    // ��ȡ�ж�״̬
    ulStatus = UARTIntStatus(UART1_BASE, true);
    // ����жϱ�־ 
    UARTIntClear(UART1_BASE, ulStatus);


    // ֱ������FIFO��û������ʱ���˳�ѭ��
    while(UARTCharsAvail(UART1_BASE)) //�����ж�(���յ������ݱ�����0x0d 0x0a��β)
	{
	
		Res =UARTCharGetNonBlocking(UART1_BASE);//(USART3->DR);	//��ȡ���յ�������
		if((UART1_RX_STA&0x8000)==0)//����δ���
		{
			if(UART1_RX_STA&0x4000)//���յ���0xff
			{
				if(Res!=0x0a)UART1_RX_STA=0;//���մ���,���¿�ʼ
				else 
				{
					UART1_RX_STA|=0x8000;	//���������

				//���������ʹ�����������ȡ�ź�
					USARTHMI_CMD_STA = true;

				}					
			}
			else //��û�յ�0Xff
			{	
				if(Res==0x0d)UART1_RX_STA|=0x4000;
				else
				{
					UART1_RX_BUF[UART1_RX_STA&0X3FFF]=Res ;
					UART1_RX_STA++;

					if(UART1_RX_STA>(UART1_REC_LEN-1))UART1_RX_STA=0;//�������ݴ���,���¿�ʼ����	  
				}		 
			}
		}   		 
	} 
	taskEXIT_CRITICAL_FROM_ISR(status_value);
} 

#endif	

//������������ȡ����,�ټӸ��ṹ�����
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
	//��Ҫ���͵��ַ�д��UART
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

int SendEnd(void)//����������
{
	SendByteToHmi(0XFF);
	SendByteToHmi(0XFF);
	SendByteToHmi(0XFF);
		
	return 0;
}
