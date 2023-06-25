#include <WiFi.h>
#include <PubSubClient.h>

const char* ssid = "SALA";
const char* password = "DIAS2020";
const char* mqttServer = "ec2kessiner.duckdns.org";
const int mqttPort = 1883;
const char* mqttUser = "kessiner";
const char* mqttPassword = "Kessiner10@";
const char* mqttTopicdioxido_de_enxofre = "esp32/dioxido_de_enxofre";
const char* mqttTopicozonio = "esp32/ozonio";
const int sensorPindioxido_de_enxofre = 33; // Potenciômetro de enxofre
const int sensorPinozonio = 32; // Potenciômetro de ozonio


WiFiClient espClient;
PubSubClient client(espClient);
void setup() {
  Serial.begin(115200);

  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println();
  Serial.print("Conectado ao WiFi com endereço IP: ");
  Serial.println(WiFi.localIP());

  client.setServer(mqttServer, mqttPort);
  while (!client.connected()) {
    Serial.println("Conectando ao servidor MQTT...");
    if (client.connect("ESP32Client", mqttUser, mqttPassword)) {
      Serial.println("Conectado ao servidor MQTT!");
    } else {
      Serial.print("Falha na conexão - Estado: ");
      Serial.print(client.state());
      delay(2000);
    }
  }
}

void loop() {
  int sensorValuedioxido_de_enxofre = analogRead(sensorPindioxido_de_enxofre);
  int sensorValueozonio = analogRead(sensorPinozonio);
float dioxido_de_enxofre = map(sensorValuedioxido_de_enxofre, 0, 4095, 0, 1000)/ 10.0;
float ozonio = map(sensorValueozonio, 0, 4095, 0, 1000)/ 10.0;
  String dioxido_de_enxofrePayload = String(dioxido_de_enxofre);
  String ozonioPayload = String(ozonio);
  Serial.print("Enviando dados para o Node-RED - dioxido_de_enxofre: ");
  Serial.println(dioxido_de_enxofre);
  client.publish(mqttTopicdioxido_de_enxofre, dioxido_de_enxofrePayload.c_str());
  Serial.print("Enviando dados para o Node-RED - ozonio: ");
  Serial.println(ozonio);
  client.publish(mqttTopicozonio, ozonioPayload.c_str());
  delay(1000);
}
