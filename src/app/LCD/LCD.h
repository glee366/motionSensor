/**
 * Helper file for LCD display
 */
#include <string.h>
#include <stdio.h>
#include <stdint.h>
#include "main.h"
#include "stm32f4xx_hal.h"

#ifndef LCD_H_
#define LCD_H_

void lcd_enable_pulse(void);

void lcd_send_nibble(uint8_t nibble);

void lcd_send(uint8_t value, uint8_t rs);

void lcd_send_cmd(uint8_t cmd);

void lcd_send_data(uint8_t data);

void lcd_init(void);

void lcd_clear(void);

void lcd_put_cur(uint8_t row, uint8_t col);

void lcd_send_string(char *str);

#endif
