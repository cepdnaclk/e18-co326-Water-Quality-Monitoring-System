#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include <OneWire.h>
#include <DallasTemperature.h>

// Update these with values suitable for your network.
const char* ssid = "HUAWEI P30 LITE";
const char* password = "A2CD89C9";

//const char* ssid = "SLT-4G_154499";
//const char* password = "A2CD89C9New";

//const char* ssid = "Eng-Student";
//const char* password = "3nG5tuDt";

const char* mqtt_server = "test.mosquitto.org";  // test.mosquitto.org

WiFiClient espClient;
PubSubClient client(espClient);
unsigned long lastMsg = 0;
#define MSG_BUFFER_SIZE	(50)
char msgTemp[MSG_BUFFER_SIZE];
char msgTurbidity[MSG_BUFFER_SIZE];

// Data wire is connected to digital pin 4 on the Arduino
#define ONE_WIRE_BUS 4
 
// Setup a oneWire instance to communicate with any OneWire device
OneWire oneWire(ONE_WIRE_BUS);	
 
// Pass oneWire reference to DallasTemperature library
DallasTemperature sensors(&oneWire);
 
#define D7 13 
#define D8 15 

void setup_wifi() {
  delay(10);
  // We start by connecting to a WiFi network
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);

  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  randomSeed(micros());

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}


void controlEmergencyLED(byte* payload) {
  // Switch on the LED if a '1' was received as the first character
  if ((char)payload[0] == '1') {
    digitalWrite(BUILTIN_LED, LOW); // Turn the LED on
  } else {
    digitalWrite(BUILTIN_LED, HIGH); // Turn the LED off
  }
}

void controlGreenLED(byte* payload) {
  // Switch on the LED if a '1' was received as the first character
  if ((char)payload[0] == '1') {
    digitalWrite(D7, HIGH); // Turn the LED on
  } else {
    digitalWrite(D7, LOW); // Turn the LED off
  }
}

void controlRedLED(byte* payload) {
  // Switch on the LED if a '1' was received as the first character
  if ((char)payload[0] == '1') {
    digitalWrite(D8, HIGH); // Turn the LED on
  } else {
    digitalWrite(D8, LOW); // Turn the LED off
  }
}


void callback(char* topic, byte* payload, unsigned int length) {
  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("] ");
  
  // Print the received payload
  for (int i = 0; i < length; i++) {
    Serial.print((char)payload[i]);
  }
  Serial.println();

  // Check which LED to control based on the topic
  if (strcmp(topic, "device/UoP_CO_326_E18_3_Actuator_LEDEMERGENCY") == 0) {    
    controlEmergencyLED(payload);  
  
  } else if (strcmp(topic, "device/UoP_CO_326_E18_3_Actuator_LEDGREEN") == 0) {    
    controlGreenLED(payload);
  
  } else if (strcmp(topic, "device/UoP_CO_326_E18_3_Actuator_LEDRED") == 0) {    
    controlRedLED(payload);
  }  
}


void reconnect() {
  // Loop until we're reconnected
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    // Create a random client ID
    String clientId = "ESP8266Client-";
    clientId += String(random(0xffff), HEX);
    // Attempt to connect
    if (client.connect(clientId.c_str())) {
      Serial.println("connected");
      // Once connected, publish an announcement...
      client.publish("device/UoP_CO_326_E18_3_Sensor_Temperature", "MQTT Server is Connected");
      // ... and resubscribe
      client.subscribe("device/UoP_CO_326_E18_3_Actuator_LEDEMERGENCY");
      client.subscribe("device/UoP_CO_326_E18_3_Actuator_LEDGREEN");
      client.subscribe("device/UoP_CO_326_E18_3_Actuator_LEDRED");
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}



void setup() {
  sensors.begin();	// Start up the library
  pinMode(D7, OUTPUT); // set the digital pin as output
  pinMode(D8, OUTPUT); // set the digital pin as output 
  pinMode(BUILTIN_LED, OUTPUT);     // Initialize the BUILTIN_LED pin as an output
  Serial.begin(115200);
  setup_wifi();
  client.setServer(mqtt_server, 1883);
  client.setCallback(callback);
}

float readTemperature() {
  sensors.requestTemperatures();
  return sensors.getTempCByIndex(0);
}

int readTurbidity() {
  int turbidityValue = analogRead(A0);
  return map(turbidityValue, 0, 750, 100, 0);
}

void publishTemperature() {
  float temperature = readTemperature();
  snprintf (msgTemp, MSG_BUFFER_SIZE, "Temperature is %.2f °C", temperature);
  Serial.print("Publish message: ");
  Serial.println(msgTemp);
  client.publish("device/UoP_CO_326_E18_3_Sensor_Temperature", msgTemp);
}

void publishTurbidity() {
  int turbidity = readTurbidity();
  snprintf (msgTurbidity, MSG_BUFFER_SIZE, "Turbidity is %d", turbidity);
  Serial.print("Publish message: ");
  Serial.println(msgTurbidity);
  client.publish("device/UoP_CO_326_E18_3_Sensor_Turbidity", msgTurbidity);
}

void loop() {
  if (!client.connected()) {
    reconnect();
  }
  client.loop();

  unsigned long now = millis();
  if (now - lastMsg > 2000) {
    lastMsg = now;
    
    publishTemperature();
    publishTurbidity();
  }
}
