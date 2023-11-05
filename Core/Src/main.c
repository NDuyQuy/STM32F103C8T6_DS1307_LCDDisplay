/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2023 STMicroelectronics.
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
#include "myLCD.h"
#include <stdio.h>
#define DS1307_I2C_ADDR     	0x68
#define DS1307_REG_SECOND     0x00
#define DS1307_REG_MINUTE     0x01
#define DS1307_REG_HOUR      	0x02
#define DS1307_REG_DOW        0x03
#define DS1307_REG_DATE       0x04
#define DS1307_REG_MONTH      0x05
#define DS1307_REG_YEAR       0x06
#define DS1307_REG_CONTROL    0x07
#define DS1307_REG_UTC_HR    	0x08
#define DS1307_REG_UTC_MIN    0x09
#define DS1307_REG_CENT       0x10
#define DS1307_TIMEOUT        1000
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
I2C_HandleTypeDef hi2c1;

/* USER CODE BEGIN PV */
uint8_t timeBuffer[7] = {0};
uint8_t timeRead[7] = {0};
char time[16],cal[16];
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_I2C1_Init(void);
/* USER CODE BEGIN PFP */
void date_display(uint8_t val);
void convertTimeIntToString(void)
{
	sprintf(time, "  %d:%d:%d", timeRead[2], timeRead[1],timeRead[0]);
	sprintf(cal,"   %d-%d-20%d",timeRead[4], timeRead[5],timeRead[6]);
}
void printNumber(uint8_t val)
{
	char* str;
	if(val<10) sprintf(str,"0%d",val);
	else sprintf(str,"%d",val);
	print_LCD(str,2);
}
void clockDisplay(void){
	move_LCD(1,1);
	printNumber(timeRead[2]);putChar_LCD(':');
	printNumber(timeRead[1]);putChar_LCD(':');
	printNumber(timeRead[0]);
}
void calDisplay(void){
	move_LCD(2,1);
	date_display(timeRead[3]);
	printNumber(timeRead[4]);putChar_LCD('-');
	printNumber(timeRead[5]);putChar_LCD('-');
	print_LCD("20",2); printNumber(timeRead[6]);
}
void display(void)
{
	move_LCD(1,1);
	print_LCD(time,16);
	move_LCD(2,1);
	date_display(timeRead[3]);
	print_LCD(cal,16);
	/*clockDisplay();
	calDisplay();*/
}
int bcd2dec(uint8_t num)
{
		return ((num/16 * 10) + (num % 16));
}

int dec2bcd(uint8_t num)
{
		return ((num/10 * 16) + (num % 10));
}

