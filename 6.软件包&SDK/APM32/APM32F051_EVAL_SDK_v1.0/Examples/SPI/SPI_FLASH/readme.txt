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

This example shows how to use SPI Peripheral to write data to W25Q16 flash and read data from W25Q16 
flash.Press KEY1 to start test. If detect operation is success,LCD will display the test information.

The USART1 is configured as follows:
    - TX:PA9, RX:PA10
    - BaudRate = 115200 baud
    - Word Length = 8 Bits
    - One Stop Bit
    - No parity
    - Hardware flow control disabled (RTS and CTS signals)
    - Receive and transmit enabled

&par Hardware Description
    - SPI1 CS(PE12)     -> W25Q16 CS
    - SPI1 SCK(PE13)    -> W25Q16 CLK
    - SPI1 MISO(PE14)   -> W25Q16 DO
    - SPI1 MOSI(PE15)   -> W25Q16 DI
    - Close J9 and J10

&par Directory contents

  - SPI/SPI_FLASH/Source/apm32f0xx_int.c     Interrupt handlers
  - SPI/SPI_FLASH/Source/main.c              Main program

&par IDE environment

  - MDK-ARM V5.36
  - EWARM V8.50.4.26131

&par Hardware and Software environment

  - This example runs on APM32F051 EVAL Devices.
