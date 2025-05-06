# STM32 Motion Activated LCD Counter
![20250505_231031](https://github.com/user-attachments/assets/fe56c9f7-db4c-428f-8116-adf802fbb9dc)

## Description

This project demonstrates interfacing a standard 16x2 Character LCD (HD44780 compatible) with an STM32F401RE microcontroller (Nucleo-F401RE board). It reads input from a digital motion sensor, displays a running count of motion detection events, shows the current detection status, and controls an indicator LED.

A key aspect of this project involves handling the 3.3V logic level of the STM32 with the typical 5V logic level of the LCD using the **Output Open Drain** GPIO configuration with external pull-up resistors, avoiding the need for a dedicated logic level shifter IC for the output signals. Timing-critical operations for the LCD are handled using the DWT cycle counter for microsecond precision.

## Hardware Required

* **Microcontroller Board:** STMicroelectronics Nucleo-F401RE
* **Display:** 16x2 Character LCD (HD44780 compatible, 5V typical)
* **Sensor:** Digital Motion Sensor: HC-SR501 PIR Sensor
* **Potentiometer:** 10kΩ (for LCD contrast adjustment)
* **Resistors:**
    * 7 x Pull-up resistors 10kΩ for LCD signal lines (RS, E, D4-D7) connected to 5V.
    * 1 x Current-limiting resistor 220Ω - 470Ω for LCD backlight (A/LED+ pin).
* **LED:** 1 x Standard LED (for motion indication)
* **Other:** Breadboard, Jumper Wires, USB cable (for programming/powering Nucleo board), 5V Power Source 

## Software & Tools

* **IDE:** STM32CubeIDE
* **Framework/Libraries:** STM32 HAL (Hardware Abstraction Layer)
* **Programming Language:** C
* **Key Components:**
    * Custom LCD driver functions (implementing HD44780 4-bit protocol)
    * Used DWT Cycle Counter library (`dwt_delay.h`) for microsecond timing.

## Features

* Initializes and controls a 16x2 Character LCD in 4-bit mode.
* Detects digital HIGH signal from a motion sensor.
* Counts motion detection events accurately (counts only once per LOW-to-HIGH transition).
* Displays the running motion count on the first line of the LCD.
* Displays the current status ("Detected!" or "Not Detected") on the second line of the LCD.
* Activates an LED indicator when motion is detected.
* Uses `snprintf` for formatting dynamic data (the counter) for display.

## Wiring & Configuration

**1. LCD Connections:**
    * LCD VSS -> GND
    * LCD VDD -> +5V
    * LCD V0 -> Middle pin (wiper) of 10kΩ potentiometer
    * Potentiometer Outer Pins -> +5V and GND
    * LCD RW -> GND (Write-only mode)
    * LCD K (LED-) -> GND
    * LCD A (LED+) -> Resistor (330Ω) -> +5V
    * **Signal Lines (with Open Drain workaround):**
        * STM32 **PA6** -> LCD RS Pin. Also connect a pull-up resistor (4.7k-10kΩ) from this line to **+5V**.
        * STM32 **PA7** -> LCD E Pin. Also connect a pull-up resistor from this line to **+5V**.
        * STM32 **PB6** -> LCD D4 Pin. Also connect a pull-up resistor from this line to **+5V**.
        * STM32 **PC7** -> LCD D5 Pin. Also connect a pull-up resistor from this line to **+5V**.
        * STM32 **PA9** -> LCD D6 Pin. Also connect a pull-up resistor from this line to **+5V**.
        * STM32 **PA8** -> LCD D7 Pin. Also connect a pull-up resistor from this line to **+5V**.

**2. Sensor Connection:**
    * Sensor VCC -> +5V
    * Sensor GND -> GND
    * Sensor OUT -> STM32 **PB8**

**3. LED Connection:**
    * LED Anode -> STM32 **PB9**
    * LED Cathode -> Resistor (330Ω) -> GND

**4. STM32CubeMX Configuration:**
    * **PA6, PA7, PA8, PA9, PB6, PC7:** Configured as `GPIO_Output_Open_Drain`, `No pull-up and no pull-down`.
    * **PB8:** Configured as `GPIO_Input`, `No pull-up and no pull-down` (or adjust Pull setting based on sensor datasheet).
    * **PB9:** Configured as `GPIO_Output_Push_Pull`, `No pull-up and no pull-down`.
    * Ensure System Clock is configured
    * Ensure DWT Cycle Counter is usable (often enabled by default or requires minimal setup).

## Building & Running

1.  Clone this repository.
2.  Open the project using STM32CubeIDE.
3.  Ensure your toolchain settings are correct for the Nucleo-F401RE board.
4.  Build the project (Project -> Build All).
5.  Connect the Nucleo board to your computer via USB.
6.  Flash the built program to the Nucleo board (Run -> Debug/Run As -> STM32 Application).
7.  Ensure all hardware components are wired correctly according to the 'Wiring & Configuration' section.
8.  Power the system (USB provides power to Nucleo, ensure 5V is supplied to LCD/pull-ups).
9.  Adjust the potentiometer until text is clearly visible on the LCD.
10. Trigger the motion sensor to observe the counter incrementing and status changes.

## Key Concepts Demonstrated

* STM32 HAL Library Usage
* GPIO Configuration (Input, Output Open Drain, Push-Pull)
* HD44780 LCD Interfacing (4-bit mode protocol)
* Custom Peripheral Driver Implementation (for LCD)
* Logic Level Interfacing Workaround (3.3V -> 5V using Open Drain + Pull-ups)
* Accurate Microsecond Timing (using DWT Cycle Counter)
* Digital Sensor Integration and Input Handling
* State Machine Logic for Event Debouncing/Counting (motion flag)
* Dynamic String Formatting (`snprintf`) for Embedded Displays
* Systematic Debugging of Embedded Hardware/Software Issues

## Applications

* For some reason it just feels like it can be refined to be a paper towel dispenser that dispenses when motion activated

## Future Improvements
* Implement sensor reading using EXTI interrupts instead of polling.
* Add more sensors using I2C or SPI.
* Use an RTOS to manage tasks.
* Add configuration options via UART.
* Optimize for low power consumption.
