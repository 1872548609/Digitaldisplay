/*!
 * @file        readme.txt
 *
 * @brief       This file is routine instruction
 *
 * @version     V1.0.0
 *
 * @date        2022-09-29
 *
 * @attention
 *
 *  Copyright (C) 2022 Geehy Semiconductor
 *
 *  You may not use this file except in compliance with the
 *  GEEHY COPYRIGHT NOTICE (GEEHY SOFTWARE PACKAGE LICENSE).
 *
 *  The program is only for reference, which is distributed in the hope
 *  that it will be useful and instructional for customers to develop
 *  their software. Unless required by applicable law or agreed to in
 *  writing, the program is distributed on an "AS IS" BASIS, WITHOUT
 *  ANY WARRANTY OR CONDITIONS OF ANY KIND, either express or implied.
 *  See the GEEHY SOFTWARE PACKAGE LICENSE for the governing permissions
 *  and limitations under the License.
 */


&par Example Description

This example shows how to use SPI Peripheral to driver LCD to display multi-level menu.

The USART1 is configured as follows:
    - TX:PA9, RX:PA10
    - BaudRate = 115200 baud
    - Word Length = 8 Bits
    - One Stop Bit
    - No parity
    - Hardware flow control disabled (RTS and CTS signals)
    - Receive and transmit enabled

&par Hardware Description
    - SPI1 CS(PA15)     -> LCD CS
    - SPI1 SCK(PB3)     -> LCD SCL
    - SPI1 MOSI(PB5)    -> LCD SDA
    - PC3               -> LCD DC
    - PC1               -> LCD RST
    - PA1               -> LCD BLK
    - 3.3V              -> LCD VCC
    - GND               -> LCD GND
    - Close J9 and J10

Menu button
  - KEY1: Previous Menu button
  - KEY2: Next Menu button
  - KEY3: Enter Menu button
  - KEY4: Return Menu button

&par Directory contents

  - LCD/LCD_Menu/Source/apm32f0xx_int.c     Interrupt handlers
  - LCD/LCD_Menu/Source/main.c              Main program

&par IDE environment

  - MDK-ARM V5.36
  - EWARM V8.50.4.26131

&par Hardware and Software environment

  - This example runs on APM32F051 EVAL Devices.
