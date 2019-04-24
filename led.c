#include <stdio.h>
#include <wiringPi.h>
#define PIN_NUM 27

int main()
{
	if(wiringPiSetup() == -1)
	{
		return -1;
	}
	
	pingMode(PIN_NUM, OUTPUT);
	
	while(1)
	{
		digitalWrite(PIN_NUM, 1);
		delay(500);
		digitalWrite(PIN_NUM, 0);
		delay(500);
	}
	
	return 0;
}