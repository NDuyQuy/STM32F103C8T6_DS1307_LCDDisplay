# DHT11_LED7
# Description 
	Using STM32F103 comunicate I2C(Hardware and use HAL Library) with DS1307 then display on  LCD16X2. 
	I have put a schematic of my similar project with arduino, so you can go in there and have a look to imagine the circuit. There is no different.
	About the configuration of STM32F103, i configure it by the STM32CubeMx.
# pin connection

## STM32F103			                            LCD16X2
    PORT_B PIN 3          							LCD_RS  
	PORT_B PIN 4									LCD_RW 
	PORT_B PIN 5									LCD_E   
	PORT_A PIN 4									LCD_D4
	PORT_A PIN 5									LCD_D5
	PORT_A PIN 6									LCD_D6
	PORT_A PIN 7									LCD_D7
## STM32F103			                            DS1307
	PORT B PIN 6(I2C SCL)							SCL
	PORT B PIN 7(I2C SDA)							SDA