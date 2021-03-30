#include "funkcje_pom.h"



int8_t spi_reg_read(uint8_t cs, uint8_t reg_addr , uint8_t *reg_data , uint16_t length)
{
  HAL_StatusTypeDef status = HAL_OK;
  int32_t iError = BMP280_OK;
  uint8_t txarray[SPI_BUFFER_LEN] = {0,};
  uint8_t rxarray[SPI_BUFFER_LEN] = {0,};
  uint8_t stringpos;
 txarray[0] = reg_addr;
 HAL_GPIO_WritePin( SPI4_CS_GPIO_Port , SPI4_CS_Pin , GPIO_PIN_RESET );
 status = HAL_SPI_TransmitReceive( &hspi4 , (uint8_t*)(&txarray), (uint8_t*)(&rxarray), length+1, 5);
 while( hspi4.State == HAL_SPI_STATE_BUSY ) {};
 HAL_GPIO_WritePin( SPI4_CS_GPIO_Port , SPI4_CS_Pin , GPIO_PIN_SET );
 for (stringpos = 0; stringpos < length; stringpos++)
 {
	 *(reg_data + stringpos) = rxarray[stringpos + BMP280_DATA_INDEX];
 }
 if (status != HAL_OK)
 {
	 iError = (-1);
 }
 return (int8_t)iError;
}


int8_t spi_reg_write(uint8_t cs, uint8_t reg_addr , uint8_t *reg_data , uint16_t length)
{
HAL_StatusTypeDef status = HAL_OK;
int32_t iError = BMP280_OK;
uint8_t txarray[SPI_BUFFER_LEN * BMP280_ADDRESS_INDEX];
uint8_t stringpos;
txarray[0] = reg_addr;
for (stringpos = 0; stringpos < length; stringpos++)
{
	txarray[stringpos+BMP280_DATA_INDEX] = reg_data[stringpos];
}
 HAL_GPIO_WritePin( SPI4_CS_GPIO_Port , SPI4_CS_Pin , GPIO_PIN_RESET );
 status = HAL_SPI_Transmit( &hspi4 , (uint8_t*)(&txarray), length*2, 100);
 while( hspi4.State == HAL_SPI_STATE_BUSY ) {};
 HAL_GPIO_WritePin( SPI4_CS_GPIO_Port , SPI4_CS_Pin , GPIO_PIN_SET );
if (status != HAL_OK)
{  iError = (-1);
return (int8_t)iError;
}

}

/**
  * @brief LCD Handler that clears display and creates data to show
  * @param  in: set temperature
  * @param  out: actual temperature of resistor
  * @retval None
  */

void LCD(float in, double out)  
{          in = floor(10*in)/10;					/**< Sets floats precision */
		   out = floor(100*out)/100;				/**< Sets floats precision */
		  char msg_lcd[16];					/**< Creates char buffer, size determined by LCD size */
		  lcd_clear();						/**< Clears display */
		  lcd_put_cur(0, 0);					/**< Sets message starting point on LCD - column 1, row 1 */
		  if(in > 50)						/**< Checks if set value is within the operating range */
		  {
			  lcd_send_string("Out of range!");
		  }
		  else if(in < 21)
		  {
			  lcd_send_string("Out of range!");
		  }
		  else							/**< If Set value is correct LCD will display data */
		  {
		  sprintf(&msg_lcd, "SET: %.1f ", in);		
		  lcd_send_string(msg_lcd);					
		  lcd_put_cur(1, 0);						
		  sprintf(&msg_lcd, "TEMP: %.2f ", out);	
		  lcd_send_string(msg_lcd);					
		  }
}

/**
  * @brief reads temperature data from bmp280 sensor
  * @retval temp: readed data from sensor
*/
float read()
{
				 /** Reading the raw data from sensor */
		         rslt = bmp280_get_uncomp_data(&ucomp_data, &bmp);

		         /** Getting the 32 bit compensated temperature */
		         rslt = bmp280_get_comp_temp_32bit(&temp32, ucomp_data.uncomp_temp, &bmp);

		         /** Getting the compensated temperature as floating point value */
		         rslt = bmp280_get_comp_temp_double(&temp, ucomp_data.uncomp_temp, &bmp);

	return temp;
}


/**
* @brief Function computes PID error and sets PWM duty
*
* @param msg: Set value
* @param temp: Current value
*/

void pid1(float msg, double temp)
{
				    /** Calculate error */			
		                    pid_error = msg-temp; 


		                    /** Calculate PID here, argument is error */
		                    /** Output data will be returned, we will use it as duty cycle parameter */
		                    duty = arm_pid_f32(&PID, pid_error);

		                    /** Check overflow, duty cycle in percent */
		                   	  if (duty > 1000) {
		                   	     duty = 1000;
		                   	  } else if (duty < 0) {
		                   	    duty = 0;
		                   	  }
							
		                   	 __HAL_TIM_SET_COMPARE(&htim3, TIM_CHANNEL_1, duty);
}
/**
* @brief Function computes PID2 error and sets PWM duty
*
* @param msg: Set value
* @param temp: Current value
*/

void pid2(float msg, double temp)
{
				    /** Calculate error */
		                    pid_error2 = temp-msg;

		                    /** Calculate PID here, argument is error */
		                    /** Output data will be returned, we will use it as duty cycle parameter */
		                    duty2 = arm_pid_f32(&PID2, pid_error2);

		                    if (duty2 > 1000) {
		                   	  duty2 = 1000;
		                   	} else if (duty2 < 120) {
		                   	   duty2 = 0;
		                   	}

		                    __HAL_TIM_SET_COMPARE(&htim4, TIM_CHANNEL_1, duty2);
}

/**
* @brief Function sends message by UART
*/
void send_uart_float(float temp12)
{

				 size =sprintf(buffer, "%f \n\r", temp12);
				 HAL_UART_Transmit(&huart3, (uint8_t*)buffer, size, 200);

}

*/

