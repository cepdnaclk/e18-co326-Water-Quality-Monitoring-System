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
  // Send the command to get temperatures
  sensors.requestTemperatures(); 

  //get turbidity reading
  int sensorValue = analogRead(A0);
  int turbidity = map(sensorValue, 0, 750, 100, 0); //map the values
  
  //print the temperature in Celsius
  Serial.print("Temperature: ");
  Serial.print(sensors.getTempCByIndex(0));
  Serial.println(" C");
  
  //print turbidity
  Serial.print("Turbidity: ");
  Serial.println(turbidity);  
  
  delay(1000);
}