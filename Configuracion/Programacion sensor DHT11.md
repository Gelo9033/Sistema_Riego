
<p align ="center"> 
  <a href="https://mqtt.org/use-cases/">
    <img src="/imagenes/dht11.png" alt="Logo" width="160" height="100">
  </a>
</p>

## Incluye librerias
#include <WiFi.h>
#include <PubSubClient.h>
#include <DHT.h>

## Configuración de Wi-Fi
const char* ssid = "******";      
const char* password = "******"; 
## Configuración del broker MQTT
const char* mqtt_server = "192.168.10.92"; 
const int mqtt_port = 1883;
const char* mqtt_user = "********"; 
const char* mqtt_password = "*****";

## Pines de los sensores DHT11
#define DHTPIN_IN 25  
#define DHTPIN_OUT 26 
#define DHTTYPE DHT11
DHT dht_in(DHTPIN_IN, DHTTYPE);
DHT dht_out(DHTPIN_OUT, DHTTYPE);

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
    if (client.connect("ESP32Client", mqtt_user, mqtt_password)) {
      // Conexión exitosa al broker MQTT
    } else {
      delay(5000); // Espera 5 segundos antes de reintentar
    }
  }
}

void setup() {
  dht_in.begin();
  dht_out.begin();
  setup_wifi();
  client.setServer(mqtt_server, mqtt_port);
}

void loop() {
  if (!client.connected()) {
    reconnect();
  }
  client.loop();

  ## Lectura y publicación de sensores DHT11
  float temp_in = dht_in.readTemperature();
  float hum_in = dht_in.readHumidity();
  float temp_out = dht_out.readTemperature();
  float hum_out = dht_out.readHumidity();

  if (!isnan(temp_in)) {
    char msg[8];
    dtostrf(temp_in, 6, 2, msg);
    client.publish("invernadero/temperatura_interna", msg);
  }

  if (!isnan(hum_in)) {
    char msg[8];
    dtostrf(hum_in, 6, 2, msg);
    client.publish("invernadero/humedad_interna", msg);
  }

  if (!isnan(temp_out)) {
    char msg[8];
    dtostrf(temp_out, 6, 2, msg);
    client.publish("invernadero/temperatura_externa", msg);
  }

  if (!isnan(hum_out)) {
    char msg[8];
    dtostrf(hum_out, 6, 2, msg);
    client.publish("invernadero/humedad_externa", msg);
  }
## lecturas de 5 min
  delay(300000); 
}
