#include <ESP8266WiFi.h>
#include <PubSubClient.h>

const char* ssid = "YOUR_WIFI";
const char* password = "YOUR_PASS";
const char* mqtt_server = "192.168.1.10";

WiFiClient espClient;
PubSubClient client(espClient);

// Motor pins
int motorFL = D1;
int motorFR = D2;
int motorBL = D3;
int motorBR = D4;

void callback(char* topic, byte* payload, unsigned int length) {
  String msg = "";

  for (int i = 0; i < length; i++) {
    msg += (char)payload[i];
  }

  Serial.println(msg);

  if (msg == "UP") {
    analogWrite(motorFL, 800);
    analogWrite(motorFR, 800);
    analogWrite(motorBL, 800);
    analogWrite(motorBR, 800);
  }

  else if (msg == "STOP") {
    analogWrite(motorFL, 0);
    analogWrite(motorFR, 0);
    analogWrite(motorBL, 0);
    analogWrite(motorBR, 0);
  }
}

void reconnect() {
  while (!client.connected()) {
    client.connect("DroneESP");
    client.subscribe("drone/control");
  }
}

void setup() {
  Serial.begin(115200);

  pinMode(motorFL, OUTPUT);
  pinMode(motorFR, OUTPUT);
  pinMode(motorBL, OUTPUT);
  pinMode(motorBR, OUTPUT);

  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
  }

  client.setServer(mqtt_server, 1883);
  client.setCallback(callback);
}

void loop() {
  if (!client.connected()) {
    reconnect();
  }

  client.loop();
}
