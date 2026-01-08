#include <WiFi.h>
#include <WiFiManager.h>

#include <PubSubClient.h>

// broker settings
const char* mqtt_server = "192.168.1.149";
const int   mqtt_port   = 1883;
const char* pub_topic   = "esp32c6/test_data";
const char* sub_topic   = "esp32c6/echo";

WiFiClient espClient;
PubSubClient client(espClient);

void mqttCallback(char* topic, byte* payload, unsigned int length) {
  Serial.print("message received [");
  Serial.print(topic);
  Serial.print("]: ");

  for (unsigned int i = 0; i < length; i++) {
    Serial.print((char)payload[i]);
  }
  Serial.println();
}

void reconnectMQTT() {
  while (!client.connected()) {
    Serial.print("connecting to mqtt...");

    if (client.connect("ESP32C6_Client")) {
      Serial.println("connected");

      client.subscribe(sub_topic);
      client.publish(pub_topic, "hello from esp32c6!");
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println("retrying in 2 seconds");
      delay(2000);
    }
  }
}

// connect to WiFi using captive portal
void setupWiFi() {
  Serial.print("connecting to wifi...");
  WiFiManager wm;

  if (!wm.autoConnect("ESP32-Setup")) {
    Serial.println("failed to connect (timeout)");
    ESP.restart();
  }

  Serial.print("wifi connected");
  Serial.println();
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
}

void eraseWiFiCreds() {
  Serial.println("erasing wifi creds...");

  WiFi.disconnect(true, true);
  WiFi.persistent(false);

  delay(1000);
  ESP.restart();
}

void eraseWiFiManagerCreds() {
  WiFiManager wm;

  Serial.println("resetting wifimanager creds...");
  wm.resetSettings();
  ESP.restart();
}

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  delay(1000);

  // uncomment one by one to erase credentials
  //eraseWiFiCreds();
  //eraseWiFiManagerCreds();

  setupWiFi();
  
  client.setServer(mqtt_server, mqtt_port);
  client.setCallback(mqttCallback);

  randomSeed(esp_random()); // for testing only
}

void loop() {
  // put your main code here, to run repeatedly:
  if (!client.connected()) reconnectMQTT();
  client.loop();

  // publish a message periodically
  static unsigned long lastMsg = 0;
  if (millis() - lastMsg > 5000) {
    lastMsg = millis();
    
    // generate random dummy data for testing
    //Serial.println("generating data");
    int data = random(0, 1000);
    char payload[16];
    snprintf(payload, sizeof(payload), "{\"data\":%d}", data);
    Serial.print("sending: ");
    Serial.println(payload);

    client.publish(pub_topic, payload);
  }
}
