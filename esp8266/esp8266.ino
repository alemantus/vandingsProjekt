#include <ESP8266WiFi.h>
#include <PubSubClient.h>



#define mqtt_server "87.63.168.126"
#define mqtt_user "admin"
#define mqtt_password "Strandparken57"
#define numberSensor 2

const char* ssid = "Kvinderegensen";
const char* password = "qralumne";

String data[2];
const char* dataOut[2]; 
int error = 0;
int wakePin = 2;
const int sleepTimeS = 20;

WiFiClient espClient;
PubSubClient client(espClient);


void setup() {
  
  Serial.begin(115200);
  pinMode(wakePin, OUTPUT);


  digitalWrite(wakePin, HIGH);

  
/*  
  while(Serial.read()!="hi"){
    error++
    delay(200);
    if (error > 5){
      client.publish("Error","Could not wake slave");
      break;
    }
  }
*/
  delay(50); // buffer between setup and the main program.

  for(int i = 0; i < numberSensor; i++){
    while(Serial.read()!='\n'){
      data[i] = Serial.read();
    }
  }
  digitalWrite(wakePin, LOW);
  delay(100); 

  //data[0] = 25; //debug
  //data[1] = 22; //debug
  //connect to wifi and mqtt server
  setup_wifi();
  client.setServer(mqtt_server, 1883); 
  if (!client.connected()) {
    reconnect();
  }
  
  client.publish("test", "succes");

  //convert data[i] from int to cons char*
  for(int i = 0; i < numberSensor; i++){
    dataOut[i] = data[i].c_str();    
  }
  client.publish("data1", dataOut[0]);
  client.publish("data2", dataOut[1]);
  //client.loop();
 

  ESP.deepSleep(sleepTimeS * 1000000); 
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


void reconnect() {
  // Loop until we're reconnected
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    // Attempt to connect
    // If you do not want to use a username and password, change next line to
    // if (client.connect("ESP8266Client")) {
    if (client.connect("ESP8266Client", mqtt_user, mqtt_password)) {
      Serial.println("connected");
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
