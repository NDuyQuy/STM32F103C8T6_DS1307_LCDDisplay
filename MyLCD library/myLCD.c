#include "myLCD.h"
char Read2Nib()
{
	uint16_t data_read;
	char HNib, LNib; //H=HIGH ;L=LOW
	portA_Direction(0); //change portA_Direction -> input
	HAL_GPIO_WritePin(LCD_E_GPIO_Port,LCD_E_Pin,1);//ENABLE
	data_read = GPIOA->IDR;
	HNib = data_read & 0XF0;
	HAL_GPIO_WritePin(LCD_E_GPIO_Port,LCD_E_Pin,0);//DISABLE
	HAL_GPIO_WritePin(LCD_E_GPIO_Port,LCD_E_Pin,1);//ENABLE
	data_read = GPIOA->IDR;
	LNib = data_read & 0XF0;
	HAL_GPIO_WritePin(LCD_E_GPIO_Port,LCD_E_Pin,0);//DISABLE
	LNib>>=4;
	return (HNib|LNib);
}
void Write2Nib(uint8_t chr)
{
	uint8_t HNib,LNib,temp_data;
	temp_data = (GPIOA->ODR) & 0X0F;
	HNib=chr & 0xF0;
  LNib=(chr<<4) & 0xF0;
	GPIOA->ODR = (HNib|temp_data);
	HAL_GPIO_WritePin(LCD_E_GPIO_Port,LCD_E_Pin,1);//ENABLE
	HAL_GPIO_WritePin(LCD_E_GPIO_Port,LCD_E_Pin,0);//DISABLE
	GPIOA->ODR = (LNib|temp_data);
	HAL_GPIO_WritePin(LCD_E_GPIO_Port,LCD_E_Pin,1);//ENABLE
	HAL_GPIO_WritePin(LCD_E_GPIO_Port,LCD_E_Pin,0);//DISABLE
}
void Write8Bit(uint8_t  chr)
{
	GPIOA->ODR = chr;// OUT 8 BIT TO GPIO A 
	HAL_GPIO_WritePin(LCD_E_GPIO_Port,LCD_E_Pin,1);//ENABLE
	HAL_GPIO_WritePin(LCD_E_GPIO_Port,LCD_E_Pin,0);//DISABLE
}
void wait_LCD()
{
	HAL_Delay(1);
}
void init_LCD()
{
	//function set 
	HAL_GPIO_WritePin(LCD_E_GPIO_Port,LCD_RS_Pin,0);
	HAL_GPIO_WritePin(LCD_E_GPIO_Port,LCD_RW_Pin,0);//rw 0 : write | 1: read
	HAL_GPIO_WritePin(LCD_E_GPIO_Port,LCD_E_Pin,0);
	#ifdef LCD8BIT 
		Write8Bit(0x38);
		wait_LCD();
	#else 
		HAL_GPIO_WritePin(LCD_E_GPIO_Port,LCD_E_Pin,0);//enable
		HAL_GPIO_WritePin(GPIOA,GPIO_PIN_5,1);
		HAL_GPIO_WritePin(LCD_E_GPIO_Port,LCD_E_Pin,0);//DISABLE
		wait_LCD();
		Write2Nib(0x28);
		wait_LCD();
	#endif
	//Display control-------------------------------------------------------------------------    
			HAL_GPIO_WritePin(LCD_E_GPIO_Port,LCD_RS_Pin,0); // the following data is COMMAND
       #ifdef LCD8BIT
              Write8Bit(0x0E);
              wait_LCD();
       #else
              Write2Nib(0x0E);
              wait_LCD();
       #endif
//Entry mode set------------------------------------------------------------------------
       HAL_GPIO_WritePin(LCD_E_GPIO_Port,LCD_RS_Pin,0); // the following data is COMMAND
       #ifdef LCD8BIT
              Write8Bit(0x06);
              wait_LCD();
       #else
              Write2Nib(0x06);
              wait_LCD();
       #endif
}
void home_LCD(){
       HAL_GPIO_WritePin(LCD_E_GPIO_Port,LCD_RS_Pin,0); // the following data is COMMAND
        #ifdef LCD8BIT
              Write8Bit(0x02);
              wait_LCD();
       #else
              Write2Nib(0x02);
              wait_LCD();
       #endif 
}
void move_LCD(uint8_t y,uint8_t x)
	{
       uint8_t Ad;
       Ad=64*(y-1)+(x-1)+0x80; // calculate the instruction code
       HAL_GPIO_WritePin(LCD_E_GPIO_Port,LCD_RS_Pin,0); // the following data is COMMAND
       #ifdef LCD8BIT
              Write8Bit(Ad);
              wait_LCD();
       #else
              Write2Nib(Ad);
              wait_LCD();
       #endif 
}
void clr_LCD()//clear LCD screen 
{	
       HAL_GPIO_WritePin(LCD_E_GPIO_Port,LCD_RS_Pin,0); //RS=0 mean the following data is COMMAND (not normal DATA)
       #ifdef LCD8BIT
              Write8Bit(0x01);
              wait_LCD();
       #else             
							Write2Nib(0x01);
              wait_LCD();
       #endif
}
void putChar_LCD(uint8_t chr)
{ 
       HAL_GPIO_WritePin(LCD_E_GPIO_Port,LCD_RS_Pin,1); //this is a normal DATA
       #ifdef LCD8BIT
              Write8Bit(chr);
              wait_LCD();
       #else
              Write2Nib(chr);
              wait_LCD();
       #endif
}
void print_LCD(char* str, unsigned char len)
{ 
       unsigned char i;
       for (i=0; i<len; i++){
             if(str[i] > 0) putChar_LCD(str[i]);
             else putChar_LCD(' ');
       }
}