/*******************************************************************************
* File Name : main.c
* Author    : bmrtech
* Version   : 1.0.1
* Date      : 2019/11/3
* Brief     : Main program module
*******************************************************************************/

/* Kernel includes. */
#include "FreeRTOS.h" /* Must come first.                          */
#include "task.h"     /* RTOS task related API prototypes.         */
#include "queue.h"    /* RTOS queue related API prototypes.        */
#include "timers.h"   /* Software timer related API prototypes.    */
#include "semphr.h"   /* Semaphore related API prototypes.         */

/* Add any manufacture supplied header files can be included here. */
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

/* Device header files. */
#include "n200_func.h"
#include "riscv_encoding.h"
#include "n200_timer.h"

/* The standard c library. */
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

/* BSP */
#include "timers.h"
#include "gd32vf103.h"
#include "bsp.h"
#include "systick.h"

/* The period of the example software timer, specified in milliseconds, and
converted to ticks using the pdMS_TO_TICKS() macro. */
#define mainSOFTWARE_TIMER_PERIOD_MS        pdMS_TO_TICKS( 1000 )

#define TFTLED_TASK_PRIORITY    ( tskIDLE_PRIORITY + 4  )     /* Start task2's priority  */
#define Time_TASK_PRIORITY     ( tskIDLE_PRIORITY + 5 )      /* Producer task's priority        */
#define LED_R_TASK_PRIORITY     ( tskIDLE_PRIORITY + 6  )     /* Start task1's priority  */
#define LED_G_TASK_PRIORITY     ( tskIDLE_PRIORITY + 6  )     /* Start task2's priority  */
#define LED_B_TASK_PRIORITY     ( tskIDLE_PRIORITY + 6  )     /* Start task2's priority  */

/* Handle to Queue  */
QueueHandle_t  Time_R_Queue;
QueueHandle_t  Time_G_Queue;
QueueHandle_t  Time_B_Queue;

QueueHandle_t  TFT_Time_Queue;
QueueHandle_t  TFT_ledR_Queue;
QueueHandle_t  TFT_ledG_Queue;
QueueHandle_t  TFT_ledB_Queue;

/* Handle to tasks  */
TaskHandle_t Time_Handlre;
TaskHandle_t LED_R_Handle;
TaskHandle_t LED_G_Handle;
TaskHandle_t LED_B_Handle;
TaskHandle_t TFTLED_Handle;

SemaphoreHandle_t RGBled_BinSem_Handle = NULL;

/***********************Local Function Prototypes******************************/
void Time_task( void *pvParameters );

void LED_R_task(void *pvParameters);
void LED_G_task(void *pvParameters);
void LED_B_task(void *pvParameters);

void TFTLED_task(void *pvParameters);
//void Counter_task(void *pvParameters);


/*******************************************************************************
* @brief       Main program
* @param[in]   None
* @param[out]  None
* @RetVal      None
*******************************************************************************/
int main(void)
{  

	/* Configure the system ready to run the demo.  The clock configuration
	    can be done here if it was not done before main() was called. */

	/* Initialize the trace recorder and start trace. */
//	vTraceEnable(TRC_START);
    bsp_Init();

    RGBled_BinSem_Handle = xSemaphoreCreateBinary();
 	xSemaphoreGive(RGBled_BinSem_Handle);


    TFT_Time_Queue = xQueueCreate( 1 , sizeof( int16_t ) );
    TFT_ledR_Queue = xQueueCreate( 1 , sizeof( int16_t ) );
    TFT_ledG_Queue = xQueueCreate( 1 , sizeof( int16_t ) );
    TFT_ledB_Queue = xQueueCreate( 1 , sizeof( int16_t ) );

    Time_R_Queue = xQueueCreate( 1 , sizeof( int16_t ));
    Time_G_Queue = xQueueCreate( 1 , sizeof( int16_t ));
    Time_B_Queue = xQueueCreate( 1 , sizeof( int16_t ));


    xTaskCreate((TaskFunction_t )Time_task,
    		    ( const char*   )"Time_task",
			    (uint16_t       )250,
			    (void*          )NULL,
			    (UBaseType_t    )Time_TASK_PRIORITY,
			    (TaskHandle_t*  )NULL );


    xTaskCreate((TaskFunction_t )LED_R_task,
    		    ( const char*   )"LED_R_task",
				(uint16_t       )250,
				(void*          )NULL,
				(UBaseType_t    )LED_R_TASK_PRIORITY,
				(TaskHandle_t*  )NULL );

    xTaskCreate((TaskFunction_t )LED_G_task,
    		    ( const char*   )"LED_G_task",
				(uint16_t       )250,
				(void*          )NULL,
				(UBaseType_t    )LED_G_TASK_PRIORITY,
				(TaskHandle_t*  )NULL );

    xTaskCreate((TaskFunction_t )LED_B_task,
    		    ( const char*   )"LED_G_task",
				(uint16_t       )250,
				(void*          )NULL,
				(UBaseType_t    )LED_B_TASK_PRIORITY,
				(TaskHandle_t*  )NULL );

    xTaskCreate((TaskFunction_t )TFTLED_task,
    		    ( const char*   )"TFTLED_task",
				(uint16_t       )250,
				(void*          )NULL,
				(UBaseType_t    )TFTLED_TASK_PRIORITY,
				(TaskHandle_t*  )NULL );

    printf("Before StartScheduler\r\n");

    vTaskStartScheduler();

    for( ;; )
    {
        ;
    };

}



