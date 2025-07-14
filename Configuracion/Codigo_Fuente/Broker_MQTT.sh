# MIT License
# Copyright (c) [Año] [Tu Nombre]
# 
# Permiso otorgado para uso libre, incluyendo modificación y distribución,


# Script de instalación y configuración de Mosquitto MQTT Broker
# Licencia: MIT (Liberado para uso libre con atribución opcional)

#Dar permisos: chmod +x install_mosquitto.sh

#Ejecutar: sudo ./install_mosquitto.sh

## Actualización del sistema
echo "Actualizando componentes del sistema..."
sudo apt update && sudo apt upgrade -y

## Instalación de Mosquitto MQTT
echo "Instalando Mosquitto MQTT Broker y cliente..."
sudo apt install -y mosquitto mosquitto-clients

## Habilitar e iniciar el servicio
echo "Configurando servicio Mosquitto..."
sudo systemctl enable mosquitto
sudo systemctl start mosquitto

## Verificación del servicio
echo "Verificando estado del servicio..."
sudo systemctl status mosquitto --no-pager

## Configuración del broker
echo "Configurando archivo de configuración..."
sudo bash -c 'cat > /etc/mosquitto/mosquitto.conf << EOF
# Configuración básica MQTT Broker
listener 1883 
allow_anonymous false
password_file /etc/mosquitto/passwd
EOF'

## Configuración de usuario/clave
echo "Creando usuario MQTT..."
sudo mosquitto_passwd -c /etc/mosquitto/passwd user

## Reiniciar servicio para aplicar cambios
echo "Reiniciando servicio..."
sudo systemctl restart mosquitto

## Verificación final
echo "Instalación completada. Verifique la conexión con:"
echo "mosquitto_sub -h localhost -t \"#\" -u user -P \"password\" -v"