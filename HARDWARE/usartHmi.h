#ifndef __USARTHMI_H
#define __USARTHMI_H
#include "stdio.h"	
#include <stdint.h>
#include <stdbool.h>
#include "inc/hw_memmap.h"
#include "inc/hw_types.h"
#include "inc/hw_ints.h"

#define UART1_REC_LEN  			200  	//定义最大接收字节数 200
#define EN_UART1_RX 			1		//使能（1）/禁止（0）串口1接收
typedef enum Hmi_err
{
	HMI_ERR_NONE
	
}HMI_ERR;

typedef struct usartHmi
{
	uint8_t command[1];
	uint16_t hmiValue;
}USARTHMI;
	  	
extern uint8_t  UART1_RX_BUF[UART1_REC_LEN]; //接收缓冲,最大USART_REC_LEN个字节.末字节为换行符 
extern uint16_t UART1_RX_STA;         		//接收状态标记	

extern bool USARTHMI_CMD_STA;
void UART1_Init(uint32_t bound);
void UART1IntHandler(void);
void GetUsarthmiCmd (uint8_t *pUsartRxBuff, uint8_t *pErr);
int SendByteToHmi(uint8_t data);
int SendStrToHmi(uint8_t *str);
int SendNumToHmi(uint16_t num);
int SendEnd(void);//结束符发送

#endif


