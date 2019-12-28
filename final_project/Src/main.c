/* USER CODE BEGIN Header */
/**
 ******************************************************************************
 * @file           : main.c
 * @brief          : Main program body
 ******************************************************************************
 * @attention
 *
 * <h2><center>&copy; Copyright (c) 2019 STMicroelectronics.
 * All rights reserved.</center></h2>
 *
 * This software component is licensed by ST under BSD 3-Clause license,
 * the "License"; You may not use this file except in compliance with the
 * License. You may obtain a copy of the License at:
 *                        opensource.org/licenses/BSD-3-Clause
 *
 ******************************************************************************
 */
/* USER CODE END Header */

/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "rtc.h"
#include "usart.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "lcd.h"
#include "time.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
#define x0  120
#define y0  100
#define r  72
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */
uint8_t state = 0;
uint8_t nes[10][4];
uint8_t alarm[10][4];
int set_time = 0;
int clock_mode = 1; // 1 for circle and 0 for digit
extern TIM_HandleTypeDef htim3;
int s_year, s_month, s_day, s_hour, s_min, s_scd = 0; // saving time info for adjusting
uint8_t rxBuffer[20];
char time_types[7][15] = { "year", "month", "day", "hour", "minute", "second",
		"time" };
char set_hint[100] =
		"set [[time yyyymmddhhmmss] [year yyyy] [month mm] [day dd] [hour hh] [minute mm] [second ss]]\r\n";
int Length_Of_leds = 7;
unsigned char led0_on[9] = { 0x6C, 0x65, 0x64, 0x30, 0x20, 0x6F, 0x6E, 0x0D,
		0x0A };
unsigned char led1_on[9] = { 0x6C, 0x65, 0x64, 0x31, 0x20, 0x6F, 0x6E, 0x0D,
		0x0A };
unsigned char led0_off[10] = { 0x6C, 0x65, 0x64, 0x30, 0x20, 0x6F, 0x66, 0x66,
		0x0D, 0x0A };
unsigned char led1_off[10] = { 0x6C, 0x65, 0x64, 0x31, 0x20, 0x6F, 0x66, 0x66,
		0x0D, 0x0A };
unsigned char settime[9] = { 0x73, 0x65, 0x74, 0x20, 0x74, 0x69, 0x6D, 0x65,
		0x20 };
unsigned char setYear[9] = { 0x73, 0x65, 0x74, 0x20, 0x79, 0x65, 0x61, 0x72,
		0x20 };
unsigned char setMonth[10] = { 0x73, 0x65, 0x74, 0x20, 0x6D, 0x6F, 0x6E, 0x74,
		0x68, 0x20 };
unsigned char setDay[8] = { 0x73, 0x65, 0x74, 0x20, 0x64, 0x61, 0x79, 0x20 };
unsigned char setHour[10] = { 0x73, 0x65, 0x74, 0x20, 0x6D, 0x6F, 0x6E, 0x74,
		0x68, 0x20 };
unsigned char setMinute[11] = { 0x73, 0x65, 0x74, 0x20, 0x6D, 0x69, 0x6E, 0x75,
		0x74, 0x65, 0x20 };
unsigned char setSecond[11] = { 0x73, 0x65, 0x74, 0x20, 0x73, 0x65, 0x63, 0x6F,
		0x6E, 0x64, 0x20 };
//unsigned char alarm_ls[10] = { 0x61, 0x6C, 0x61, 0x72, 0x6D, 0x73, 0x20, 0x6C,
//		0x73, 0x20 };
//unsigned char alarm_set[11] = { 0x61, 0x6C, 0x61, 0x72, 0x6D, 0x73, 0x20, 0x73,
//		0x65, 0x74, 0x20 };
//unsigned char alarm_change[14] = { 0x61, 0x6C, 0x61, 0x72, 0x6D, 0x73, 0x20,
//		0x63, 0x68, 0x61, 0x6E, 0x67, 0x65, 0x20 };
//unsigned char alarm_delete[14] = { 0x61, 0x6C, 0x61, 0x72, 0x6D, 0x73, 0x20,
//		0x64, 0x65, 0x6C, 0x65, 0x74, 0x65, 0x20 };
//unsigned char alarms_change_success[22] = { 0x61, 0x6C, 0x61, 0x72, 0x6D, 0x73,
//		0x20, 0x73, 0x65, 0x74, 0x20, 0x73, 0x75, 0x63, 0x63, 0x65, 0x73, 0x73,
//		0x0D, 0x0A, 0x0D, 0x0A };
//unsigned char alarm_add[10] = { 0x61, 0x6C, 0x61, 0x72, 0x6D, 0x73, 0x20, 0x61,
//		0x64, 0x64 };

//unsigned char countdown_ls[12] = { 0x63, 0x6F, 0x75, 0x6E, 0x74, 0x64, 0x6F,
//		0x77, 0x6E, 0x20, 0x6C, 0x73 }; //countdown ls\r\n
//unsigned char countdown_rm[12] = { 0x63, 0x6F, 0x75, 0x6E, 0x74, 0x64, 0x6F,
//		0x77, 0x6E, 0x20, 0x72, 0x6D }; //countdown ls\r\n72 6D
//unsigned char countdown_rm_finish[23] = { 0x63, 0x6F, 0x75, 0x6E, 0x74, 0x64,
//		0x6F, 0x77, 0x6E, 0x20, 0x72, 0x6D, 0x20, 0x73, 0x75, 0x63, 0x63, 0x65,
//		0x73, 0x73, 0x21, 0x0D, 0x0A };
//unsigned char countdown_set[13] = { 0x63, 0x6F, 0x75, 0x6E, 0x74, 0x64, 0x6F,
//		0x77, 0x6E, 0x20, 0x73, 0x65, 0x74 };
//unsigned char countdown_set_success[24] = { 0x63, 0x6F, 0x75, 0x6E, 0x74, 0x64,
//		0x6F, 0x77, 0x6E, 0x20, 0x73, 0x65, 0x74, 0x20, 0x73, 0x75, 0x63, 0x63,
//		0x65, 0x73, 0x73, 0x21, 0x0D, 0x0A };
//unsigned char countdown_add[13] = { 0x63, 0x6F, 0x75, 0x6E, 0x74, 0x64, 0x6F,
//		0x77, 0x6E, 0x20, 0x64, 0x64, 0x64 };