void DS1307_SetRegByte(uint8_t regAddr, uint8_t val) {
    uint8_t bytes[2] = { regAddr, dec2bcd(val) };
    HAL_I2C_Master_Transmit(&hi2c1, DS1307_I2C_ADDR << 1, bytes, 2, DS1307_TIMEOUT);
}
uint8_t DS1307_GetRegByte(uint8_t regAddr) {
    uint8_t val;
    HAL_I2C_Master_Transmit(&hi2c1, DS1307_I2C_ADDR << 1, &regAddr, 1, DS1307_TIMEOUT);
	__NOP();
	__NOP();
    HAL_I2C_Master_Receive(&hi2c1, DS1307_I2C_ADDR << 1, &val, 1, DS1307_TIMEOUT);
	__NOP();
	__NOP();
    return bcd2dec(val);
}
void writeDS1307(uint8_t hour, uint8_t min, uint8_t sec, uint8_t wday, uint8_t date, uint8_t month, uint8_t year){
	/*uint8_t buffer[8];
	buffer[0]=0x00;
	buffer[1]=dec2bcd(sec);
	buffer[2]=dec2bcd(min);
	buffer[3]=dec2bcd(hour);
	buffer[4]=dec2bcd(wday);
	buffer[5]=dec2bcd(date);
	buffer[6]=dec2bcd(month);
	buffer[7]=dec2bcd(year);
	HAL_I2C_Master_Transmit(&hi2c1,DS1307_I2C_ADDR << 1,buffer,8,DS1307_TIMEOUT);*/
	DS1307_SetRegByte(DS1307_REG_SECOND,sec);
	DS1307_SetRegByte(DS1307_REG_MINUTE,min);
	DS1307_SetRegByte(DS1307_REG_HOUR,hour);
	DS1307_SetRegByte(DS1307_REG_DOW,wday);
	DS1307_SetRegByte(DS1307_REG_DATE,date);
	DS1307_SetRegByte(DS1307_REG_MONTH,month);
	DS1307_SetRegByte(DS1307_REG_YEAR,year);
}
void readDS1307(void)
{
	timeRead[0]=DS1307_GetRegByte(DS1307_REG_SECOND);
	timeRead[1]=DS1307_GetRegByte(DS1307_REG_MINUTE);
	timeRead[2]=DS1307_GetRegByte(DS1307_REG_HOUR);
	timeRead[3]=DS1307_GetRegByte(DS1307_REG_DOW);
	timeRead[4]=DS1307_GetRegByte(DS1307_REG_DATE);
	timeRead[5]=DS1307_GetRegByte(DS1307_REG_MONTH);
	timeRead[6]=DS1307_GetRegByte(DS1307_REG_YEAR);
}
uint8_t toASCII(uint8_t val)
{
	switch(val)
	{
		case 0: val= 0x30; break;
		case 1: val= 0x31; break;
		case 2: val= 0x32; break;
		case 3: val= 0x33; break;
		case 4: val= 0x34; break;
		case 5: val= 0x35; break;
		case 6: val= 0x36; break;
		case 7: val= 0x37; break;
		case 8: val= 0x38; break;
		case 9: val= 0x39; break;
	}
	return val;
}
void time_display(uint8_t *time)
{
	char hnm[5] ;//hour and min
	hnm[0]=(char)toASCII(time[1]/10);
	hnm[1]=(char)toASCII(time[1]%10);
	hnm[2]=':';
	hnm[3]=(char)toASCII(time[0]/10);
	hnm[4]=(char)toASCII(time[0]%10);
	move_LCD(1,1);
	putChar_LCD(' ');
	putChar_LCD(' ');
	print_LCD(hnm,5);
}
void date_display(uint8_t val)
{
	switch(val)
	{
		case 2: print_LCD("Mon ",3); break;
		case 3: print_LCD("Tue ",3); break;
		case 4: print_LCD("Wed ",3); break;
		case 5: print_LCD("Thu ",3); break;
		case 6: print_LCD("Fri ",3); break;
		case 7: print_LCD("Sat ",3); break;
		case 1: print_LCD("Sun ",3); break;
	}
}
void cal_display(uint8_t *time)
{
	char hnm[10] ;//hour and min
	hnm[0]=(char)toASCII(time[3]/10);
	hnm[1]=(char)toASCII(time[3]%10);
	hnm[2]='/';
	hnm[3]=(char)toASCII(time[4]/10);
	hnm[4]=(char)toASCII(time[4]%10);
	hnm[5]='/';
	hnm[6]=(char)toASCII(2);
	hnm[7]=(char)toASCII(0);
	hnm[8]=(char)toASCII(time[5]/10);
	hnm[9]=(char)toASCII(time[5]%10);
	move_LCD(2,1);
	date_display(time[2]);
	putChar_LCD(' ');
	putChar_LCD(' ');
	print_LCD(hnm,10);
}
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
  MX_I2C1_Init();
  /* USER CODE BEGIN 2 */
	/*lcd16x2_init_4bits(LCD_RS_GPIO_Port, LCD_RS_Pin, LCD_E_Pin,
      LCD_D4_GPIO_Port, LCD_D4_Pin, LCD_D5_Pin, LCD_D6_Pin, LCD_D7_Pin);

  lcd16x2_printf("Hello World");*/
	init_LCD(); 
	clr_LCD(); 
	putChar_LCD(' '); 
	putChar_LCD(' '); 
	char* row1 = "Demo of the";
	print_LCD(row1,12); 
	move_LCD(2,1); 
	print_LCD("2x16 LCD Display",16); 
	clr_LCD(); 
	HAL_Delay(500);
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
		readDS1307();
		convertTimeIntToString();
		display();
		HAL_Delay(1000);//delay 1s 
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
  * @brief I2C1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_I2C1_Init(void)
{

  /* USER CODE BEGIN I2C1_Init 0 */

  /* USER CODE END I2C1_Init 0 */

  /* USER CODE BEGIN I2C1_Init 1 */

  /* USER CODE END I2C1_Init 1 */
  hi2c1.Instance = I2C1;
  hi2c1.Init.ClockSpeed = 100000;
  hi2c1.Init.DutyCycle = I2C_DUTYCYCLE_2;
  hi2c1.Init.OwnAddress1 = 0;
  hi2c1.Init.AddressingMode = I2C_ADDRESSINGMODE_7BIT;
  hi2c1.Init.DualAddressMode = I2C_DUALADDRESS_DISABLE;
  hi2c1.Init.OwnAddress2 = 0;
  hi2c1.Init.GeneralCallMode = I2C_GENERALCALL_DISABLE;
  hi2c1.Init.NoStretchMode = I2C_NOSTRETCH_DISABLE;
  if (HAL_I2C_Init(&hi2c1) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN I2C1_Init 2 */

  /* USER CODE END I2C1_Init 2 */

}

/**
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
static void MX_GPIO_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOC, GPIO_PIN_13, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOA, LCD_D4_Pin|LCD_D5_Pin|LCD_D6_Pin|LCD_D7_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOB, LCD_RS_Pin|LCD_RW_Pin|LCD_E_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin : PC13 */
  GPIO_InitStruct.Pin = GPIO_PIN_13;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

  /*Configure GPIO pins : LCD_D4_Pin LCD_D5_Pin LCD_D6_Pin LCD_D7_Pin */
  GPIO_InitStruct.Pin = LCD_D4_Pin|LCD_D5_Pin|LCD_D6_Pin|LCD_D7_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_OD;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /*Configure GPIO pins : LCD_RS_Pin LCD_RW_Pin LCD_E_Pin */
  GPIO_InitStruct.Pin = LCD_RS_Pin|LCD_RW_Pin|LCD_E_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

}

/* USER CODE BEGIN 4 */
void portA_Direction(int direction){
	// purpose is change direction of port A, pin 4|5|6|7
	//correspond mode register of them when cofigure is CRL(Port Configuration Register Low)(Port x configuration bits (y= 0->7))
	//correspond bit of MODE  = bit (0:1)	(4:5)	(8:9)	 	(12:13) |	(16:17)	(20:21)	(24:25)	(28:29)
	//00-Analog,01-Floating Input, 10-Input with pull up|pull down, 11-Reversed
	//correspond bit of CNF 	= bit (2:3)	(6:7)	(10:11)	(14:15)	| (18:19)	(22:23)	(26:27)	(30:31)
	//00-General purpose output push-pull, 01-00-General purpose output open-drain
	if (direction == 0) {
			// Configure pins 4-7 as inputs with floating configuration
			GPIOA->CRL &= 0x0000FFFF;
			GPIOA->CRL |= 0x44440000;
	} else {
			// Configure pins 4-7 as outputs with open-drain configuration
			GPIOA->CRL &= 0x0000FFFF;
			GPIOA->CRL |= 0x11110000;
	}
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
  __disable_irq();
  while (1)
  {
  }
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
