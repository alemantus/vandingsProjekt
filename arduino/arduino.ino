// **** INCLUDES *****
#include "LowPower.h"
#include <avr/sleep.h>

// Use pin 2 as wake up pin
long int placeHolder = 0;
long int data[2];

void sleepNow()         // here we put the arduino to sleep
{

  set_sleep_mode(SLEEP_MODE_PWR_DOWN);   // sleep mode is set here

  sleep_enable();          // enables the sleep bit in the mcucr register
  // so sleep is possible. just a safety pin


  attachInterrupt(0, wakeUpNow, LOW); // use interrupt 0 (pin 2) and run function
  // wakeUpNow when pin 2 gets LOW

  sleep_mode();            // here the device is actually put to sleep!!
  // THE PROGRAM CONTINUES FROM HERE AFTER WAKING UP

  sleep_disable();         // first thing after waking from sleep:
  // disable sleep...
  detachInterrupt(0);      // disables interrupt 0 on pin 2 so the
  // wakeUpNow code will not be executed
  // during normal running time.
  digitalWrite(LED_BUILTIN, HIGH); 
  delay(500);

}

void wakeUpNow(void)
{
 
}
void setup()
{
  // Configure wake up pin as input.
  // This will consumes few uA of current.
  pinMode(LED_BUILTIN, OUTPUT);

  pinMode(2, INPUT);
  //pinMode(2,INPUT);
  Serial.begin(115200);
  //attachInterrupt(0, pinInterrupt, LOW);
}



void loop()
{
  digitalWrite(LED_BUILTIN, LOW);  // turn LED on to indicate awake
  int i = 0;
  while(i<5){
    placeHolder = placeHolder + analogRead(A0);
    i++;
    delay(200);
  }
  
  data[0] = placeHolder/6;
  //data[1] = analogRead(A1);
  placeHolder=0;
  data[1]=analogRead(A0);
  //delay(200);
  //itoa(data[0], str, 10);
  //Serial.write(String(data[0]).c_str());
  Serial.write(String(data[1]).c_str());
  //delay(10);
  //Serial.println(data[1]);
  delay(6000);
  sleepNow();



}


