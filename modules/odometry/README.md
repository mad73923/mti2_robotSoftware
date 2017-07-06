# Odometry module with 2 Infineon TLE5012B

This module uses the SPI2 instance with the following setting. Both sensors use the same SPI bus. Only the CS pin is different for each side.

| Property | Value     |
| -------- | --------- |
| SPI instance | SPI2  |
| SPI Prio | 11  |
| Pin SCK  | PB13       |
| Pin MOSI | PC3       |
| Pin CS left  | PC2      |
| Pin CS right | PC4      |
| CLK frequency[kHz] | 314.5     |

## Timer for autoupdate

| Property | Value     |
| -------- | --------- |
| Timer instance | TIM6  |
| Timer Prio | 12  |
| Timer Frequency[Hz] | 50  |

Auto update can be enabled / disabled by following functions:

```
void odometry_autoupdate_start(void);
void odometry_autoupdate_stop(void);
```

## Usage

In order to use/init the module correctly, call `odometry_init()`.
