#include <stdio.h>
#include <wiringPi.h>
#define PIN_NUM 27 // pin number
 
int main(void){
 
        
 
        // wiringPi setup
        if(wiringPiSetup() == -1){
                return -1;
        }
 
        // pin = 27
        pinMode(PIN_NUM, OUTPUT);
 
        // LED toggle
        while(1){
                digitalWrite(PIN_NUM,1); // pin 27, input 1, light ON
                delay(500); // Delay 0.5 sec
                digitalWrite(PIN_NUM,0); // light OFF
                delay(500); // Delay 0.5 sec
        }
 
        return 0;
}