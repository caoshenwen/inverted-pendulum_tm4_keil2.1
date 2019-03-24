#include "show.h"
#include "DataScope_DP.h"
#include "my_include.h"


unsigned char i,temp;          //计数变量
unsigned char Send_Count; //串口需要发送的数据个数
float Vol;
int Clear_Count;
/**************************************************************************
函数功能：虚拟示波器往上位机发送数据 关闭显示屏
入口参数：无
返回  值：无
作    者：平衡小车之家
**************************************************************************/
void DataScope(void)
{   
		DataScope_Get_Channel_Data( angle, 1 );      
		DataScope_Get_Channel_Data(PWM , 2 );         
		DataScope_Get_Channel_Data(velocity_A, 3 );              
		DataScope_Get_Channel_Data(BalancePWM, 4 );   
		DataScope_Get_Channel_Data(SpeedPWM, 5 ); //用您要显示的数据替换0就行了
//		DataScope_Get_Channel_Data(0 , 6 );//用您要显示的数据替换0就行了
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


