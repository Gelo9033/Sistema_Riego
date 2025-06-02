<p align ="center"> 
    <img src="/imagenes/anemometro.png" alt="Logo" width="160" height="100">
  </a>
</p>


#include <WiFi.h>
#include <PubSubClient.h>

// Configuración de Wi-Fi
const char* ssid = "TU_SSID";           // Reemplaza con tu SSID
const char* password = "TU_CONTRASEÑA"; // Reemplaza con tu contraseña

// Configuración del broker MQTT
const char* mqtt_server = "192.168.10.92"; // Dirección IP de tu broker MQTT
const int mqtt_port = 1883;
const char* mqtt_user = "TU_USUARIO_MQTT";       // Reemplaza con tu usuario MQTT
const char* mqtt_password = "TU_CONTRASEÑA_MQTT"; // Reemplaza con tu contraseña MQTT

// Configuración del anemómetro
const int anemometerPin = 34; // GPIO al que está conectado el anemómetro
const float voltageMin = 0.0;   // Voltaje mínimo de salida del anemómetro (sin viento)
const float voltageMax = 3.3;   // Voltaje máximo de salida del anemómetro (velocidad máxima)
const float windSpeedMax = 32.4; // Velocidad máxima del anemómetro en m/s

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
  setup_wifi();
  client.setServer(mqtt_server, mqtt_port);
}

void loop() {
  if (!client.connected()) {
    reconnect();
  }
  client.loop();

  // Lectura del anemómetro
  int rawValue = analogRead(anemometerPin);
  float voltage = (rawValue / 4095.0) * 3.3; // Convertir valor crudo a voltaje
  float windSpeed = (voltage / voltageMax) * windSpeedMax; // Calcular velocidad del viento en m/s

  // Publicar velocidad del viento en MQTT
  char msg[8];
  dtostrf(windSpeed, 6, 2, msg);
  client.publish("invernadero/velocidad_viento", msg);

  delay(300000); // Espera 5 minutos (300,000 milisegundos) entre lecturas
}
