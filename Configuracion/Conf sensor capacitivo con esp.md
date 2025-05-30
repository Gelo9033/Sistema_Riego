<p align ="center"> 
    <img src="/imagenes/sensor_capacitivo.png" alt="Logo" width="100" height="90">
  </a>
</p>
<h1>SENSOR CAPCITIVO+ESP32(ARDUINO) -MQTT-> HOME ASSITANT </h1>

## CONFIGURACION

### Se incluyen librerias
#include <WiFi.h>
#include <PubSubClient.h>
### Configuración de Wi-Fi
const char* ssid = "**********";
const char* password = "********";
### Configuración del broker MQTT
const char* mqtt_server = "ip de donde se encuentra el broker"; 
const int mqtt_port = 1883;
### Pines analógicos a utilizar
const int analogPins[] = {32, 33, 34, 35, 36};
const int numPins = sizeof(analogPins) / sizeof(analogPins[0]);
WiFiClient espClient;
PubSubClient client(espClient);
void setup_wifi() {
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
  }
}
void reconnect() {
  while (!client.connected()) {
    if (client.connect("ESP32Client")) {
      // Conexión exitosa al broker MQTT
    } else {
      delay(5000); 
    }
  }
}

void setup() {
  setup_wifi();
  client.setServer(mqtt_server, mqtt_port);
}

void loop() {
  if (!client.connected()) {
    reconnect();
  }
  client.loop();

  for (int i = 0; i < numPins; i++) {
    int rawValue = analogRead(analogPins[i]);
    char topic[20];
    snprintf(topic, sizeof(topic), "espAnalogo%d", i + 1);
    char msg[10];
    snprintf(msg, sizeof(msg), "%d", rawValue);
    client.publish(topic, msg);
  }

  delay(1000); // Espera 1 segundo entre lecturas
}