unsigned char countdown_ls[] = "countdown ls"; //countdown ls\r\n
unsigned char countdown_rm[] = "countdown rm"; //
unsigned char countdown_rm_finish[] = "countdown rm finish\r\n";
unsigned char countdown_set[] = "countdown set";
unsigned char countdown_set_success[] = "countdown set success\r\n";
unsigned char countdown_add[] = "countdown add";
unsigned char change_state[] = "change state";
unsigned char alarm_ls[] = "alarm ls";
unsigned char alarm_change[] = "alarm change";
unsigned char alarm_delete[] = "alarm delete";
unsigned char alarms_change_success[] = "alarms change success\r\n";
unsigned char alarm_add[] = "alarm add";
static uint8_t count_alarms_0 = 0;
static unsigned char uRx_Data[1024] = { 0 };
static unsigned char uLength = 0;

_Bool judgement_string(unsigned char read[], unsigned char want[], int length);
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */
HAL_StatusTypeDef HAL_UART_Receive_IT(UART_HandleTypeDef *huart, uint8_t *pData,
		uint16_t Size);
/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin) {
	HAL_Delay(100);
	switch (GPIO_Pin) {
	case KEY_0_Pin:
//		HAL_UART_Transmit(&huart1, "KEY_0 pressed.\r\n", 15, 0xffff);
		if (set_time != 0){
			adjust_time(1);}
		else {
			state = !state;
		}
		HAL_Delay(100);
		break;
	case KEY_1_Pin:
//		HAL_UART_Transmit(&huart1, "KEY_1 pressed.\r\n", 15, 0xffff);
		if (set_time != 0)
			adjust_time(0);
		HAL_Delay(100);
		break;
	case WK_UP_Pin:
		if (HAL_GPIO_ReadPin(WK_UP_GPIO_Port, WK_UP_Pin) == GPIO_PIN_SET) {
//			HAL_UART_Transmit(&huart1, "WK_UP pressed.\r\n", 16, 0xffff);
			Draw_set_time_block();
		}

		HAL_Delay(100);
		break;
	default:
		break;
	}
}

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart) {
	if (set_time != 0) {
		HAL_UART_Transmit(&huart1, "Please set time by key\r\n", 24, 0xffff);
		return;
	}

	if (huart->Instance == USART1) {
		static unsigned char uRx_Data[1024] = { 0 };
		static unsigned char uLength = 0;
		char set_info[4] = "set ";
		char set_time_type[10];
		char set_time_info[20];
		if (rxBuffer[0] == '\n') {
			// Check whether the received info is interrupt
			int is_same = 1;
			for (int i = 0; i < 4; i++) {
				if (uRx_Data[i] != set_info[i]) {
					is_same = 0;
				}
			}

			// If it is interrupt then:
			int failed = 0;
			if (is_same == 1) {
//				HAL_UART_Transmit(&huart1, "set check\r\n", 11, 0xffff);
				int count = 4;
				while (uRx_Data[count] != ' ' && count - 4 < 8) {
					set_time_type[count - 4] = uRx_Data[count];
					count++;
				}

				int time_type = -1;
				for (int i = 0; i < 7; i++) {
					is_same = 1;
					for (int j = 0; j < count - 4; j++) {
						if (set_time_type[j] != time_types[i][j]) {
							is_same = 0;
						}
					}

					if (is_same == 1 && count - 4 > 0) {
						time_type = i;
						break;
					}
				}

				if (time_type == -1) {
//					HAL_UART_Transmit(&huart1, "type check fail\r\n", 16,
//							0xffff);
					failed = 1;
				} else {
//					HAL_UART_Transmit(&huart1, "type check succ\r\n", 16,
//							0xffff);
					count++;
					int start_num = count;
					while (uRx_Data[count] != '\r' && count < 25) {
						if (uRx_Data[count] - '0' < 0
								|| uRx_Data[count] - '0' > 9) {
							failed = 1;
							break;
						}

						set_time_info[count - start_num] = uRx_Data[count];
						count++;
					}

					if (time_type == 0) {
						if (count - start_num != 4) {
							failed = 1;
						}
					} else {
						if (count - start_num != 2 && time_type != 6) {
							failed = 1;
						}
					}
					int value = atoi(set_time_info);
					if (failed == 0) {
//						HAL_UART_Transmit(&huart1, "time check succ\r\n", 16,
//								0xffff);
						if (time_type == 6) {
							if (count - start_num != 14) {
//								HAL_UART_Transmit(&huart1,
//										"six1 check fail\r\n", 16, 0xffff);
								failed = 1;
							} else {
//								HAL_UART_Transmit(&huart1,
//										"six1 check succ\r\n", 16, 0xffff);
								int year_s, month_s, day_s, hour_s, min_s,
										scd_s = 0;

								year_s = (set_time_info[0] - '0') * 1000
										+ (set_time_info[1] - '0') * 100
										+ (set_time_info[2] - '0') * 10
										+ (set_time_info[3] - '0');
								month_s = (set_time_info[4] - '0') * 10
										+ (set_time_info[5] - '0');
								day_s = (set_time_info[6] - '0') * 10
										+ (set_time_info[7] - '0');
								hour_s = (set_time_info[8] - '0') * 10
										+ (set_time_info[9] - '0');
								min_s = (set_time_info[10] - '0') * 10
										+ (set_time_info[11] - '0');
								scd_s = (set_time_info[12] - '0') * 10
										+ (set_time_info[13] - '0');

								int date_num[6] = { year_s, month_s, day_s,
										hour_s, min_s, scd_s };
								for (int i = 0; i < 6; i++) {
									if (check_legal_date(i, date_num[i]) == 0) {
										failed = 1;
//										HAL_UART_Transmit(&huart1,
//												"date check fail\r\n", 16,
//												0xffff);
										break;
									}
								}

								if (failed == 0) {
									char test[20];
									sprintf(test, "%d %d %d %d %d %d",
											date_num[0], date_num[1],
											date_num[2], date_num[3],
											date_num[4], date_num[5]);
//									HAL_UART_Transmit(&huart1, test, 15,
//											0xffff);
//									HAL_UART_Transmit(&huart1,
//											"date check succ\r\n", 16, 0xffff);
									Set_Time(date_num[3], date_num[4],
											date_num[5]);
									Set_Date(date_num[0] - 2000, date_num[1],
											date_num[2]);
								}
							}
						} else {
							if (check_legal_date(time_type, value) == 1) {
//								HAL_UART_Transmit(&huart1,
//										"date check succ\r\n", 16, 0xffff);
								int date_num[6] = { s_year, s_month, s_day,
										s_hour, s_min, s_scd };

								if (time_type == 0) {
									value = value - 2000;
								}

								date_num[time_type] = value;
								if (time_type > 2) {
									Set_Time(date_num[3], date_num[4],
											date_num[5]);
								} else {
									Set_Date(date_num[0], date_num[1],
											date_num[2]);
								}
							} else {
//								HAL_UART_Transmit(&huart1,
//										"date check fail\r\n", 16, 0xffff);
								failed = 1;
							}
						}
					} else {
//						HAL_UART_Transmit(&huart1, "time check fail\r\n", 16,
//								0xffff);
						failed = 1;
					}
				}

			} else {
				failed = 1;
			}

			if (judgement_string(uRx_Data, change_state,
					sizeof(change_state) - 1)) {
				state = !state;
				HAL_UART_Transmit(&huart1, uRx_Data, uLength, 0xffff);
			} else if (judgement_string(uRx_Data, countdown_ls,
					sizeof(countdown_ls) - 1)) {
				unsigned char willreturn[400] = { 0 };
				int temp = 0;
				for (; temp < 10; temp++) {
					unsigned char addition[40] = { 0 };
					unsigned char judge[8];
					if (nes[temp % 10][0] == 1) {
						sprintf(judge, "%s", "Used");
					} else if (nes[temp % 10][0] == 0) {
						sprintf(judge, "%s", "Unused");
					} else if (nes[temp % 10][0] == 2) {
						sprintf(judge, "%s", "lock");
					}
					sprintf(addition, "countdown %d: %d:%d:%d %s \r\n", temp,
							nes[temp % 10][1], nes[temp % 10][2],
							nes[temp % 10][3], judge);
					strcat(willreturn, addition);
				}
				HAL_UART_Transmit(&huart1, willreturn, sizeof(willreturn),
						0xffff);
			} else if (judgement_string(uRx_Data, countdown_rm,
					sizeof(countdown_rm) - 1)) {
				int countdown_number = uRx_Data[13] - 0x30;
				nes[countdown_number][0] = 2;
				nes[countdown_number][1] = 0;
				nes[countdown_number][2] = 0;
				nes[countdown_number][3] = 0;
				HAL_UART_Transmit(&huart1, countdown_rm_finish,
						sizeof(countdown_rm_finish), 0xffff);
			} else if (judgement_string(uRx_Data, countdown_set,
					sizeof(countdown_set) - 1)) {
				int order = uRx_Data[14] - 0x30;
				nes[order][1] = (uRx_Data[16] - 0x30) * 10
						+ (uRx_Data[17] - 0x30) % 24;
				nes[order][2] = (uRx_Data[19] - 0x30) * 10
						+ (uRx_Data[20] - 0x30) % 60;
				nes[order][3] = (uRx_Data[22] - 0x30) * 10
						+ (uRx_Data[23] - 0x30) % 60;
				nes[order][0] = 0;
				HAL_UART_Transmit(&huart1, countdown_set_success,
						sizeof(countdown_set_success), 0xffff);
			} else if (judgement_string(uRx_Data, countdown_add,
					sizeof(countdown_add) - 1)) {
				int countdown_number = 0;
				nes[countdown_number][1] = (uRx_Data[14] - 0x30) * 10
						+ (uRx_Data[15] - 0x30) % 24;
				nes[countdown_number][2] = (uRx_Data[17] - 0x30) * 10
						+ (uRx_Data[18] - 0x30) % 60;
				nes[countdown_number][3] = (uRx_Data[20] - 0x30) * 10
						+ (uRx_Data[21] - 0x30) % 60;
				nes[countdown_number][0] = 0;
				HAL_UART_Transmit(&huart1, countdown_set_success,
						sizeof(countdown_set_success), 0xffff);
			} else if (judgement_string(uRx_Data, alarm_ls,
					sizeof(alarm_ls) - 1)) {
				unsigned char willreturn[400] = { 0 };
				int temp = 0;
				for (; temp < 10; temp++) {
					unsigned char addition[40] = { 0 };
					unsigned char judge[8];
					if (alarm[temp % 10][0] == 1) {
						sprintf(judge, "%s", "Used");
					} else if (alarm[temp % 10][0] == 0) {
						sprintf(judge, "%s", "Unused");
					} else if (alarm[temp % 10][0] == 2) {
						sprintf(judge, "%s", "lock");
					}
					sprintf(addition, "Alarms %d: %d:%d:%d %s \r\n", temp,
							alarm[temp % 10][1], alarm[temp % 10][2],
							alarm[temp % 10][3], judge);
					strcat(willreturn, addition);
				}
				HAL_UART_Transmit(&huart1, willreturn, sizeof(willreturn),
						0xffff);
			} else if (judgement_string(uRx_Data, alarm_delete,
					sizeof(alarm_delete) - 1)) {
				int alarm_number = uRx_Data[13] - 0x30;
				alarm[alarm_number][0] = 2;
				alarm[alarm_number][1] = 0;
				alarm[alarm_number][2] = 0;
				alarm[alarm_number][3] = 0;
				HAL_UART_Transmit(&huart1, alarm_delete, sizeof(alarm_delete),
						0xffff);
			} else if (judgement_string(uRx_Data, alarm_change,
					sizeof(alarm_change) - 1)) {
				int order = uRx_Data[13] - 0x30;
				alarm[order][1] = (uRx_Data[15] - 0x30) * 10
						+ (uRx_Data[16] - 0x30) % 24;
				alarm[order][2] = (uRx_Data[18] - 0x30) * 10
						+ (uRx_Data[19] - 0x30) % 60;
				alarm[order][3] = (uRx_Data[21] - 0x30) * 10
						+ (uRx_Data[22] - 0x30) % 60;
				alarm[order][0] = 0;
				HAL_UART_Transmit(&huart1, alarms_change_success,
						sizeof(alarms_change_success), 0xffff);
			} else if (judgement_string(uRx_Data, alarm_add,
					sizeof(alarm_add) - 1)) {
				int alarm_number = count_alarms_0 % 10;
				alarm[alarm_number][0] = 0;
				alarm[alarm_number][1] = (uRx_Data[10] - 0x30) * 10
						+ (uRx_Data[11] - 0x30) % 24;
				alarm[alarm_number][2] = (uRx_Data[13] - 0x30) * 10
						+ (uRx_Data[14] - 0x30) % 24;
				alarm[alarm_number][3] = (uRx_Data[16] - 0x30) * 10
						+ (uRx_Data[17] - 0x30) % 24;
				HAL_UART_Transmit(&huart1, alarms_change_success,
						sizeof(alarms_change_success), 0xffff);
			} else if (failed == 1) {
				HAL_UART_Transmit(&huart1,
						"Please follow the format or check the range:\r\n", 46,
						0xffff);
				HAL_UART_Transmit(&huart1, set_hint, 95, 0xffff);
			}

			uLength = 0;
		} else {
			uRx_Data[uLength] = rxBuffer[0];
			uLength++;
		}
	}

}

