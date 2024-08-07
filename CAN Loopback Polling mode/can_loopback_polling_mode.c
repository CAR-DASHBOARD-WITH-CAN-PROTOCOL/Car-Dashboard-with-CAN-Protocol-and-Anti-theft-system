
/*please do activate sys, RCC> crystal oscillator, and can1 according to sir given ppt*/


/* USER CODE BEGIN Header */
/**
 ******************************************************************************
 * @file           : main.c
 * @brief          : Main program body
 ******************************************************************************
 * @attention
 *
 * Copyright (c) 2024 STMicroelectronics.
 * All rights reserved.
 *
 * This software is licensed under terms that can be found in the LICENSE file
 * in the root directory of this software component.
 * If no LICENSE file comes with this software, it is provided AS-IS.
 *
 ******************************************************************************
 */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"
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
CAN_HandleTypeDef hcan1;
/* USER CODE BEGIN PV */
uint8_t ubKeyNumber = 0x0; //Counter Variable
CAN_TxHeaderTypeDef   TxHeader; // CAN Data Frame header fields like RTR Bit, DLC, ID
CAN_RxHeaderTypeDef   RxHeader; // CAN Data Frame header fields like RTR Bit, DLC, ID
uint8_t               TxData[8]; //Actual Payload
uint8_t               RxData[8]; //Actual Payload
uint32_t              TxMailbox; //Buffer for Tx Messages
/* USER CODE END PV */
/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_CAN1_Init(void);
/* USER CODE BEGIN PFP */
void CAN_filterConfig(void); //Acceptance filter Configuration
void LED_Display(uint8_t LedStatus);
/* USER CODE END PFP */
/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
/* USER CODE END 0 */
/**
 * @brief  The application entry point.
 * @retval int
 */
int main(void)
{
 /* USER CODE BEGIN 1 */
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
 MX_CAN1_Init();
 /* USER CODE BEGIN 2 */
 CAN_Config();
 if (HAL_CAN_Start(&hcan1) != HAL_OK)
	  {
		/* Start Error */
		Error_Handler();
	  }
 CAN_TransmitData();
 if(CAN_ReceiveData() == 1)
	{
	  /* OK: Turn on LED1 */
	  	  HAL_GPIO_WritePin(GPIOD, GPIO_PIN_12, SET);//green led
	}
	else
	{
	  /* KO: Turn on LED2 */
		  HAL_GPIO_WritePin(GPIOD, GPIO_PIN_13, SET);//orange led
	}
 /* USER CODE END 2 */
 /* Infinite loop */
 /* USER CODE BEGIN WHILE */
 while (1)
 {
   /* USER CODE END WHILE */
   /* USER CODE BEGIN 3 */
 }
 /* USER CODE END 3 */
}
/**
 * @brief System Clock Configuration
 * @retval None
 */
void SystemClock_Config(void)
{
 RCC_OscInitTypeDef RCC_OscInitStruct = {0};
 RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};
 /** Configure the main internal regulator output voltage
 */
 __HAL_RCC_PWR_CLK_ENABLE();
 __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);
 /** Initializes the RCC Oscillators according to the specified parameters
 * in the RCC_OscInitTypeDef structure.
 */
 RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
 RCC_OscInitStruct.HSIState = RCC_HSI_ON;
 RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
 RCC_OscInitStruct.PLL.PLLState = RCC_PLL_NONE;
 if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
 {
   Error_Handler();
 }
 /** Initializes the CPU, AHB and APB buses clocks
 */
 RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                             |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
 RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_HSI;
 RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
 RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
 RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;
 if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_0) != HAL_OK)
 {
   Error_Handler();
 }
}
/**
 * @brief CAN1 Initialization Function
 * @param None
 * @retval None
 */
static void MX_CAN1_Init(void)
{
 /* USER CODE BEGIN CAN1_Init 0 */
 /* USER CODE END CAN1_Init 0 */
 /* USER CODE BEGIN CAN1_Init 1 */
 /* USER CODE END CAN1_Init 1 */
 hcan1.Instance = CAN1;
 hcan1.Init.Prescaler = 16;
 hcan1.Init.Mode = CAN_MODE_LOOPBACK;
 hcan1.Init.SyncJumpWidth = CAN_SJW_1TQ;
 hcan1.Init.TimeSeg1 = CAN_BS1_9TQ;
 hcan1.Init.TimeSeg2 = CAN_BS2_4TQ;
 hcan1.Init.TimeTriggeredMode = DISABLE;
 hcan1.Init.AutoBusOff = DISABLE;
 hcan1.Init.AutoWakeUp = DISABLE;
 hcan1.Init.AutoRetransmission = DISABLE;
 hcan1.Init.ReceiveFifoLocked = DISABLE;
 hcan1.Init.TransmitFifoPriority = DISABLE;
 if (HAL_CAN_Init(&hcan1) != HAL_OK)
 {
   Error_Handler();
 }
 /* USER CODE BEGIN CAN1_Init 2 */
 /* USER CODE END CAN1_Init 2 */
}
/**
 * @brief GPIO Initialization Function
 * @param None
 * @retval None
 */
