# Checklist mergerequest to master

Before sending a mergerequest check following points:

- **No binaries**
- Strict layers
	-  Calls only from top to bottom.
	1. Application (main.c)
	2. Module
	3. (additional abstraction layer)
	4. Low level drivers (includes STM32L4xx.h)
- In 4th layer: no direct instantiation of hardware in C files. Use `#defines` in Header file instead ([example](debug/debug_hardware.h#L21)). This is makes hardware changes more flexible and the C file is unchanged.

### Optional

- Write a small `README.md` with functions, hardware configuration and usage of your module and place it in your module folder. Your colleagues will appreciate it!

### Thank you!