_Bool judgement_string(unsigned char read[], unsigned char want[], int length) {
	int willreturn = 1;
	int count = 0;
	for (; count < length; count++) {
		if (read[count] != want[count]) {
			willreturn = 0;
		}
	}
	return willreturn;
}

/* USER CODE END 0 */

/**
 * @brief  The application entry point.
 * @retval int
 */
int main(void) {
	/* USER CODE BEGIN 1 */
	for (; state < 10; state++) {
		nes[state % 10][state / 10] = 2;
		alarm[state % 10][state / 10] = 2;
	}
	for (; state < 40; state++) {
		nes[state % 10][state / 10] = 1;
		alarm[state % 10][state / 10] = 0;
	}
	state = 0;
	/* USER CODE END 1 */

	/* MCU Configuration--------------------------------------------------------*/

	/* Reset of all peripherals, Initializes the Flash interface and the Systick. */
	HAL_Init();

	/* USER CODE BEGIN Init */

	/* USER CODE END Init */

	/* Configure the system clock */
	SystemClock_Config();

	/* USER CODE BEGIN SysInit */

	/* USER CODE END SysInit */

	/* Initialize all configured peripherals */
	MX_GPIO_Init();
	MX_ADC1_Init();
	MX_TIM3_Init();
	MX_RTC_Init();
	MX_USART1_UART_Init();
	HAL_TIM_Base_Start_IT(&htim3);
	/* USER CODE BEGIN 2 */
	HAL_UART_Receive_IT(&huart1, (uint8_t *) rxBuffer, 1);
	/* USER CODE END 2 */

	/* Infinite loop */
	/* USER CODE BEGIN WHILE */

//两种界面共有部分显示：两种界面共有的日期，星期和按钮
//	Draw_static();
//界面�????????显示：显示当前时间的时钟表盘
//	Draw_disc_clock();
//功能：设置当前时间为18:20:30
	Set_Time(18, 20, 30);

//设置当前日期�????????2020.1.5
	Set_Date(20, 1, 5);

//界面二显示：显示当前时间的数字时�????????
	Draw_static();

	while (1) {
		/* USER CODE END WHILE */
		if (state) {
			Draw_digital_clock();
		} else {
			Draw_disc_clock();
		}
		/* USER CODE BEGIN 3 */
	}
	/* USER CODE END 3 */
}