static void MX_GPIO_Init(void)
{
 GPIO_InitTypeDef GPIO_InitStruct = {0};
/* USER CODE BEGIN MX_GPIO_Init_1 */
/* USER CODE END MX_GPIO_Init_1 */
 /* GPIO Ports Clock Enable */
 __HAL_RCC_GPIOH_CLK_ENABLE();
 __HAL_RCC_GPIOD_CLK_ENABLE();
 __HAL_RCC_GPIOA_CLK_ENABLE();
 /*Configure GPIO pin Output Level */
 HAL_GPIO_WritePin(GPIOD, GPIO_PIN_12|GPIO_PIN_13|GPIO_PIN_14|GPIO_PIN_15, GPIO_PIN_RESET);
 /*Configure GPIO pins : PD12 PD13 PD14 PD15 */
 GPIO_InitStruct.Pin = GPIO_PIN_12|GPIO_PIN_13|GPIO_PIN_14|GPIO_PIN_15;
 GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
 GPIO_InitStruct.Pull = GPIO_NOPULL;
 GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
 HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);
/* USER CODE BEGIN MX_GPIO_Init_2 */
/* USER CODE END MX_GPIO_Init_2 */
}
/* USER CODE BEGIN 4 */
void CAN_Config(void)
{
 CAN_FilterTypeDef sFilterConfig;
 sFilterConfig.FilterBank = 0;
 sFilterConfig.FilterMode = CAN_FILTERMODE_IDMASK;
 sFilterConfig.FilterScale = CAN_FILTERSCALE_32BIT;
 sFilterConfig.FilterIdHigh = 0x0000;
 sFilterConfig.FilterIdLow = 0x0000;
 sFilterConfig.FilterMaskIdHigh = 0x0000;
 sFilterConfig.FilterMaskIdLow = 0x0000;
 sFilterConfig.FilterFIFOAssignment = CAN_FILTER_FIFO0;
 sFilterConfig.FilterActivation = ENABLE;
 sFilterConfig.SlaveStartFilterBank = 14;
 if (HAL_CAN_ConfigFilter(&hcan1, &sFilterConfig) != HAL_OK)
 {
   Error_Handler();
 }
}
void CAN_TransmitData(void)
{
	/*##- Start the Transmission process #####################################*/
	  TxHeader.StdId = 0x11;
	  TxHeader.RTR = CAN_RTR_DATA;
	  TxHeader.IDE = CAN_ID_STD;
	  TxHeader.DLC = 2;
	  TxHeader.TransmitGlobalTime = DISABLE;
	  TxData[0] = 0xCA;
	  TxData[1] = 0xFE;
	  /* Request transmission */
	  //API HAL_CAN_AddTxMessage to request a transmission of new Message
	  if(HAL_CAN_AddTxMessage(&hcan1, &TxHeader, TxData, &TxMailbox) != HAL_OK)
	  {
	    /* Transmission request Error */
	    Error_Handler();
	  }
	  /* Wait for transmission to complete */
	  //HAL_CAN_GetTxMailboxesFreeLevel To get no of free Tx Mailboxes, wait until at least 3 Mailbox is free
	  while(HAL_CAN_GetTxMailboxesFreeLevel(&hcan1) != 3) {}
}
int CAN_ReceiveData(void)
{
	/*##- Start the Reception process ########################################*/
	//Monitor the Reception of the Message using HAL_CAN_GetRxFifoFillLevel until at least one Msg is Received
	  if(HAL_CAN_GetRxFifoFillLevel(&hcan1, CAN_RX_FIFO0) != 1)
	  {
	    /* Reception Missing */
	    Error_Handler();
	  }
	  //Get the Msg using HAL_CAN_GetRxMessage
	  if(HAL_CAN_GetRxMessage(&hcan1, CAN_RX_FIFO0, &RxHeader, RxData) != HAL_OK)
	  {
	    /* Reception Error */
	    Error_Handler();
	  }
	  if((RxHeader.StdId != 0x11)                     ||
	     (RxHeader.RTR != CAN_RTR_DATA)               ||
	     (RxHeader.IDE != CAN_ID_STD)                 ||
	     (RxHeader.DLC != 2)                          ||
	     ((RxData[0]<<8 | RxData[1]) != 0xCAFE))
		  {
			/* Rx message Error */
			return 0;
		  }
	  return 1;
}
/* USER CODE END 4 */
/**
 * @brief  This function is executed in case of error occurrence.
 * @retval None
 */
void Error_Handler(void)
{
 /* USER CODE BEGIN Error_Handler_Debug */
 /* User can add his own implementation to report the HAL error return state */
	HAL_GPIO_WritePin(GPIOD, GPIO_PIN_14, SET);
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
    ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
 /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */

/*Note: if we use this code in normal mode then we can only transmit data other node would be receiving data, other node will be have receive function to receive the data with same configuration. 

If we use loop back mode then no need of other mode, we will be transmitting and we will be receiving with same code*/
