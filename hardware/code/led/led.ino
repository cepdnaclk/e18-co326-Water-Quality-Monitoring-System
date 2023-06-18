#define D7 13 
#define D8 15 

void setup() {
  pinMode(D7, OUTPUT); // set the digital pin as output.
  pinMode(D8, OUTPUT); // set the digital pin as output.
}

void loop() {
  digitalWrite(D7, HIGH);
  delay(1000);          
  digitalWrite(D7, LOW); 
  delay(1000);    
  
  digitalWrite(D8, HIGH);
  delay(1000);          
  digitalWrite(D8, LOW); 
  delay(1000);        
}