/**
 * @brief System Clock Configuration
 * @retval None
 */
void SystemClock_Config(void) {
	RCC_OscInitTypeDef RCC_OscInitStruct = { 0 };
	RCC_ClkInitTypeDef RCC_ClkInitStruct = { 0 };
	RCC_PeriphCLKInitTypeDef PeriphClkInit = { 0 };

	/** Initializes the CPU, AHB and APB busses clocks
	 */
	RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI
			| RCC_OSCILLATORTYPE_LSI;
	RCC_OscInitStruct.HSIState = RCC_HSI_ON;
	RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
	RCC_OscInitStruct.LSIState = RCC_LSI_ON;
	RCC_OscInitStruct.PLL.PLLState = RCC_PLL_NONE;
	if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK) {
		Error_Handler();
	}
	/** Initializes the CPU, AHB and APB busses clocks
	 */
	RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_SYSCLK
			| RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2;
	RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_HSI;
	RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
	RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
	RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

	if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_0) != HAL_OK) {
		Error_Handler();
	}
	PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_RTC;
	PeriphClkInit.RTCClockSelection = RCC_RTCCLKSOURCE_LSI;
	if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit) != HAL_OK) {
		Error_Handler();
	}
}

/* USER CODE BEGIN 4 */

//共有部分的绘�????????
void Draw_static() {
	LCD_Init();
	LCD_Clear(BLACK);
	//日期和星�????????
	RTC_DateTypeDef sDate = { 0 };

	char weekdays[7][15] = { "Sunday", "Monday", "Tuesday", "Wednesday",
			"Thursday", "Friday", "Saturday" };
	char year[20], month[3], day[3], weekday[15];
	char hyphon[2] = { '-' };
	if (HAL_RTC_GetDate(&hrtc, &sDate, RTC_FORMAT_BIN) == HAL_OK) {
		sprintf(year, "%d", sDate.Year + 2000);
		sprintf(month, "%02d", sDate.Month);
		sprintf(day, "%02d", sDate.Date);
	}
	strcat(year, hyphon);
	strcat(year, month);
	strcat(year, hyphon);
	strcat(year, day);
	strcpy(weekday, weekdays[sDate.WeekDay]);

	POINT_COLOR = CYAN;
	LCD_ShowString(30, 200, 200, 24, 16, (uint8_t*) year);
	POINT_COLOR = RED;
	LCD_ShowString(30, 220, 200, 24, 16, (uint8_t*) weekday);
	POINT_COLOR = BLACK;

	//底部按钮
	LCD_Fill(20, 250, 110, 300, GRAYBLUE);
	LCD_Fill(130, 250, 220, 300, GRAYBLUE);
	POINT_COLOR = BLACK;
	BACK_COLOR = GRAYBLUE;
	LCD_ShowString(30, 270, 200, 24, 16, (uint8_t*) "Set Time");
	LCD_ShowString(135, 270, 200, 24, 16, (uint8_t*) "Count Down");
	BACK_COLOR = BLACK;
	POINT_COLOR = WHITE;
}

