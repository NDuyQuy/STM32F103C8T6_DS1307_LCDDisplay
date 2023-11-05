#ifndef _MY_LCD_H
#define _MY_LCD_H
#include "main.h"
// define LCD8BIT
char Read2Nib();  //read 2 nibbles from LCD
void Write2Nib(uint8_t chr); //write 2 nibbles into LCD
void Write8Bit(uint8_t  chr); //write 8 bit into LCD
void wait_LCD();             //wait LCD free
void init_LCD();  
void clr_LCD(); 
void home_LCD();  //send cursor to home
void move_LCD(uint8_t y, uint8_t x);  //move cursor to wanted direction (row, collumn)
void putChar_LCD(uint8_t chr);                         //write 1 character on LCD
void print_LCD(char* str, unsigned char len); //display a string on LCD
#endif
