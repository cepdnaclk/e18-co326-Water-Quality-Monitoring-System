#include <OneWire.h>
#include <DallasTemperature.h>
 
// Data wire is connected to digital pin 4 on the Arduino
#define ONE_WIRE_BUS 4
 
// Setup a oneWire instance to communicate with any OneWire device
OneWire oneWire(ONE_WIRE_BUS);	
 
// Pass oneWire reference to DallasTemperature library
DallasTemperature sensors(&oneWire);
 
void setup(void){
  sensors.begin();	// Start up the library
  Serial.begin(9600);
}
 
void loop(void){ 
  
  int sensorValue = analogRead(A0);
  //Serial.println(sensorValue);
  int turbidity = map(sensorValue, 0, 750, 100, 0);
  Serial.println(turbidity);
  delay(1000);
  
  
}