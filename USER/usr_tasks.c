#include "usr_tasks.h"
#include "my_include.h"



int16_t pwm_value;

void task_blinky(void * pvParameters)
{
	
	portTickType ui32WakeTime;
    ui32WakeTime = xTaskGetTickCount();
	//int i;
	
	while(1)
	{	
			switch(KeyPress)
			{
				case 0:
					
					GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_1, GPIO_PIN_1-GPIOPinRead(GPIO_PORTF_BASE,GPIO_PIN_1));
				    GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_2|GPIO_PIN_3, 0);
					break;
				
				case 1:
					GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_2, GPIO_PIN_2-GPIOPinRead(GPIO_PORTF_BASE,GPIO_PIN_2));
				    GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_1|GPIO_PIN_3, 0);
					break;
					
				case 2:
					GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_3, GPIO_PIN_3-GPIOPinRead(GPIO_PORTF_BASE,GPIO_PIN_3));
				    GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_1|GPIO_PIN_2, 0);
					break;
				
				case 3:
					GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_1|GPIO_PIN_2, 
				                 (GPIO_PIN_1-GPIOPinRead(GPIO_PORTF_BASE,GPIO_PIN_1))|
				                 (GPIO_PIN_2-GPIOPinRead(GPIO_PORTF_BASE,GPIO_PIN_2)));
				    GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_3, 0);
					break;
					
			   default :  break;		
			}
		
		vTaskDelayUntil(&ui32WakeTime, 100 / portTICK_RATE_MS);			
	}
}


void task_main(void * pvParameters)
{
	// Get the current tick count.
	portTickType ui32WakeTime;
    ui32WakeTime = xTaskGetTickCount();
	
	while(1)
	{	
		switch(KeyPress)	
		{	
			case 0: 
				pwm_value=PIDSpeed_Realize(angle,0);
			
				UARTprintf("                                                  pid£º%d  \r\n",pwm_value);
				vTaskDelayUntil(&ui32WakeTime, 10 / portTICK_RATE_MS);		
				break;
			
		    case 1: vTaskDelayUntil(&ui32WakeTime, 10 / portTICK_RATE_MS); break;
			
			case 2: vTaskDelayUntil(&ui32WakeTime, 10 / portTICK_RATE_MS); break;
			
			
			case 3: vTaskDelayUntil(&ui32WakeTime, 10 / portTICK_RATE_MS); break;
			
			
			case 4: vTaskDelayUntil(&ui32WakeTime, 10 / portTICK_RATE_MS); break;
			
			default: vTaskDelayUntil(&ui32WakeTime, 10 / portTICK_RATE_MS); break ;
		}
		
		
		
		   
	}
}




void task_adc(void * pvParameters)
{
	// Get the current tick count.
	portTickType ui32WakeTime;
    ui32WakeTime = xTaskGetTickCount();
	
	while(1)
	{	
			ADC_Trig();
		    UARTprintf("½Ç¶È£º %d\r\n",angle);
			vTaskDelayUntil(&ui32WakeTime, 10 / portTICK_RATE_MS);
	}
}



void task_key(void * pvParameters)
{
	// Get the current tick count.
	portTickType ui32WakeTime;
    ui32WakeTime = xTaskGetTickCount();
	
	while(1)
	{	
            key_scan();
			vTaskDelayUntil(&ui32WakeTime, 10 / portTICK_RATE_MS);
	}
}



void task_pwm(void * pvParameters)
{
	// Get the current tick count.
	portTickType ui32WakeTime;
    ui32WakeTime = xTaskGetTickCount();
	
	while(1)
	{	
            PWMOUTSet(2000);
			vTaskDelayUntil(&ui32WakeTime, 10 / portTICK_RATE_MS);		
	}
}


void task_qei(void * pvParameters)
{
	// Get the current tick count.
	portTickType ui32WakeTime;
    ui32WakeTime = xTaskGetTickCount();
	
	while(1)
	{	
            UARTprintf("                   ËÙ¶È£º%d  \r\n",qei_data_array[0].velocity);
			vTaskDelayUntil(&ui32WakeTime, 10 / portTICK_RATE_MS);		
	}
}
