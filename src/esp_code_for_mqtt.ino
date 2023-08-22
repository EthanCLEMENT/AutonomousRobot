#include <WiFi.h>
#include <PubSubClient.h>

const char* ssid = "HexIOT";
const char* password = "H3xag0nePriv4te";
const char* mqtt_server = "10.1.4.89";
const int mqtt_port = 1883;
const char* mqtt_topic = "CASH";

WiFiClient espClient;
PubSubClient client(espClient);

void setup_wifi() {
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
  }

  Serial.println(WiFi.localIP());
}

void reconnect() {
  while (!client.connected()) {

    if (client.connect("ESP32Client")) {
      client.subscribe(mqtt_topic);
    } else {
      Serial.print(client.state());
      delay(5000);
    }
  }
}

void setup() {
  Serial.begin(9600);
  setup_wifi();
  client.setServer(mqtt_server, mqtt_port);
}

void loop() {
  if (!client.connected()) {
    reconnect();
  }

  client.loop();

  // Check for serial data
  while (Serial.available()) {
    String message = Serial.readStringUntil('\n');

    if (message.startsWith("forw")) {
      client.publish(mqtt_topic, "forward");
    } else if (message.startsWith("lef")) {
      client.publish(mqtt_topic, "left");
    } else if (message.startsWith("bac")) {
      client.publish(mqtt_topic, "back");
    } else if (message.startsWith("rig")) {
      client.publish(mqtt_topic, "right");
    }
  }
}