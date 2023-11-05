#ifndef _LCD_H
#define _LCD_H
#include "main.h"
void lcd_init (void);
void send_to_lcd (char data, int rs);
void lcd_send_cmd (char cmd);
void lcd_send_data (char data);
void lcd_put_cur(int row, int col);
void lcd_send_string(char* str, uint8_t length);
#endif