#include <WiFi.h>          // Librería para conexión WiFi
#include <PubSubClient.h>  // Librería para conexión MQTT
#include <DHT.h>           // Librería para sensores DHT

// Configuración de red WiFi (reemplazar con tus credenciales)
const char* ssid = "SSID";       // Nombre de tu red WiFi
const char* password = "password";   // Contraseña de tu WiFi

// Configuración del broker MQTT (reemplazar con tus datos)
const char* mqtt_server = "1.1.1.1";  // IP del servidor MQTT
const int mqtt_port = 1883;                 // Puerto MQTT estándar
const char* mqtt_user = "user";         // Usuario MQTT
const char* mqtt_password = "password";        // Contraseña MQTT

// Configuración de sensores DHT11
#define DHTPIN_IN 25    // Pin para sensor DHT interno
#define DHTPIN_OUT 26   // Pin para sensor DHT externo
#define DHTTYPE DHT11   // Tipo de sensor (DHT11)

// Inicialización de sensores
DHT dht_in(DHTPIN_IN, DHTTYPE);   // Sensor interno
DHT dht_out(DHTPIN_OUT, DHTTYPE); // Sensor externo

// Clientes para WiFi y MQTT
WiFiClient espClient;
PubSubClient client(espClient);

// Función para conectar a WiFi
void setup_wifi() {
  delay(10);
  Serial.begin(115200);
  WiFi.begin(ssid, password);
  
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  
  Serial.println("");
  Serial.println("WiFi conectado");
  Serial.println("Dirección IP: ");
  Serial.println(WiFi.localIP());
}

// Función para reconectar a MQTT
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

// Configuración inicial
void setup() {
  Serial.begin(115200);
  dht_in.begin();      // Inicia sensor interno
  dht_out.begin();     // Inicia sensor externo
  setup_wifi();        // Conecta a WiFi
  client.setServer(mqtt_server, mqtt_port);  // Configura MQTT
}

// Bucle principal
void loop() {
  if (!client.connected()) {
    reconnect();  // Reconoecta si es necesario
  }
  client.loop();  // Mantiene conexión MQTT

  // Lectura de sensores
  float temp_in = dht_in.readTemperature();    // Temperatura interna
  float hum_in = dht_in.readHumidity();       // Humedad interna
  float temp_out = dht_out.readTemperature(); // Temperatura externa
  float hum_out = dht_out.readHumidity();     // Humedad externa

  // Publicación de datos internos
  if (!isnan(temp_in)) {  // Verifica que la lectura sea válida
    char msg[8];
    dtostrf(temp_in, 6, 2, msg);  // Convierte float a string
    client.publish("invernadero/temperatura_interna", msg);
    Serial.print("Temp Interna: ");
    Serial.println(temp_in);
  }

  if (!isnan(hum_in)) {
    char msg[8];
    dtostrf(hum_in, 6, 2, msg);
    client.publish("invernadero/humedad_interna", msg);
    Serial.print("Hum Interna: ");
    Serial.println(hum_in);
  }

  // Publicación de datos externos
  if (!isnan(temp_out)) {
    char msg[8];
    dtostrf(temp_out, 6, 2, msg);
    client.publish("invernadero/temperatura_externa", msg);
    Serial.print("Temp Externa: ");
    Serial.println(temp_out);
  }

  if (!isnan(hum_out)) {
    char msg[8];
    dtostrf(hum_out, 6, 2, msg);
    client.publish("invernadero/humedad_externa", msg);
    Serial.print("Hum Externa: ");
    Serial.println(hum_out);
  }

  delay(300000);  // Espera 5 minutos (300,000 ms) entre lecturas
}