void Draw_set_time_block() {
	if (set_time < 6) {
		LCD_Fill(20, 250, 110, 300, RED);
		POINT_COLOR = BLACK;
		BACK_COLOR = RED;
		LCD_ShowString(30, 270, 200, 24, 16, (uint8_t*) time_types[set_time]);
		BACK_COLOR = BLACK;
		POINT_COLOR = WHITE;
		set_time++;
	} else {
		LCD_Fill(20, 250, 110, 300, GRAYBLUE);
		POINT_COLOR = BLACK;
		BACK_COLOR = GRAYBLUE;
		LCD_ShowString(30, 270, 200, 24, 16, (uint8_t*) "Set Time");
		BACK_COLOR = BLACK;
		POINT_COLOR = WHITE;
		set_time = 0;
	}
}

void adjust_time(int increase) {
	int month_days[12] = { 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 };
	// �?查二月天�?
	if ((((s_year + 2000) % 4 == 0) && ((s_year + 2000) % 100 != 0))
			|| (s_year + 2000) % 400 == 0) {
		month_days[1]++;
	}

	int level = set_time;
	int levels_thr[5] = { 12, month_days[s_month - 1], 23, 59, 59 };
	int levels_thr_low[5] = { 1, 1, 0, 0, 0 };
	int date_num[5] = { s_month, s_day, s_hour, s_min, s_scd };
	while (level > 0) {
		char year_s[20];
		char month_s[20];
		char day_s[20];
		sprintf(year_s, "%02d\r\n", s_year);
		sprintf(month_s, "%02d\r\n", s_month);
		sprintf(day_s, "%02d\r\n", s_day);
//		HAL_UART_Transmit(&huart1, year_s, 4, 0xffff);
//		HAL_UART_Transmit(&huart1, month_s, 4, 0xffff);
//		HAL_UART_Transmit(&huart1, day_s, 4, 0xffff);
		if (level == 1) {
			if (increase == 1) {
				s_year++;
			} else {
				s_year--;
			}
		} else {
			if (increase == 1) {
				if (date_num[level - 2] >= levels_thr[level - 2]) {
					date_num[level - 2] = levels_thr_low[level - 2];
				} else {
					date_num[level - 2]++;
					break;
				}
			} else {
				if (date_num[level - 2] <= levels_thr_low[level - 2]) {
					date_num[level - 2] = levels_thr[level - 2];
				} else {
					date_num[level - 2]--;
					break;
				}
			}
		}

		level--;
	}

	s_month = date_num[0];
	s_day = date_num[1];
	s_hour = date_num[2];
	s_min = date_num[3];
	s_scd = date_num[4];

	char hr_s[20], min_s[3], scd_s[3];
	sprintf(hr_s, "%02d", date_num[2]);
	sprintf(min_s, "%02d", date_num[3]);
	sprintf(scd_s, "%02d", date_num[4]);

	char hyphon[2] = { '-' };
	strcat(hr_s, hyphon);
	strcat(hr_s, min_s);
	strcat(hr_s, hyphon);
	strcat(hr_s, scd_s);

	char timem[100];
	sprintf(timem, "%s\r\n", hr_s);
//	HAL_UART_Transmit(&huart1, timem, 10, 0xffff);
	if (set_time != 0) {
		Set_Date(s_year, date_num[0], date_num[1]);
		Set_Time(date_num[2], date_num[3], date_num[4]);
	}
}

