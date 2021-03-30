#include "main.h"
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


#define SPI_BUFFER_LEN 28
#define BMP280_DATA_INDEX 1
#define BMP280_ADDRESS_INDEX 2

       int8_t rslt;
	   struct bmp280_dev bmp;
	   struct bmp280_config conf;
	   struct bmp280_uncomp_data ucomp_data;
	   int32_t temp32;
	   double temp;
	   char buffer[40];
	   uint8_t size;

	   /* PID error */
	  	float pid_error;
	  	float pid_error2;
	   /* Duty cycle for PWM */
	  	float duty;
	  	float duty2;
	   /* ARM PID Instance, float_32 format */
	  	arm_pid_instance_f32 PID;
	  	arm_pid_instance_f32 PID2;


int8_t spi_reg_read(uint8_t cs, uint8_t reg_addr , uint8_t *reg_data , uint16_t length);
int8_t spi_reg_write(uint8_t cs, uint8_t reg_addr , uint8_t *reg_data , uint16_t length);
void LCD(float in, double out);
float read();
void pid1(float msg, double temp);
void pid2(float msg, double temp);
void send_uart_float(float temp12);
