#include "usr_tasks.h"
#include "my_include.h"

#ifdef DEBUG
void
__error__(char *pcFilename, uint32_t ui32Line)
{
}

#endif




#define START_TASK_PRIO				1
#define START_STK_SIZE				128
void start_task(void * pvParameters); 
TaskHandle_t StartTask_Handler;
 

#define TASK_KEY_PRIO			3
#define TASK_KEY_STK_SIZE		128
void task_key(void * pvParameters);
TaskHandle_t TaskKEY_Handler;	

#define TASK_BLINKY_PRIO			4
#define TASK_BLINKY_STK_SIZE		128
void task_blinky(void * pvParameters);
TaskHandle_t TaskBlinky_Handler;

#define TASK_ADC_PRIO			5
#define TASK_ADC_STK_SIZE		128
void task_adc(void * pvParameters);
TaskHandle_t TaskADC_Handler ;

#define TASK_PWM_PRIO			6
#define TASK_PWM_STK_SIZE		128
void task_pwm(void * pvParameters);
TaskHandle_t TaskPWM_Handler;


#define TASK_QEI_PRIO			7
#define TASK_QEI_STK_SIZE		128
void task_qei(void * pvParameters);
TaskHandle_t TaskQEI_Handler;

#define TASK_main_PRIO			8
#define TASK_main_STK_SIZE		128
void task_main(void * pvParameters);
TaskHandle_t TaskMain_Handler;

 



int main(void)
{	
	
    BSP_SysInit();
	
	LED_Init();
	ADC_Init();
	InitConsole();
	KEY_init();
	PWM_Config();
	QEI_Config();
	PID_Speed_Init();
	
    UARTprintf("\n\nThis is a adc example!\n\n");
	
	xTaskCreate((TaskFunction_t	) start_task,
				(char*			) "start_task",
				(uint16_t		) START_STK_SIZE,
				(void * 		) NULL,
				(UBaseType_t	) START_TASK_PRIO,
				(TaskHandle_t*	) &StartTask_Handler);
				
    vTaskStartScheduler();
}

void start_task(void * pvParameters)
{
	taskENTER_CRITICAL();
	
	
	
	xTaskCreate((TaskFunction_t	) task_blinky,
				(char*			) "task_blinky",
				(uint16_t		) TASK_BLINKY_STK_SIZE,
				(void * 		) NULL,
				(UBaseType_t	) TASK_BLINKY_PRIO,
				(TaskHandle_t*	) &TaskBlinky_Handler);
				
				
	xTaskCreate((TaskFunction_t	) task_adc,
				(char*			) "task_adc",
				(uint16_t		) TASK_ADC_STK_SIZE,
				(void * 		) NULL,
				(UBaseType_t	) TASK_ADC_PRIO,
				(TaskHandle_t*	) &TaskADC_Handler);
				
				
	xTaskCreate((TaskFunction_t	) task_key,
				(char*			) "task_key",
				(uint16_t		) TASK_KEY_STK_SIZE,
				(void * 		) NULL,
				(UBaseType_t	) TASK_KEY_PRIO,
				(TaskHandle_t*	) &TaskKEY_Handler);
				
				
	xTaskCreate((TaskFunction_t	) task_pwm,
				(char*			) "task_pwm",
				(uint16_t		) TASK_PWM_STK_SIZE,
				(void * 		) NULL,
				(UBaseType_t	) TASK_PWM_PRIO,
				(TaskHandle_t*	) &TaskPWM_Handler);
				
	xTaskCreate((TaskFunction_t	) task_qei,
				(char*			) "task_qei",
				(uint16_t		) TASK_QEI_STK_SIZE,
				(void * 		) NULL,
				(UBaseType_t	) TASK_QEI_PRIO,
				(TaskHandle_t*	) &TaskQEI_Handler);
					
	xTaskCreate((TaskFunction_t	) task_main,
				(char*			) "task_main",
				(uint16_t		) TASK_main_STK_SIZE,
				(void * 		) NULL,
				(UBaseType_t	) TASK_main_PRIO,
				(TaskHandle_t*	) &TaskMain_Handler);


	
				
	vTaskDelete(StartTask_Handler);
	taskEXIT_CRITICAL(); 
}

//*****************************************************************************
// This hook is called by FreeRTOS when an stack overflow error is detected.
//*****************************************************************************
void
vApplicationStackOverflowHook(xTaskHandle *pxTask, char *pcTaskName)
{
    // This function can not return, so loop forever.  Interrupts are disabled
    // on entry to this function, so no processor interrupts will interrupt
    // this loop.
    while(1){
		;
    }
}

