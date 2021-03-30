/* USER CODE BEGIN Header */
/**
  ****************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ****************************************************************************
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
  ****************************************************************************
  */
/* USER CODE END Header */

/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "eth.h"
#include "i2c.h"
#include "spi.h"
#include "tim.h"
#include "usart.h"
#include "usb_otg.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "bmp280.h"
#include "bmp280_defs.h"
#include "stdio.h"
#include "stdlib.h"
#include "math.h"
#include "arm_math.h"
#include "math_helper.h"
#include "i2c-lcd.h"
#include "funkcje_pom.h"

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */


/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/** PID controller parameters definition */

#define PID_PARAM_KP       600     /*!<  Proportional */ 
#define PID_PARAM_KI       2.9     /*!<  Integral */
#define PID_PARAM_KD       80      /*!<  Derivative */


/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */

/** Define UART message variables */

	  
	   uint32_t pres1=0, temp1=0;
	   int timer =0;
	   char msg_in[4];
	   float msg=0;
       float msg_last = 0;



/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);






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

	/** Set PID parameters */
	 
	   PID.Kp = PID_PARAM_KP;        /*!< Proportional */
	   PID.Ki = PID_PARAM_KI;        /*!< Integral */
	   PID.Kd = PID_PARAM_KD;        /*!< Derivative */

    /** Set PID2 parameters */

	   PID2.Kp = PID_PARAM_KP;        /*!< Proportional */
	   PID2.Ki = PID_PARAM_KI;        /*!< Integral */
	   PID2.Kd = PID_PARAM_KD;        /*!< Derivative */

	/** Initialize PID systems, float32_t format */

	 	  arm_pid_init_f32(&PID, 1);
	 	  arm_pid_init_f32(&PID2, 1);


  /* USER CODE END 1 */
  

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  
  /** Initialize SysTick Timer */
  HAL_Init();

  /* USER CODE BEGIN Init */


 // print_rslt(" bmp280_set_power_mode status", rslt);///////////////////////////

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_ETH_Init();
  MX_USART3_UART_Init();
  MX_USB_OTG_FS_PCD_Init();
  MX_SPI4_Init();
  MX_I2C1_Init();
  MX_TIM2_Init();
  MX_TIM3_Init();
  MX_TIM4_Init();
  /* USER CODE BEGIN 2 */

  /** Initialize LCD */
  lcd_init();

 
   bmp.delay_ms = HAL_Delay;
   bmp.dev_id = 0;
   bmp.intf = BMP280_SPI_INTF;
   bmp.read = spi_reg_read;
   bmp.write = spi_reg_write;


   rslt = bmp280_init(&bmp);

   /* Always read the current settings before writing, especially when
   * all the configuration is not modified
   */
   rslt = bmp280_get_config(&conf, &bmp);

   /* Configuring the temperature oversampling, filter coefficient and output data rate */
   /* Overwrite the desired settings */
   conf.filter = BMP280_FILTER_COEFF_2;

   /* Temperature oversampling set at 4x */
   conf.os_temp = BMP280_OS_4X;

   /* Pressure over sampling none (disabling pressure measurement) */
   conf.os_pres = BMP280_OS_NONE;

   /* Setting the output data rate as 1HZ(1000ms) */
   conf.odr = BMP280_ODR_1000_MS;
   rslt = bmp280_set_config(&conf, &bmp);

   /* Always set the power mode after setting the configuration */
   rslt = bmp280_set_power_mode(BMP280_NORMAL_MODE, &bmp);

   /** Start UART data receiving */
   HAL_UART_Receive_IT(&huart3, msg_in,4);

   /** Initialize PWMs */
   HAL_TIM_PWM_Start(&htim3, TIM_CHANNEL_1);

   HAL_TIM_PWM_Start(&htim4, TIM_CHANNEL_1);

   //HAL_TIM_Base_Start_IT(&htim2); ///////////////////////////////////////////////////////////////////

   /** Start UART data transmit */
   size = sprintf(buffer, "Podaj temperature \r\n");
   HAL_UART_Transmit_IT(&huart3, buffer, size);

  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
                /** Read temperature from sensor */
	  	  	  	  temp=read();

                /** Send temperature values to calculate PWM duties */
	              pid1(msg,temp);
	              pid2(msg,temp);
               
	             // send_uart_float(temp);
	              LCD(msg, temp);

	         /** Sleep time between measurements = BMP280_ODR_1000_MS */
	         bmp.delay_ms(300);



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
  RCC_PeriphCLKInitTypeDef PeriphClkInitStruct = {0};

  /** Configure LSE Drive Capability 
  */
  HAL_PWR_EnableBkUpAccess();
  /** Configure the main internal regulator output voltage 
  */
  __HAL_RCC_PWR_CLK_ENABLE();
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE3);
  /** Initializes the CPU, AHB and APB busses clocks 
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_BYPASS;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLM = 4;
  RCC_OscInitStruct.PLL.PLLN = 72;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = 3;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }
  /** Initializes the CPU, AHB and APB busses clocks 
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK)
  {
    Error_Handler();
  }
  PeriphClkInitStruct.PeriphClockSelection = RCC_PERIPHCLK_USART3|RCC_PERIPHCLK_I2C1
                              |RCC_PERIPHCLK_CLK48;
  PeriphClkInitStruct.Usart3ClockSelection = RCC_USART3CLKSOURCE_PCLK1;
  PeriphClkInitStruct.I2c1ClockSelection = RCC_I2C1CLKSOURCE_PCLK1;
  PeriphClkInitStruct.Clk48ClockSelection = RCC_CLK48SOURCE_PLL;
  if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInitStruct) != HAL_OK)
  {
    Error_Handler();
  }
}

/* USER CODE BEGIN 4 */

/** 
* @brief Interrupt checks if UART information is correct
*
* @param huart: is UART3 handler
*/

void HAL_UART_RxCpltCallback(UART_HandleTypeDef* huart)
{
    /** Message convertion */
    msg = atof(msg_in); 
    msg = msg + 0.01;
    msg = floor(10 * msg) / 10;

    /** Checks if set value is within the working range of controller */

    if (msg > 50) 
    {
        size = sprintf(buffer, "Out of range\r\n");
        HAL_UART_Transmit_IT(&huart3, buffer, size);
        msg = msg_last;
    }
    else if (msg < 21)
    {
        size = sprintf(buffer, "Out of range\r\n");
        HAL_UART_Transmit_IT(&huart3, buffer, size);
        msg = msg_last;
    }
    else
    {
        size = sprintf(buffer, "Podano nowa temperature!: %.1f \r\n", msg);
        HAL_UART_Transmit_IT(&huart3, buffer, size);
        msg_last = msg;
    }


    HAL_UART_Receive_IT(&huart3, msg_in, 4);
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