int check_legal_date(int time_type, int value) {
	int month_days[12] = { 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 };
	// �?查二月天�?
	if ((((s_year + 2000) % 4 == 0) && ((s_year + 2000) % 100 != 0))
			|| (s_year + 2000) % 400 == 0) {
		month_days[1]++;
	}

	switch (time_type) {
	case 1:
		if (value > 12 || value < 1) {
			return 0;
		}
		break;
	case 2:
		if (value > month_days[s_month - 1] || value < 1) {
			return 0;
		}
		break;
	case 3:
		if (value > 23 || value < 0) {
			return 0;
		}
		break;
	case 4:
		if (value > 59 || value < 0) {
			return 0;
		}
		break;
	case 5:
		if (value > 59 || value < 0) {
			return 0;
		}
		break;
	case 0:
		break;
	}

	return 1;
}

//圆盘时钟的绘�????????(显示当前时间)
void Draw_disc_clock() {
	uint8_t state0 = 0;
	//背景+�????????+圆心
	LCD_Fast_DrawPoint(x0, y0, WHITE);
	LCD_Draw_Circle(x0, y0, 1);
	LCD_Draw_Circle(x0, y0, 4);
	LCD_Draw_Circle(x0, y0, r);
	LCD_Draw_Circle(x0, y0, r - 1);
	LCD_Draw_Circle(x0, y0, r - 2);
	//60个齿�????????
	float pi = 4.0 * atan(1);
	float angle = 0;
	uint8_t x1;
	uint8_t y1;
	uint8_t x2;
	uint8_t y2;
	uint8_t l;
	for (int i = 0; i < 60; i++) {
		if (i % 15 == 0) {
			l = 12;
		} else if (i % 5 == 0) {
			l = 8;
		} else {
			l = 6;
		}

		x1 = x0 + floor(r * cos(angle));
		y1 = y0 - floor(r * sin(angle));
		x2 = x0 + floor((r - l) * cos(angle));
		y2 = y0 - floor((r - l) * sin(angle));
		LCD_DrawLine(x1, y1, x2, y2);
		//角度增加6度，pi/30表示弧度制的6�??????????
		angle = angle + pi / 30;
	}
	//获取当前时间
	RTC_TimeTypeDef sTime = { 0 };
	RTC_DateTypeDef sDate = { 0 };
	HAL_RTC_GetTime(&hrtc, &sTime, RTC_FORMAT_BIN);
	uint8_t h, m, s, hx, hy, mx, my, sx, sy;
	double h_angle, m_angle;
	hx, hy, mx, my, sx, sy = 0;

	int is_changed = 0;
	while (1) {
		led_twinkle();
		if (state != state0) {
			POINT_COLOR = BLACK;
			LCD_Fast_DrawPoint(x0, y0, BLACK);
			LCD_Draw_Circle(x0, y0, 1);
			LCD_Draw_Circle(x0, y0, 4);
			LCD_Draw_Circle(x0, y0, r);
			LCD_Draw_Circle(x0, y0, r - 1);
			LCD_Draw_Circle(x0, y0, r - 2);
			float pi = 4.0 * atan(1);
			float angle = 0;
			uint8_t x1;
			uint8_t y1;
			uint8_t x2;
			uint8_t y2;
			uint8_t l;
			for (int i = 0; i < 60; i++) {
				if (i % 15 == 0) {
					l = 12;
				} else if (i % 5 == 0) {
					l = 8;
				} else {
					l = 6;
				}
				x1 = x0 + floor(r * cos(angle));
				y1 = y0 - floor(r * sin(angle));
				x2 = x0 + floor((r - l) * cos(angle));
				y2 = y0 - floor((r - l) * sin(angle));
				LCD_DrawLine(x1, y1, x2, y2);
				//角度增加6度，pi/30表示弧度制的6�??
				angle = angle + pi / 30;
			}
			LCD_DrawLine(x0, y0, sx, sy);
			LCD_DrawLine(x0, y0, mx, my);
			LCD_DrawLine(x0, y0, hx, hy);
			return;
		}

		if (set_time == 0) {
			if (HAL_RTC_GetTime(&hrtc, &sTime, RTC_FORMAT_BIN) == HAL_OK
					&& s != sTime.Seconds) {
				s = sTime.Seconds;
				m = sTime.Minutes;
				h = sTime.Hours;

				if (s_hour != h || s_min != m || s_scd != s) {
					is_changed = 1;
				}

				s_hour = h;
				s_min = m;
				s_scd = s;
			}

			if (HAL_RTC_GetDate(&hrtc, &sDate, RTC_FORMAT_BIN) == HAL_OK) {
				s_year = sDate.Year;
				s_month = sDate.Month;
				s_day = sDate.Date;
			}
		} else {
			if (s_hour != h || s_min != m || s_scd != s) {
				is_changed = 1;
				h = s_hour;
				m = s_min;
				s = s_scd;
			}
		}

		if (is_changed == 1) {
			POINT_COLOR = BLACK;
			LCD_DrawLine(x0, y0, sx, sy);
			LCD_DrawLine(x0, y0, mx, my);
			LCD_DrawLine(x0, y0, hx, hy);

			h_angle = s_hour * 2 * pi / 12 + (double) s_min * pi / 360; // h/12*360*(pi/180) + m/60*30*(pi/180)
			m_angle = s_min * 2 * pi / 60 + (double) s_scd * pi / 1800; // m/60*360*(pi/180) + s/60*6*(pi/180)
			hx = x0 + floor(20 * sin(h_angle));
			hy = y0 - floor(20 * cos(h_angle));
			mx = x0 + floor(30 * sin(m_angle));
			my = y0 - floor(30 * cos(m_angle));
			sx = x0 + floor(50 * sin(s * 2 * pi / 60));
			sy = y0 - floor(50 * cos(s * 2 * pi / 60));

			POINT_COLOR = BRRED;
			LCD_DrawLine(x0, y0, sx, sy);
			POINT_COLOR = BROWN;
			LCD_DrawLine(x0, y0, mx, my);
			POINT_COLOR = GRED;
			LCD_DrawLine(x0, y0, hx, hy);
			is_changed = 0;
		}
	}
}

