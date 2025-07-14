// Configuración de la red Wi-Fi
const char* ssid = "SSID";         // Nombre de la red WiFi
const char* password = "passwrod";         // Contraseña del WiFi

// Configuración del broker MQTT
const char* mqtt_server = "1.1.1.1";   // IP del broker MQTT (Raspberry Pi)
const int mqtt_port = 1883;                  // Puerto MQTT estándar
const char* mqtt_user = "user";             // Usuario MQTT
const char* mqtt_password = "password";          // Contraseña MQTT
const char* mqtt_topic = "esp32/anemometro";  // Topic para publicar datos

// Clientes WiFi y MQTT
WiFiClient espClient;                         // Cliente WiFi
PubSubClient client(espClient);               // Cliente MQTT

// Configuración del hardware
const int pinAnemometro = 34;                 // Pin GPIO34 (entrada analógica)

// Parámetros de calibración del sensor
const float voltajeMinimo = 0.05;             // Voltaje mínimo que genera el anemómetro
const float voltajeMaximo = 3.3;              // Voltaje máximo (3.3V es el máximo del ADC)
const float velocidadMaxima = 32.4;           // Velocidad máxima que puede medir (m/s)

// Factores de conversión
const float mps_a_kmh = 3.6;                  // Conversión m/s a km/h
const float mps_a_mph = 2.23694;              // Conversión m/s a mph

// Configuración de muestreo
const int numMuestras = 10;                   // Número de muestras para promedio
unsigned long lastMsgTime = 0;                // Último tiempo de publicación
const long intervaloPublicacion = 1000;       // Intervalo de publicación (1 segundo)

// Función para conectar a WiFi
void setup_wifi() {
  delay(10);
  Serial.println();
  Serial.print("Conectando a ");
  Serial.println(ssid);

  WiFi.begin(ssid, password);  // Inicia conexión WiFi

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("\nWiFi conectado");
  Serial.print("Dirección IP: ");
  Serial.println(WiFi.localIP());  // Muestra la IP asignada
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
  Serial.begin(115200);                      // Inicia comunicación serial
  setup_wifi();                              // Conecta a WiFi
  client.setServer(mqtt_server, mqtt_port);  // Configura servidor MQTT

  analogSetAttenuation(ADC_11db);            // Configura ADC para medir hasta 3.3V
}

// Bucle principal
void loop() {
  if (!client.connected()) {
    reconnect();  // Reconoecta si se perdió la conexión
  }
  client.loop();  // Mantiene la conexión MQTT activa

  unsigned long now = millis();
  if (now - lastMsgTime > intervaloPublicacion) {
    lastMsgTime = now;

    // Lectura del anemómetro (promedio de varias muestras)
    int sumaLecturas = 0;
    for (int i = 0; i < numMuestras; i++) {
      int lectura = analogRead(pinAnemometro);  // Lee valor analógico
      sumaLecturas += lectura;
      delay(5);  // Pequeña pausa entre lecturas
    }
    
    // Conversión a voltaje (ESP32 tiene ADC de 12 bits: 0-4095)
    float voltaje = (sumaLecturas / numMuestras) * (3.3 / 4095.0);
    voltaje = constrain(voltaje, voltajeMinimo, voltajeMaximo);  // Limita el rango

    // Cálculo de velocidad en m/s
    float velocidad_mps = ((voltaje - voltajeMinimo) / (voltajeMaximo - voltajeMinimo)) * velocidadMaxima;
    
    // Conversiones a otras unidades
    float velocidad_kmh = velocidad_mps * mps_a_kmh;
    float velocidad_mph = velocidad_mps * mps_a_mph;

    // Mostrar datos por serial
    Serial.print("Voltaje: ");
    Serial.print(voltaje, 2);
    Serial.print(" V | Velocidad: ");
    Serial.print(velocidad_mps, 2);
    Serial.print(" m/s | ");
    Serial.print(velocidad_kmh, 2);
    Serial.print(" km/h | ");
    Serial.print(velocidad_mph, 2);
    Serial.println(" mph");

    // Publicar datos por MQTT en formato JSON
    char mensaje[100];
    snprintf(mensaje, sizeof(mensaje),
             "{\"voltaje\":%.2f,\"mps\":%.2f,\"kmh\":%.2f,\"mph\":%.2f}",
             voltaje, velocidad_mps, velocidad_kmh, velocidad_mph);
    client.publish(mqtt_topic, mensaje);  // Publica en el topic MQTT
  }
}