<div>
<a href="https://mqtt.org/use-cases/">
<img src="imagenes/mqtt.jpg" alt="Logo" width="80" height="80">
</div>

# Instalacion MQTT

## Actualizacion de Componentes
sudo apt update && sudo apt upgrade -y

## Instalacion Mosquito MQTT y Cliente
sudo apt install -y mosquitto mosquitto-clients

## Iniciar servicio escucha MQTT
sudo systemctl enable mosquitto

## Verificar que el servicio este activo
sudo systemctl status mosquitto

## Configuracion para escucha de clientes 
sudo nano /etc/mosquitto/mosquitto.conf
### Configuracion
- listener 1883
- allow_anonymous false
- password_file /etc/mosquitto/passwd
- - Configurar clave facil
### Conficuracion de Cliente 
sudo mosquitto_passwd -c /etc/mosquitto/passwd lfgelo

