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

## Configuración del pin se selecciona el Pin 14 
const int pinAnemometro = 14; 

## Parámetros de calibración

const float voltajeMinimo = 0.05; 
## Maimo voltaje   

const float voltajeMaximo = 3.3;     
const float velocidadMaxima = 32.4;  

## Factores de conversión (velocidad de viento)
const float mps_a_kmh = 3.6;         
const float mps_a_mph = 2.23694;    

## Número de muestras para el promedio
const int numMuestras = 10;

void setup() {
  Serial.begin(115200);
  delay(1000); // Espera para estabilizar

## Mejora en la captura de Señal
  analogSetAttenuation(ADC_11db); // Permite medir hasta ~3.3V
}

void loop() {
  int sumaLecturas = 0;
  for (int i = 0; i < numMuestras; i++) {
    sumaLecturas += analogRead(pinAnemometro);
    delay(5); // Pequeña pausa entre lecturas
  }
  int valorADC = sumaLecturas / numMuestras;
## Convertir el valor ADC a voltaje
  float voltaje = (valorADC / 4095.0) * 3.3;

## Asegurar que el voltaje esté dentro del rango esperado
  voltaje = constrain(voltaje, 0.0, voltajeMaximo);

## Calcular la velocidad del viento en m/s
  float velocidad_mps = 0.0;
  if (voltaje >= voltajeMinimo) {
    velocidad_mps = ((voltaje - voltajeMinimo) / (voltajeMaximo - voltajeMinimo)) * velocidadMaxima;
  }

## Convertir a otras unidades
  float velocidad_kmh = velocidad_mps * mps_a_kmh;
  float velocidad_mph = velocidad_mps * mps_a_mph;

## Mostrar los resultados
  Serial.print("Voltaje: ");
  Serial.print(voltaje, 2);
  Serial.print(" V | Velocidad: ");
  Serial.print(velocidad_mps, 2);
  Serial.print(" m/s | ");
  Serial.print(velocidad_kmh, 2);
  Serial.print(" km/h | ");
  Serial.print(velocidad_mph, 2);
  Serial.println(" mph");

  delay(1000); // Esperar 1 segundo antes de la siguiente lectura
}