/*******************************************************************************
* @brief       Time_task function
* @param[in]   None
* @param[out]  None
* @RetVal      None
*******************************************************************************/
void Time_task( void *pvParameters )
{
	int16_t SendNum = 0;
    for( ;; )
    {
         /* send data to queue */
    	 printf("Producer send number.\r\n");
         SendNum++;

         xQueueOverwrite( Time_R_Queue, ( void* )&SendNum );
         xQueueOverwrite( Time_G_Queue, ( void* )&SendNum );
         xQueueOverwrite( Time_B_Queue, ( void* )&SendNum );
         xQueueSend( TFT_Time_Queue, ( void* )&SendNum, 0 );

         vTaskDelay( mainSOFTWARE_TIMER_PERIOD_MS );

     }
}



/*******************************************************************************
* @brief       TFTLED_task function
* @param[in]   None
* @param[out]  None
* @RetVal      None
*******************************************************************************/
void TFTLED_task(void *pvParameters)
{

	int16_t ReceiveNum = 0;

    LCD_ShowString(0, 0, (u8 *)("sys_times:"), BLACK);
    Draw_Circle(8, 24, 8, RED);
    Draw_Circle(8, 40, 8, GREEN);
    Draw_Circle(8, 56, 8, BLUE);

    for( ;; )
    {
		   if( xQueueReceive( TFT_Time_Queue, &ReceiveNum, 10) == pdPASS)
	       {
			   printf("Customer receive number:%d\r\n",ReceiveNum);
			   LCD_ShowNum_int(88, 0, (uint16_t)ReceiveNum, 5, BLACK);
	       }
		   if( xQueueReceive( TFT_ledR_Queue, &ReceiveNum, 10) == pdPASS)
	       {
			   printf("Customer receive number:%d\r\n",ReceiveNum);
			   LCD_ShowNum_int(18, 16, (uint16_t)ReceiveNum, 5, RED);
	       }
		   if( xQueueReceive( TFT_ledG_Queue, &ReceiveNum, 10) == pdPASS)
	       {
			   printf("Customer receive number:%d\r\n",ReceiveNum);
			   LCD_ShowNum_int(18, 32, (uint16_t)ReceiveNum, 5, GREEN);
	       }
		   if( xQueueReceive( TFT_ledB_Queue, &ReceiveNum, 10) == pdPASS)
	       {
			   printf("Customer receive number:%d\r\n",ReceiveNum);
			   LCD_ShowNum_int(18, 48, (uint16_t)ReceiveNum, 5, BLUE);
	       }

	}

}

/*******************************************************************************
* @brief       LED_R_task function
* @param[in]   None
* @param[out]  None
* @RetVal      None
*******************************************************************************/
void LED_R_task(void *pvParameters)
{
	int16_t Time = 0;
    for( ;; )
    {

		 if( xQueueReceive( Time_R_Queue, &Time, portMAX_DELAY ) == pdPASS)
	     {
			 xSemaphoreTake(RGBled_BinSem_Handle, portMAX_DELAY);
			 if(0 == Time%3)
			 {
				 nano_board_led_on(LED_R);
	             xQueueSend( TFT_ledR_Queue, ( void* )&Time, 0 );
	             vTaskDelay( 5 * mainSOFTWARE_TIMER_PERIOD_MS );
				 nano_board_led_off(LED_R);
			 }
		 	 xSemaphoreGive(RGBled_BinSem_Handle);
	     }
    }

}

/*******************************************************************************
* @brief       LED_G_task function
* @param[in]   None
* @param[out]  None
* @RetVal      None
*******************************************************************************/
void LED_G_task(void *pvParameters)
{
	int16_t Time = 0;
    for( ;; )
    {
		 if( xQueueReceive( Time_G_Queue, &Time, portMAX_DELAY ) == pdPASS)
	     {
			 xSemaphoreTake(RGBled_BinSem_Handle, portMAX_DELAY);
			 if(0 == Time%5)
			 {
				 nano_board_led_on(LED_G);
				 xQueueSend( TFT_ledG_Queue, ( void* )&Time, 0 );
		         vTaskDelay( 7 * mainSOFTWARE_TIMER_PERIOD_MS );
				 nano_board_led_off(LED_G);
			 }
	   		 xSemaphoreGive(RGBled_BinSem_Handle);
	     }
    }

}

/*******************************************************************************
* @brief       LED_B_task function
* @param[in]   None
* @param[out]  None
* @RetVal      None
*******************************************************************************/
void LED_B_task(void *pvParameters)
{
	int16_t Time = 0;
    for( ;; )
    {
		 if( xQueueReceive( Time_B_Queue, &Time, portMAX_DELAY ) == pdPASS)
	     {
			 xSemaphoreTake(RGBled_BinSem_Handle, portMAX_DELAY);
			 if( 0 == Time%7 )
			 {
				 nano_board_led_on(LED_B);
				 xQueueSend( TFT_ledB_Queue, ( void* )&Time, 0 );
		         vTaskDelay( 3 * mainSOFTWARE_TIMER_PERIOD_MS );
				 nano_board_led_off(LED_B);
			 }
			 xSemaphoreGive(RGBled_BinSem_Handle);
	     }
    }

}

