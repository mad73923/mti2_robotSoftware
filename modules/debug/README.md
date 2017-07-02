# Debug output via UART interface

This module makes Debug prints possible. On Nucleo Boards, it's connected to the VCOM port to forward messages to the debugging PC.
Init first with debug_init();!

Then debug_printf(const char* format, ...) is available with variable parameter count.

So far, there's only one buffer available. The first message can be send asynchronously with DMA usage. If a secound message is send while the predecessor message isn't sent yet, a synchronous queue will be used.

Maximum message length: DEBUG_BUFFERSIZE

## Usage
### void debug_init(void)
This function initializes the debug module and must be called once before using the module functions.

### void debug_printf(const char* format, ...)
**Calling this function inside an interrupt service routine may cause problems!!**
The string specified in format will be printed to the UART. Variable number of parameters is accepted.
```
debug_printf("Test %d %s", someInteger, someString);
```

### void debug_led_on(void);
Turns the debug LED on.

### void debug_led_off(void);
Turns the debug LED off.

### void debug_led_toggle(void);
Toggles the debug LED.

## GPIO Configuration

| Pin  |   Function    |
| ---- | ------------- |
| PA02 | UART TX       |
| PA05 | LED           |

## UART Configuration

| Property | Value     |
| -------- | --------- |
| Instance | USART2    |
| Baudrate | 115200    |
| Databits | 8         |
| Stopbits | 1         |
| Direction | TX only  |

## DMA Configuration

| Property | Value     |
| -------- | --------- |
| Instance | DMA1      |
| Channel  | 7         |
| Request  | 2         |
| IRQ Priority | 1    |

