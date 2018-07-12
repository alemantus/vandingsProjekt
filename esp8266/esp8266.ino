/*
 Basic ESP8266 MQTT example

 This sketch demonstrates the capabilities of the pubsub library in combination
 with the ESP8266 board/library.

 It connects to an MQTT server then:
  - publishes "hello world" to the topic "outTopic" every two seconds
  - subscribes to the topic "inTopic", printing out any messages
    it receives. NB - it assumes the received payloads are strings not binary
  - If the first character of the topic "inTopic" is an 1, switch ON the ESP Led,
    else switch it off

 It will reconnect to the server if the connection is lost using a blocking
 reconnect function. See the 'mqtt_reconnect_nonblocking' example for how to
 achieve the same result without blocking the main loop.

 To install the ESP8266 board, (using Arduino 1.6.4+):
  - Add the following 3rd party board manager under "File -> Preferences -> Additional Boards Manager URLs":
       http://arduino.esp8266.com/stable/package_esp8266com_index.json
  - Open the "Tools -> Board -> Board Manager" and click install for the ESP8266"
  - Select your ESP8266 in "Tools -> Board"

*/

#include <ESP8266WiFi.h>
#include <PubSubClient.h>

// Update these with values suitable for your network.

const char* ssid = "alemantus";
const char* password = "Alex1240";
const char* mqtt_server = "10.0.0.26";

const char* serverUser = "admin";
const char* serverPassword = "Strandparken57";

const char* dataOut[2];

WiFiClient espClient;
PubSubClient client(espClient);
long lastMsg = 0;
char msg[50];
int value = 0;
int wakePin = 2;
String data[2];
int sensorNumber = 2;
int error = 0;

void setup() {
  pinMode(BUILTIN_LED, OUTPUT);     // Initialize the BUILTIN_LED pin as an output
  pinMode(wakePin, OUTPUT);
  Serial.begin(115200);

  digitalWrite(wakePin, HIGH);
  delay(500); //give the slave some time to wake up
  if(error < 5){
    if (Serial.available() > 0) {
      for(int i = 0; i < sensorNumber; i++){
        while(Serial.read()!='\n'){
          data[i]=Serial.read();
        }
      }
      digitalWrite(wakePin, LOW); 
    }else{
      delay(100);
      error++;
    }
  }else{
    Serial.println("Could wake slave");
  }
  data[0] = "1";
  data[1] = "2";
  for(int i = 0; i < sensorNumber; i++){ //convert data from int to const char*
    dataOut[i] = data[i].c_str();    
  }
  
  setup_wifi();
  client.setServer(mqtt_server, 1883);
  client.setCallback(callback);

  if (!client.connected()) {
    reconnect();
  }
  client.loop();
  for(int i = 0; i < sensorNumber; i++){
    snprintf (msg, 75, "data%ld", i);
    client.publish(msg, dataOut[i]);
  }
  snprintf (msg, 75, "hello world #%ld", value);
  Serial.println(msg);
  client.publish("outTopic", msg);
  ESP.deepSleep(600 * 1000000); 
}

void setup_wifi() {

  delay(10);
  // We start by connecting to a WiFi network
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);

  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}

void callback(char* topic, byte* payload, unsigned int length) {
  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("] ");
  for (int i = 0; i < length; i++) {
    Serial.print((char)payload[i]);
  }
  Serial.println();

  // Switch on the LED if an 1 was received as first character
  if ((char)payload[0] == '1') {
    digitalWrite(BUILTIN_LED, LOW);   // Turn the LED on (Note that LOW is the voltage level
    // but actually the LED is on; this is because
    // it is acive low on the ESP-01)
  } else {
    digitalWrite(BUILTIN_LED, HIGH);  // Turn the LED off by making the voltage HIGH
  }

}

void reconnect() {
  // Loop until we're reconnected
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    // Attempt to connect
    if (client.connect("ESP8266Client", serverUser, serverPassword)) {
      Serial.println("connected");
      // Once connected, publish an announcement...
      //client.publish("outTopic", "hello world");
      // ... and resubscribe
      client.subscribe("inTopic");
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}
void loop() {


}
