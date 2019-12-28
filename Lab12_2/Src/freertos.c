/* USER CODE BEGIN Header */
/**
 ******************************************************************************
 * File Name          : freertos.c
 * Description        : Code for freertos applications
 ******************************************************************************
 * @attention
 *
 * <h2><center>&copy; Copyright (c) 2019 STMicroelectronics.
 * All rights reserved.</center></h2>
 *
 * This software component is licensed by ST under Ultimate Liberty license
 * SLA0044, the "License"; You may not use this file except in compliance with
 * the License. You may obtain a copy of the License at:
 *                             www.st.com/SLA0044
 *
 ******************************************************************************
 */
/* USER CODE END Header */

/* Includes ------------------------------------------------------------------*/
#include "FreeRTOS.h"
#include "task.h"
#include "main.h"
#include "cmsis_os.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
/* USER CODE BEGIN Variables */

/* USER CODE END Variables */
osThreadId MsgProducerHandle;
osThreadId MsgConsumerHandle;
osThreadId LEDsHandle;
osMessageQId myQueue01Handle;
osMailQId mail01Handle;
typedef struct {
	uint16_t var;
} mailStruct;
/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN FunctionPrototypes */

/* USER CODE END FunctionPrototypes */

void MsgProducerTask(void const * argument);
void MsgConsumerTask(void const * argument);
void LEDsTask(void const * argument);

void MX_FREERTOS_Init(void); /* (MISRA C 2004 rule 8.1) */

/**
 * @brief  FreeRTOS initialization
 * @param  None
 * @retval None
 */
void MX_FREERTOS_Init(void) {
	/* USER CODE BEGIN Init */

	/* USER CODE END Init */

	/* USER CODE BEGIN RTOS_MUTEX */
	/* add mutexes, ... */
	/* USER CODE END RTOS_MUTEX */

	/* USER CODE BEGIN RTOS_SEMAPHORES */
	/* add semaphores, ... */
	/* USER CODE END RTOS_SEMAPHORES */

	/* USER CODE BEGIN RTOS_TIMERS */
	/* start timers, add new ones, ... */
	/* USER CODE END RTOS_TIMERS */

	/* Create the queue(s) */
	/* definition and creation of myQueue01 */
	osMessageQDef(myQueue01, 16, uint16_t);
	myQueue01Handle = osMessageCreate(osMessageQ(myQueue01), NULL);

	osMailQDef(mail01, 4, mailStruct);
	mail01Handle = osMailCreate(osMailQ(mail01), NULL);
	/* USER CODE BEGIN RTOS_QUEUES */
	/* add queues, ... */
	/* USER CODE END RTOS_QUEUES */

	/* Create the thread(s) */
	/* definition and creation of MsgProducer */
	osThreadDef(MsgProducer, MsgProducerTask, osPriorityNormal, 0, 128);
	MsgProducerHandle = osThreadCreate(osThread(MsgProducer), NULL);

	/* definition and creation of MsgConsumer */
	osThreadDef(MsgConsumer, MsgConsumerTask, osPriorityNormal, 0, 128);
	MsgConsumerHandle = osThreadCreate(osThread(MsgConsumer), NULL);

	osThreadDef(LEDs, LEDsTask, osPriorityNormal, 0, 128);
	LEDsHandle = osThreadCreate(osThread(LEDs), NULL);
	/* USER CODE BEGIN RTOS_THREADS */

	/* add threads, ... */
	/* USER CODE END RTOS_THREADS */

}

/* USER CODE BEGIN Header_MsgProducerTask */
/**
 * @brief  Function implementing the MsgProducer thread.
 * @param  argument: Not used
 * @retval None
 */
/* USER CODE END Header_MsgProducerTask */

unsigned char msg[40] = { 0 };
UART_HandleTypeDef huart1;
osEvent event;
mailStruct * mail;
mailStruct * pMail;
#define pdFALSE			( ( BaseType_t ) 0 )
void MsgProducerTask(void const * argument) {

	/* USER CODE BEGIN MsgProducerTask */
	/* Infinite loop */
	mail = (mailStruct *) osMailAlloc(mail01Handle, osWaitForever);
	mail->var = 3;
	osMailPut(mail01Handle, mail);
	mail = (mailStruct *) osMailAlloc(mail01Handle, osWaitForever);
	mail->var = 2;
	osMailPut(mail01Handle, mail);
	mail = (mailStruct *) osMailAlloc(mail01Handle, osWaitForever);
	mail->var = 1;
	osMailPut(mail01Handle, mail);
	mail = (mailStruct *) osMailAlloc(mail01Handle, osWaitForever);
	mail->var = 0;
	osMailPut(mail01Handle, mail);
	sprintf(msg, "Mail Init Begin\r\n");
	HAL_UART_Transmit(&huart1, (uint8_t*) msg, strlen(msg), HAL_MAX_DELAY);
	for (;;) {
		int count = xQueueIsQueueEmptyFromISR(mail01Handle);
		if (xQueueIsQueueEmptyFromISR(mail01Handle) == pdFALSE) {
			mail = (mailStruct *) osMailAlloc(mail01Handle, osWaitForever);
			mail->var = 3;
			osMailPut(mail01Handle, mail);
			mail = (mailStruct *) osMailAlloc(mail01Handle, osWaitForever);
			mail->var = 2;
			osMailPut(mail01Handle, mail);
			mail = (mailStruct *) osMailAlloc(mail01Handle, osWaitForever);
			mail->var = 1;
			osMailPut(mail01Handle, mail);
			mail = (mailStruct *) osMailAlloc(mail01Handle, osWaitForever);
			mail->var = 0;
			osMailPut(mail01Handle, mail);
		}
		osDelay(500);
	}
	/* USER CODE END MsgProducerTask */
}

/* USER CODE BEGIN Header_MsgConsumerTask */
/**
 * @brief Function implementing the MsgConsumer thread.
 * @param argument: Not used
 * @retval None
 */
/* USER CODE END Header_MsgConsumerTask */
void MsgConsumerTask(void const * argument) {
	/* USER CODE BEGIN MsgConsumerTask */
	/* Infinite loop */
	/* Infinite loop */
	osDelay(1000);
	for (;;) {
		event = osMailGet(mail01Handle, osWaitForever);
		if (event.status == osEventMail) {
			pMail = event.value.p;
			sprintf(msg, "Mail value: %d\r\n", pMail->var);
			HAL_UART_Transmit(&huart1, (uint8_t*) msg, strlen(msg),
			HAL_MAX_DELAY);
			osMailFree(mail01Handle, pMail);
		}
	}
	/* USER CODE END MsgConsumerTask */
}

/* Private application code --------------------------------------------------*/
/* USER CODE BEGIN Application */

/* USER CODE END Application */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
void LEDsTask(void const * argument) {
	/* USER CODE BEGIN LEDsTask */
	/* Infinite loop */
	for (;;) {

		HAL_GPIO_TogglePin(LED0_GPIO_Port, LED0_Pin);
		HAL_Delay(100);
		HAL_GPIO_TogglePin(LED1_GPIO_Port, LED1_Pin);
	}
	/* USER CODE END LEDsTask */
}
