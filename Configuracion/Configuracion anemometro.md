<p align="center">
  <img src="/imagenes/anemometro.png" alt="Anemómetro" width="160" height="90">
</p>
 
<h1 align="center">Anemómetro con ESP32</h1>

Este proyecto implementa un anemómetro digital utilizando un ESP32 para medir la velocidad del viento con alta sensibilidad y precisión. El sistema convierte las lecturas analógicas de un sensor de viento en valores de velocidad en diferentes unidades.

<br>

Elementos utilizados:

- **Microcontrolador:** ESP32
- **Sensor de viento:** Motor de dos polos
- **Componentes adicionales:**
  - Diodo Zener
  - Resistencia de 1 kΩ
  - Condensador de 100 µF
  - Regulador de voltaje L7805CV
## Librerias 

- include <WiFi.h>
- include <PubSubClient.h>

## Configuración de la red Wi-Fi

const char* ssid = "FIBRAMAX_GUAMAN";
const char* password = "Lu1s331990@";

## Configuración del broker MQTT

const char* mqtt_server = "192.168.100.92"; // IP de tu Raspberry Pi
const int mqtt_port = 1883;
const char* mqtt_user = "lfgelo";
const char* mqtt_password = "12345";
const char* mqtt_topic = "esp32/anemometro";

## Cliente Wi-Fi y MQTT

WiFiClient espClient;
PubSubClient client(espClient);

## Configuración del pin se selecciona el Pin 14 

const int pinAnemometro = 34;

## Parámetros de calibración

const float voltajeMinimo = 0.05; 
const float voltajeMaximo = 3.3;      
const float velocidadMaxima = 32.4;

const float mps_a_kmh = 3.6;
const float mps_a_mph = 2.23694;


const int numMuestras = 10;

unsigned long lastMsgTime = 0;
const long intervaloPublicacion = 1000; // 1 segundo

## Conexión WiFi
void setup_wifi() {
  delay(10);
  Serial.println();
  Serial.print("Conectando a ");
  Serial.println(ssid);

  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("\nWiFi conectado");
  Serial.print("Dirección IP: ");
  Serial.println(WiFi.localIP());
}

## Conexión MQTT
void reconnect() {
  while (!client.connected()) {
    Serial.print("Intentando conexión MQTT...");
    if (client.connect("ESP32Client", mqtt_user, mqtt_password)) {
      Serial.println("Conectado al broker MQTT");
    } else {
      Serial.print("Fallo, rc=");
      Serial.print(client.state());
      Serial.println(" Intentando de nuevo en 5 segundos");
      delay(5000);
    }
  }
}

## Conexión Servidor MQTT

void setup() {
  Serial.begin(115200);
  setup_wifi();
  client.setServer(mqtt_server, mqtt_port);

  analogSetAttenuation(ADC_11db); // Para medir hasta 3.3V
}

void loop() {
  if (!client.connected()) {
    reconnect();
  }
  client.loop();

  unsigned long now = millis();
  if (now - lastMsgTime > intervaloPublicacion) {
    lastMsgTime = now;

   - **Lectura y procesamiento del anemómetro**
    int sumaLecturas = 0;
    for (int i = 0; i < numMuestras; i++) {
      int lectura = analogRead(pinAnemometro);
      sumaLecturas += lectura;
      delay(5);
    }
    
    voltaje = constrain(voltaje, voltajeMinimo, voltajeMaximo);

    
    float velocidad_mps = ((voltaje - voltajeMinimo) / (voltajeMaximo - voltajeMinimo)) * velocidadMaxima;

    
    float velocidad_kmh = velocidad_mps * mps_a_kmh;
    float velocidad_mph = velocidad_mps * mps_a_mph;

    
    Serial.print("Voltaje: ");
    Serial.print(voltaje, 2);
    Serial.print(" V | Velocidad: ");
    Serial.print(velocidad_mps, 2);
    Serial.print(" m/s | ");
    Serial.print(velocidad_kmh, 2);
    Serial.print(" km/h | ");
    Serial.print(velocidad_mph, 2);
    Serial.println(" mph");

   - **Formato JSON para MQTT**
    char mensaje[100];
    snprintf(mensaje, sizeof(mensaje),
             "{\"voltaje\":%.2f,\"mps\":%.2f,\"kmh\":%.2f,\"mph\":%.2f}",
             voltaje, velocidad_mps, velocidad_kmh, velocidad_mph);
    client.publish(mqtt_topic, mensaje);
  }
}