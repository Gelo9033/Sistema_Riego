<p align ="center"> 
    <img src="/imagenes/postgres.png" alt="Logo" width="100" height="90">
  </a>
</p>
<h1>CONEXIÓN POSTGRES -> HOMEASSISTANT </h1> 

## CONFIGURACION YAML EN CONFIG/CONFIGURATION.YAML
recorder:
  db_url: postgresql://lfgelo:12345@localhost/flores_monteserin?client_encoding=utf8 
  //Conexion a BD con usuario y contraseña
  commit_interval: 30
  //Intervalo de escritura de lecturas 30s
  purge_keep_days: 14
  //Reset de Lecturas cada 14 dias
  include:
    domains:
      - sensor



default_config:


frontend:
themes: !include_dir_merge_named themes

automation: !include automations.yaml
script: !include scripts.yaml
scene: !include scenes.yaml

mqtt:
sensor:
- name: "Velocidad del viento"
    state_topic: "esp32/anemometro"
    unit_of_measurement: "km/h"
    value_template: "{{ value_json.kmh }}"
    icon: mdi:weather-windy

- name: "Viento (m/s)"
    state_topic: "esp32/anemometro"
    unit_of_measurement: "m/s"
    value_template: "{{ value_json.mps }}"
    icon: mdi:weather-windy-variant

- name: "Viento (mph)"
    state_topic: "esp32/anemometro"
    unit_of_measurement: "mph"
    value_template: "{{ value_json.mph }}"

- name: "Voltaje del sensor"
    state_topic: "esp32/anemometro"
    unit_of_measurement: "V"
    value_template: "{{ value_json.voltaje }}"