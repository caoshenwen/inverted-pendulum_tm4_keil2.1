#include "show.h"
#include "DataScope_DP.h"
#include "my_include.h"


unsigned char i,temp;          //��������
unsigned char Send_Count; //������Ҫ���͵����ݸ���
float Vol;
int Clear_Count;
/**************************************************************************
�������ܣ�����ʾ��������λ���������� �ر���ʾ��
��ڲ�������
����  ֵ����
��    �ߣ�ƽ��С��֮��
**************************************************************************/
void DataScope(void)
{   
		DataScope_Get_Channel_Data( angle, 1 );      
		DataScope_Get_Channel_Data(PWM , 2 );         
		DataScope_Get_Channel_Data(velocity_A, 3 );              
		DataScope_Get_Channel_Data(BalancePWM, 4 );   
		DataScope_Get_Channel_Data(SpeedPWM, 5 ); //����Ҫ��ʾ�������滻0������
//		DataScope_Get_Channel_Data(0 , 6 );//����Ҫ��ʾ�������滻0������
//		DataScope_Get_Channel_Data(0, 7 );
//		DataScope_Get_Channel_Data( 0, 8 ); 
//		DataScope_Get_Channel_Data(0, 9 );  
//		DataScope_Get_Channel_Data( 0 , 10);
		Send_Count = DataScope_Data_Generate(5);
		for( i = 0 ; i < Send_Count; i++) 
		{
			//while((USART1->SR&0X40)==0);  
			UARTCharPut(UART0_BASE, DataScope_OutPut_Buffer[i]);	
			//UARTprintf("%c",a);

		}
}


