# Odometry module with 2 Infineon TLE5012B

This module uses the SPI2 instance with the following setting. Both sensors use the same SPI bus. Only the CS pin is different for each side.

| Property | Value     |
| -------- | --------- |
| SPI instance | SPI2  |
| Pin SCK  | PB13       |
| Pin MOSI | PC3       |
| Pin CS left  | PC2      |
| Pin CS right | PC4      |
| CLK frequency[kHz] | 314.5     |

## Usage

In order to use/init the module correctly, call `odometry_init()`.
