# UID module

This module gets the device-UID. The device electronic signature is stored in the System memory area of the Flash memory module, and can be read using the debug interface or by the CPU. It contains factory-programmed identification and calibration data that allow the user firmware or other external devices to automatically match to the characteristics of the STM32L4x6 microcontroller.

No init needed.

No hardware instances needed.

## Usage

`uid_getUIDString` returns a pointer to the UID-string in the the following format: `[0-9a-fA-F]{2}(:[0-9a-fA-F]{2}){11}`. 36 chars total, \0 included.
