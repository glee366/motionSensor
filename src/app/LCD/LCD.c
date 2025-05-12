
#include <string.h>
#include <stdio.h>
#include <stdint.h>
#include "main.h"
#include "stm32f4xx_hal.h"
#include "dwt_delay.h"
/**
 * @brief Falling edge clock initialize the LCD display
 */

void lcd_enable_pulse(void) {
    HAL_GPIO_WritePin(GPIOA, GPIO_PIN_7, SET);
    DWT_Delay(2); // Enable pulse high time
    HAL_GPIO_WritePin(GPIOA, GPIO_PIN_7, RESET);
    DWT_Delay(50); // Enable cycle time / execution time
}

void lcd_send_nibble(uint8_t nibble) {
    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_6, (nibble >> 0) & 0x01);
    HAL_GPIO_WritePin(GPIOC, GPIO_PIN_7, (nibble >> 1) & 0x01);
    HAL_GPIO_WritePin(GPIOA, GPIO_PIN_9, (nibble >> 2) & 0x01);
    HAL_GPIO_WritePin(GPIOA, GPIO_PIN_8, (nibble >> 3) & 0x01);
}

void lcd_send(uint8_t value, uint8_t rs) {
    // Set RS pin
    HAL_GPIO_WritePin(GPIOA, GPIO_PIN_6, rs ? SET : RESET);

    // Send high nibble
    lcd_send_nibble(value >> 4);
    lcd_enable_pulse();

    // Send low nibble
    lcd_send_nibble(value & 0x0F);
    lcd_enable_pulse();
}

/**
 * @brief send command function to set text on LCD display
 * @param int cmd
 */

void lcd_send_cmd(uint8_t cmd) {
    lcd_send(cmd, 0);
    // Most commands need > 37us execution time
    // Clear (0x01) and Return Home (0x02) need ~1.52ms
    if (cmd == 0x01 || cmd == 0x02) {
        HAL_Delay(2);
    } else {
        DWT_Delay(100); // General command delay
    }
}

void lcd_send_data(uint8_t data) {
    lcd_send(data, 1);
    DWT_Delay(100); // Data write delay
}

void lcd_init(void) {
    // 1. Wait >40ms after VCC rises to 4.5V
    HAL_Delay(50);

    // 2. Send Function Set (0x30) 3 times, with delays. Puts LCD in 8-bit mode initially.
    lcd_send_nibble(0x03); lcd_enable_pulse(); HAL_Delay(5); // Wait > 4.1ms
    lcd_send_nibble(0x03); lcd_enable_pulse(); HAL_Delay(100); // Wait > 100us
    lcd_send_nibble(0x03); lcd_enable_pulse(); HAL_Delay(100);

    // 3. Set 4-bit mode
    lcd_send_nibble(0x02); lcd_enable_pulse(); HAL_Delay(100);

    // 4. Function Set: 4-bit, 2 lines, 5x8 font
    lcd_send_cmd(0x28);

    // 5. Display ON/OFF Control: Display ON, Cursor OFF, Blink OFF
    lcd_send_cmd(0x0C);

    // 6. Clear Display
    lcd_send_cmd(0x01); // Requires HAL_Delay(2) inside lcd_send_cmd

    // 7. Entry Mode Set: Increment cursor position, no display shift
    lcd_send_cmd(0x06);
}

void lcd_clear() {
    lcd_send_cmd(0x01); // Clear display command
}

void lcd_put_cur(uint8_t row, uint8_t col) {
    uint8_t address;
    switch (row) {
        case 0: address = 0x00; break;
        case 1: address = 0x40; break;
        default: address = 0x00; break;
    }
    lcd_send_cmd(0x80 | (address + col)); // Set DDRAM address command
}

void lcd_send_string(char *str) {
    while (*str) {
        lcd_send_data(*str++);
    }
}
