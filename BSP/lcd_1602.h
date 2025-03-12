#ifndef __LCD_1602_H
#define __LCD_1602_H

#include "main.h"
#include "gpio.h"

void LCD_INIT(void);
void lcd1602_show_string(uint8_t x,uint8_t y,char *str);
#endif