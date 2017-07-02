# Readme Motor-Module
## Pinout
```
PA5 (DIR LEFT)
PA6 (DIR RIGHT)
PB6 (PWM LEFT)
PA7 (PWM RIGHT)
PB5 (BEEPER)
```
## Used Timers
```
TIMER 3 (HARDWARE PWM RIGHT MOTOR)
TIMER 4 (HARDWARE PWM LEFT MOTOR)
```

## How to initialize
To initialize the motor-module you have to do the following:

```c
int main(void){
	system_init();
	debug_init();

	motor_timer_init(); //This function initializes the PWM-TIMER (Motors) and the GPIOs (Direction)

	while(1){
	}
}
```

## How to use
To use the motors you have to do the following (in this example I take the left motor):

```c
int main(void){
	system_init();
	debug_init();

	motor_timer_init(); //This function initializes the PWM-TIMER (Motors) and the GPIOs (Direction)

	while(1){

		motor_setSpeed(MOTORLEFT, 1000);	//In this function you set the speed of the motor by choosing which motor you 
		                                    want (MOTORLEFT, MOTORRIGHT) and the speed you want to have (0-1999)

		//do other stuff

		if(motor_has_to_stop){
			motor_stop(MOTORLEFT);			//In this function you can stop the motor
		}

	}
}
```

