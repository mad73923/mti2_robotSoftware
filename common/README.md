# Checklist mergerequest to master

Before sending a mergerequest check following points:

- Commit your progress
- **No binaries**
- **Test all functionality 100%**
- Merge master into your branch
- **Test again!!!** Your module mustn't interfere other modules!
- Strict layers
	-  Calls only from top to bottom.
	    1. Application (main.c)
	    2. Module
	    3. (additional abstraction layer)
	    4. Low level drivers (includes STM32L4xx.h)
- In 4th layer: no direct instantiation of hardware in C files. Use `#defines` in Header file instead (for example see modules/debug/debug_hardware.h line 21 ++). This is makes hardware changes more flexible and the C file is unchanged.
- Add folder with used datasheets into your datasheet folder (module/*yourModule*/datasheets)

### Optional

- Write a small `README.md` with functions, hardware configuration and usage of your module and place it in your module folder. Your colleagues will appreciate it!

DONE!
- **Claim mergerequest**

### Thank you!