//数字时钟的绘�??????????(显示当前时间)
void Draw_digital_clock() {
	uint8_t state1 = 1;
	POINT_COLOR = RED;
	LCD_Fast_DrawPoint(80, 105, RED);
	LCD_Draw_Circle(80, 105, 1);
	LCD_Fast_DrawPoint(80, 115, RED);
	LCD_Draw_Circle(80, 115, 1);
	LCD_Fast_DrawPoint(150, 105, RED);
	LCD_Draw_Circle(150, 115, 1);
	LCD_Fast_DrawPoint(150, 105, RED);
	LCD_Draw_Circle(150, 115, 1);

	RTC_TimeTypeDef sTime = { 0 };
	uint8_t hour, min, sec = 0;
	hour = sTime.Hours;
	min = sTime.Minutes;
	sec = sTime.Seconds;

	RTC_DateTypeDef sDate = { 0 };
	int blink = 0;
	while (1) {
		led_twinkle();
		if (state != state1) {
			POINT_COLOR = BLACK;
			LCD_Fast_DrawPoint(80, 105, BLACK);
			LCD_Draw_Circle(80, 105, 1);
			LCD_Fast_DrawPoint(80, 115, BLACK);
			LCD_Draw_Circle(80, 115, 1);
			LCD_Fast_DrawPoint(150, 105, BLACK);
			LCD_Draw_Circle(150, 115, 1);
			LCD_Fast_DrawPoint(150, 105, BLACK);
			LCD_Draw_Circle(150, 115, 1);
			Draw_num(90, 80, min / 10, 20, BLACK);
			Draw_num(120, 80, min % 10, 20, BLACK);
			Draw_num(20, 80, hour / 10, 20, BLACK);
			Draw_num(50, 80, hour % 10, 20, BLACK);
			Draw_num(20, 80, hour / 10, 20, BLACK);
			Draw_num(50, 80, hour % 10, 20, BLACK);
			Draw_num(90, 80, min / 10, 20, BLACK);
			Draw_num(120, 80, min % 10, 20, BLACK);
			Draw_num(160, 80, sec / 10, 20, BLACK);
			Draw_num(190, 80, sec % 10, 20, BLACK);
			return;
		}

		if (set_time == 0) {
			if (HAL_RTC_GetTime(&hrtc, &sTime, RTC_FORMAT_BIN) == HAL_OK
					&& sec != sTime.Seconds) {
				sec = sTime.Seconds;
				if (hour != sTime.Hours) {
					hour = sTime.Hours;
					Draw_num(20, 80, hour / 10, 20, RED);
					Draw_num(50, 80, hour % 10, 20, RED);
				}
				if (min != sTime.Minutes) {
					min = sTime.Minutes;
					Draw_num(90, 80, min / 10, 20, RED);
					Draw_num(120, 80, min % 10, 20, RED);
				}
				Draw_num(160, 80, sec / 10, 20, RED);
				Draw_num(190, 80, sec % 10, 20, RED);
			}

			if (HAL_RTC_GetDate(&hrtc, &sDate, RTC_FORMAT_BIN) == HAL_OK) {
				s_year = sDate.Year;
				s_month = sDate.Month;
				s_day = sDate.Date;
			}

			s_hour = hour;
			s_min = min;
			s_scd = sec;
		} else {
			if (blink == 0) {
				POINT_COLOR = BLACK;
				LCD_Fast_DrawPoint(80, 105, BLACK);
				LCD_Draw_Circle(80, 105, 1);
				LCD_Fast_DrawPoint(80, 115, BLACK);
				LCD_Draw_Circle(80, 115, 1);
				LCD_Fast_DrawPoint(150, 105, BLACK);
				LCD_Draw_Circle(150, 115, 1);
				LCD_Fast_DrawPoint(150, 105, BLACK);
				LCD_Draw_Circle(150, 115, 1);
				blink = 1;
				HAL_Delay(200);
			} else {
				POINT_COLOR = RED;
				LCD_Fast_DrawPoint(80, 105, RED);
				LCD_Draw_Circle(80, 105, 1);
				LCD_Fast_DrawPoint(80, 115, RED);
				LCD_Draw_Circle(80, 115, 1);
				LCD_Fast_DrawPoint(150, 105, RED);
				LCD_Draw_Circle(150, 115, 1);
				LCD_Fast_DrawPoint(150, 105, RED);
				LCD_Draw_Circle(150, 115, 1);

				Draw_num(20, 80, s_hour / 10, 20, RED);
				Draw_num(50, 80, s_hour % 10, 20, RED);
				Draw_num(90, 80, s_min / 10, 20, RED);
				Draw_num(120, 80, s_min % 10, 20, RED);
				Draw_num(160, 80, s_scd / 10, 20, RED);
				Draw_num(190, 80, s_scd % 10, 20, RED);
				blink = 0;
				HAL_Delay(200);
			}

		}
	}
}

uint8_t nes_count[10] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
uint8_t alarm_count[10] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
uint8_t A_1[2] = "AB";
uint8_t A_2[2] = "CD";
uint8_t A_3[2] = "EF";

