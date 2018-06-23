// **** INCLUDES *****
#include "LowPower.h"

// Use pin 2 as wake up pin
const int wakeUpPin = 2;
int data[2];
void wakeUp()
{
    // Just a handler for the pin interrupt.
}

void setup()
{
    // Configure wake up pin as input.
    // This will consumes few uA of current.
    pinMode(wakeUpPin, INPUT);   
    
}

void loop() 
{
    // Allow wake up pin to trigger interrupt on low.
    attachInterrupt(0, wakeUp, HIGH);
    
    // Enter power down state with ADC and BOD module disabled.
    // Wake up when wake up pin is HIGH.
    LowPower.powerDown(SLEEP_FOREVER, ADC_OFF, BOD_OFF); 
    
    // Disable external pin interrupt on wake up pin.
    detachInterrupt(0); 

    
    data[0] = analogRead(A0);
    data[1] = analogRead(A1);

    delay(200);
    Serial.println(Data[0]);
    delay(10);
    Serial.println(Data[1]);
}
