#include <WiFi.h>
#include <PubSubClient.h>

// Configuración WiFi
const char* ssid = "**********";
const char* password = "********";

// Configuración MQTT
const char* mqtt_server = "1.1.1.1";  
const int mqtt_port = 1883;
const char* mqtt_user = "usuario";          
const char* mqtt_pass = "contraseña";       

// Pines del sensor capacitivo
const int capPins[] = {32, 33, 34, 35, 36}; 
const int numPins = sizeof(capPins)/sizeof(capPins[0]);

// Umbrales de activación (ajustar según necesidad)
const int thresholds[] = {2000, 2000, 2000, 2000, 2000};

WiFiClient espClient;
PubSubClient client(espClient);

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

  Serial.println("");
  Serial.println("WiFi conectado");
  Serial.println("Dirección IP: ");
  Serial.println(WiFi.localIP());
}

void reconnect() {
  while (!client.connected()) {
    Serial.print("Intentando conexión MQTT...");
    
    if (client.connect("ESP32Capacitive", mqtt_user, mqtt_pass)) {
      Serial.println("conectado");
    } else {
      Serial.print("falló, rc=");
      Serial.print(client.state());
      Serial.println(" intentando de nuevo en 5 segundos");
      delay(5000);
    }
  }
}

void setup() {
  Serial.begin(115200);
  setup_wifi();
  client.setServer(mqtt_server, mqtt_port);
  
  // Configurar pines como entradas
  for(int i=0; i<numPins; i++) {
    pinMode(capPins[i], INPUT);
  }
}

void loop() {
  if (!client.connected()) {
    reconnect();
  }
  client.loop();

  for (int i = 0; i < numPins; i++) {
    int rawValue = touchRead(capPins[i]);  // Lectura capacitiva
    
    // Publicar valor crudo
    char rawTopic[30];
    snprintf(rawTopic, sizeof(rawTopic), "sensor/capacitivo/%d/raw", i+1);
    client.publish(rawTopic, String(rawValue).c_str());
    
    // Publicar estado binario (activado/desactivado)
    char stateTopic[30];
    snprintf(stateTopic, sizeof(stateTopic), "sensor/capacitivo/%d/state", i+1);
    client.publish(stateTopic, (rawValue < thresholds[i]) ? "ON" : "OFF");
    
    Serial.print("Sensor ");
    Serial.print(i+1);
    Serial.print(": ");
    Serial.print(rawValue);
    Serial.print(" - ");
    Serial.println((rawValue < thresholds[i]) ? "ACTIVADO" : "INACTIVO");
  }

  delay(100); // Pequeño delay para estabilidad
}