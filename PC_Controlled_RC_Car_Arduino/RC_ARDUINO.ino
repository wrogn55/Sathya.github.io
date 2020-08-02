#include <VirtualWire.h>
void setup()
{
 //Begin the serial connection
 Serial.begin(9600);
 while(!Serial); //Wait till serial connection is ready
 Serial.println("Serial ready"); //Debugging
 //Initialize the IO and ISR
 vw_setup(2000); // Bits per sec
}
void loop()
{
 //If serial input is available
 if(Serial.available())
 {
 char input = Serial.read(); //Read input
 send(&nput); //Send input via RF transmitter
 } else {
 }
}
void send (char *message)
{
 vw_send((uint8_t *)message, strlen(message)); //Send the message
 vw_wait_tx(); // Wait until the whole message is gone
}