#include <stdio.h>
#include <wiringPi.h>

#define LED_PIN 27 // pin number
#define BT_PIN 0
 
int main(void){
        
 
        // wiringPi setup
        if(wiringPiSetup() == -1){
                return -1;
        }
 
        // pin = 27
        pinMode(LED_PIN, OUTPUT);
        pinMode(BT_PIN, INPUT);
        
        pullUpDnControl(BT_PIN, PUD_UP);
 
        // LED toggle
        while(1){
        	if(!(digitalRead(BT_PIN)))
        	{
        		digitalWrite(LED_PIN,1); // pin 27, input 1, light ON
            delay(500); // Delay 0.5 sec
            digitalWrite(LED_PIN,0); // light OFF
            delay(500); // Delay 0.5 sec
        	}
        }
        return 0;
}