void led_twinkle() {
	int temp = 0;
	int blink = 0;
	for (; temp < 10; temp++) {
		if (nes_count[temp] > 50) {
			nes_count[temp] = 0;
			nes[temp][0] = 2;
			blink = 0;
		}
		if (nes[temp][0] == 2) {
			//HAL_UART_Transmit(&huart1, A_1, sizeof(A_1), 0xffff);
			continue;
		}
		if (!nes[temp][1] && !nes[temp][2] && !nes[temp][3]
				&& nes[temp][0] == 0) {
			//HAL_UART_Transmit(&huart1, A_2, sizeof(A_2), 0xffff);
			nes[temp][0] = 1;
		}
		if (nes[temp][0] == 1) {

			//HAL_UART_Transmit(&huart1, A_3, sizeof(A_3), 0xffff);
			nes_count[temp] += 1;
			blink = 1;
			HAL_GPIO_WritePin(GPIOA, GPIO_PIN_8, GPIO_PIN_SET);
			HAL_GPIO_WritePin(GPIOD, GPIO_PIN_2, GPIO_PIN_RESET);
		}
	}
	temp = 0;
	RTC_TimeTypeDef sTime = { 0 };
	HAL_RTC_GetTime(&hrtc, &sTime, RTC_FORMAT_BIN);
	for (; temp < 10; temp++) {

		if (alarm_count[temp] > 50) {
			alarm_count[temp] = 0;
			alarm[temp][0] = 2;
			blink = 0;
		}
		if (alarm[temp % 10][0] == 2) {
			continue;
		}
		if (alarm[temp][0] == 0 && sTime.Hours == alarm[temp][1]
				&& sTime.Minutes == alarm[temp][2]
				&& sTime.Seconds == alarm[temp][3]) {
			//HAL_UART_Transmit(&huart1, A_2, sizeof(A_2), 0xffff);
			alarm[temp][0] = 1;
		}
		if (alarm[temp % 10][0] == 1) {
			alarm_count[temp] += 1;
			blink = 2;
			HAL_GPIO_WritePin(GPIOA, GPIO_PIN_8, GPIO_PIN_RESET);
			HAL_GPIO_WritePin(GPIOD, GPIO_PIN_2, GPIO_PIN_RESET);
		}
	}
	HAL_Delay(25);
	if (blink != 0) {
		HAL_GPIO_TogglePin(GPIOD, GPIO_PIN_2);
		HAL_GPIO_TogglePin(GPIOA, GPIO_PIN_8);
	}
	HAL_Delay(25);
}

//给定坐标和数字�?��?�绘制指定数字的八数码格式，长度为数字大�??????????
void Draw_num(int x, int y, int num, int len, int color) {
	LCD_Fill(x, y, x + len - 1, y + 2 * len - 2, BLACK);
	POINT_COLOR = color;
	if (num == 2 || num == 3 || num == 5 || num == 6 || num == 7 || num == 8
			|| num == 9 || num == 0) {
		LCD_DrawLine(x + 2, y, x + len - 3, y);
	}
	if (num == 2 || num == 3 || num == 4 || num == 5 || num == 6 || num == 8
			|| num == 9) {
		LCD_DrawLine(x + 2, y + len - 1, x + len - 3, y + len - 1);
	}
	if (num == 2 || num == 3 || num == 5 || num == 6 || num == 8 || num == 9
			|| num == 0) {
		LCD_DrawLine(x + 2, y + 2 * len - 2, x + len - 3, y + 2 * len - 2);
	}
	if (num == 4 || num == 5 || num == 6 || num == 8 || num == 9 || num == 0) {
		LCD_DrawLine(x, y + 2, x, y + len - 3);
	}
	if (num == 2 || num == 6 || num == 8 || num == 0) {
		LCD_DrawLine(x, y + len + 1, x, y + 2 * len - 4);
	}
	if (num == 1 || num == 2 || num == 3 || num == 4 || num == 7 || num == 8
			|| num == 9 || num == 0) {
		LCD_DrawLine(x + len - 1, y + 2, x + len - 1, y + len - 3);
	}
	if (num == 1 || num == 3 || num == 4 || num == 5 || num == 6 || num == 7
			|| num == 8 || num == 9 || num == 0) {
		LCD_DrawLine(x + len - 1, y + len + 1, x + len - 1, y + 2 * len - 4);
	}
	POINT_COLOR = WHITE;
}

void Set_Time(int h, int m, int s) {
	RTC_TimeTypeDef sTime = { 0 };
	sTime.Hours = h;
	sTime.Minutes = m;
	sTime.Seconds = s;
	HAL_RTC_SetTime(&hrtc, &sTime, RTC_FORMAT_BIN);
}

void Set_Date(int year, int month, int day) {
	RTC_DateTypeDef sDate = { 0 };
	char weekday[15];
	char weekdays[7][15] = { "Sunday   ", "Monday    ", "Tuesday  ", "Wednesday",
			"Thursday ", "Friday   ", "Saturday " };
	sDate.Year = year;
	sDate.Month = month;
	sDate.Date = day;
	HAL_RTC_SetDate(&hrtc, &sDate, RTC_FORMAT_BIN);

	year = 2000 + year;
	char year_s[20], month_s[3], day_s[3];
	char hyphon[2] = { '-' };
	sprintf(year_s, "%04d", year);
	sprintf(month_s, "%02d", month);
	sprintf(day_s, "%02d", day);
	strcpy(weekday, weekdays[sDate.WeekDay]);

	strcat(year_s, hyphon);
	strcat(year_s, month_s);
	strcat(year_s, hyphon);
	strcat(year_s, day_s);


//	HAL_UART_Transmit(&huart1, year_s, 10, 0xffff);
//	HAL_UART_Transmit(&huart1, "\r\n", 2, 0xffff);
	POINT_COLOR = CYAN;
	LCD_ShowString(30, 200, 200, 24, 16, (uint8_t*) year_s);
	POINT_COLOR = RED;
	LCD_ShowString(30, 220, 200, 24, 16, (uint8_t*) weekday);
	POINT_COLOR = BLACK;
}
/* USER CODE END 4 */

/**
 * @brief  This function is executed in case of error occurrence.
 * @retval None
 */
void Error_Handler(void) {
	/* USER CODE BEGIN Error_Handler_Debug */
	/* User can add his own implementation to report the HAL error return state */

	/* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{ 
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     tex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
