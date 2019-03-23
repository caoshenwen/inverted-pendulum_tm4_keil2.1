#ifndef __USARTHMI_H
#define __USARTHMI_H
#include "stdio.h"	
#include <stdint.h>
#include <stdbool.h>
#include "inc/hw_memmap.h"
#include "inc/hw_types.h"
#include "inc/hw_ints.h"

#define UART1_REC_LEN  			200  	//�����������ֽ��� 200
#define EN_UART1_RX 			1		//ʹ�ܣ�1��/��ֹ��0������1����
typedef enum Hmi_err
{
	HMI_ERR_NONE
	
}HMI_ERR;

typedef struct usartHmi
{
	uint8_t command[1];
	uint16_t hmiValue;
}USARTHMI;
	  	
extern uint8_t  UART1_RX_BUF[UART1_REC_LEN]; //���ջ���,���USART_REC_LEN���ֽ�.ĩ�ֽ�Ϊ���з� 
extern uint16_t UART1_RX_STA;         		//����״̬���	

extern bool USARTHMI_CMD_STA;
void UART1_Init(uint32_t bound);
void UART1IntHandler(void);
void GetUsarthmiCmd (uint8_t *pUsartRxBuff, uint8_t *pErr);
int SendByteToHmi(uint8_t data);
int SendStrToHmi(uint8_t *str);
int SendNumToHmi(uint16_t num);
int SendEnd(void);//����������

#endif


