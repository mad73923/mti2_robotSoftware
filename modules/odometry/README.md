# Odometry module with 2 Infineon TLE5012B

This module uses the SPI1 instance with the following setting. Both sensors use the same SPI bus. Only the CS pin is different for each side.

| Property | Value     |
| -------- | --------- |
| Pin SCK  | PB3       |
| Pin MOSI | PB5       |
| Pin CS left  | PA15      |
| Pin CS right | PA14      |
| CLK frequency[kHz] | 314.5     |

## Usage

In order to use/init the module correctly, call `odometry_init()`.
