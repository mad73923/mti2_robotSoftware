# Readme WLAN-module
## Pinout
```
PC10 (UART_TX) -> WLAN RX
PC11 (UART_RX) -> WLAN TX
```

## How to use
You have to call use the WLAN-module like in the following code-example
´´´
int main(void){
	system_init();
	debug_init();
	WLANinit(); //does the pin- DMA- and UART-RX-Interrupt-Config
	WLANconnectToAp("RobotStation","StationPassword","10.42.0.10"); //connects to WLAN-AP with SSID=RobotStation, Password=StationPassword and tries to get the IP-Adress=10.42.0.10
	while(WLANgetWLANConnectionStatus()!=1){}       //Wait for WLAN-connection ready
	WLANconnectToTCPserver("10.42.0.143","2323");   //Try to connect to the Server 10.42.0.143 through Port 2323 and start TCP-listener on success
	while(WLANgetTCPConnectionStatus()!=1){}        //Wait for TCP-connection ready
	while(1){

	}
}
´´´
