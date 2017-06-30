# Readme Env_Sensor_Module
## Pinout
```
PA0	(ANALOG_IN_0 (0°-180°))
PA1	(ANALOG_IN_1 (180°-360°))
PC7	(MOTOR_SERVO_PWM_PIN)
```
## Used Timers
```
TIMER 5 (TIMER TO START ADC CONVERSION)
TIMER 8 (HARDWARE PWM SERVO MOTOR)
```

## How to initialize
To initialize the env_sensor-module you have to do the following:

```c
int main(void){
	system_init();
	debug_init();
	
	adc_init();					//Init ADC-Channel on pin PA0 & PA1
	env_timer_init();			//Init Timer-Interrupt to start ADC-Conversion		
	servo_timer_init();			//Init Servo-Motor-PWM-Channel 
	
	start_env_data_collector();	//Starting Environment Data Collection

	while(1){

	}
}
```

## How to use
To use the environment_sensors you have to do the following:

```c
int main(void){
	system_init();
	debug_init();

	adc_init();					//Init ADC-Channel on pin PA0 & PA1
	env_timer_init();			//Init Timer-Interrupt to start ADC-Conversion		
	servo_timer_init();			//Init Servo-Motor-PWM-Channel 
	
	start_env_data_collector();	//Starting Environment Data Collection

	while(1){
		if(getCurrentDistanceArrayIndex()==(PI_OFFSET-1)){
			for(int i=0; i<NR_VALUES; i++){
				//Iterate through the distance Array and do Stuff with it
			}
		}
	}
}
```

## Functions for manually sensing
To make your own sensing pattern you can use the following functions:

```c
uint16_t servo_set_angle(uint16_t iAngle) 		//iAngle from 0-18 which leads to 0 -> 0° till 18 -> 180°
struct distValues getFrontBackDistance(void)	//The returned struct has the front and backsensor data in it (2 values)

if ((LL_ADC_IsEnabled(ADC_NR) == 1) && (LL_ADC_IsDisableOngoing(ADC_NR) == 0) && (LL_ADC_REG_IsConversionOngoing(ADC_NR) == 0))
	{
		/* Start ADC conversion */
		LL_ADC_REG_StartConversion(ADC_NR);
	}
```
