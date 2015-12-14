
#include <iostream>
#include <string>
#include <unistd.h>
#include "SimpleGPIO.h"

#include<opencv2/opencv.hpp>
using namespace cv; 

using namespace std;

//unsigned int LEDGPIO = 60;   // GPIO1_28 = (1x32) + 28 = 60
unsigned int LEDGPIO = 23; 	//GPIO0_23 = (0*32) + 23 = 50
//unsigned int ButtonGPIO = 15;   // GPIO0_15 = (0x32) + 15 = 15
unsigned int SensorOut = 61; 	//GPIO1_29 = (1*32) + 29 = 61

int main(int argc, char *argv[]){

	cout << "Testing the PIR Motion Sensor!" << endl;

        gpio_export(LEDGPIO);    // The LED
        //gpio_export(ButtonGPIO);   // The push button switch
	gpio_export(SensorOut); 	//The Output of the PIR Sensor
        gpio_set_dir(LEDGPIO, OUTPUT_PIN);   // The LED is an output
	//gpio_set_dir(ButtonGPIO, INPUT_PIN);   // The push button input
	gpio_set_dir(SensorOut, INPUT_PIN); 	

	// Flash the LED 5 times
	for(int i=0; i<5; i++){
		cout << "Setting the LED on" << endl;
                gpio_set_value(LEDGPIO, HIGH);
		usleep(200000);         // on for 200ms
		cout << "Setting the LED off" << endl;
                gpio_set_value(LEDGPIO, LOW);
		usleep(200000);         // off for 200ms
	}

	// Wait for the push button to be pressed
	unsigned int value = LOW; 
	//cout << "Sensor will now start detecting!" << endl;

    bool detect = true;
    
    while(detect){
        gpio_get_value(SensorOut, &value);
        if(value == HIGH){
            usleep(5000000);
            gpio_get_value(SensorOut, &value);
            if(value==HIGH){
                
            cout<<"Motion Detected"<<endl;
            cout << endl <<  "End of Motion Detection!" << endl;
            
            //cout << "Finished Testing the Motion Sensor & GPIO Pins" << endl;
            detect = false;
            
            }
        }
        usleep(1000000);
    
    }
	    gpio_unexport(LEDGPIO);     // unexport the LED
    gpio_unexport(SensorOut);  // unexport the push button

	return 0;
}

