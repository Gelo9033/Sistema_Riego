<p align ="center"> 
    <img src="/imagenes/home_docker.png" alt="Logo" width="160" height="90">
  </a>
</p>
<h1>DOCKER -> HOME ASSITANT </h1>


## Descargar imagen Docker Home Assitant
- docker pull ghcr.io/home-assistant/home-assistant:stable
## Guardar imagen con un nombre facil
- docker save -o homeassistant.tar ghcr.io/home-assistant/home-assistant:stable
## Cargar mi imagen a local
- docker load -i homeassistant.tar
## Crear archivo .yml
version: '3.8'
services:
  homeassistant:
    container_name: homeassistant
    image: ghcr.io/home-assistant/home-assistant:stable
    volumes:
      - ./config:/config
      - /etc/localtime:/etc/localtime:ro
    restart: unless-stopped
    network_mode: host
    environment:
      - TZ=America/Guayaquil
## Levantar servicio de imagen 
- docker compose up -d
## La imagen se levanta en el puerto 8123
- http://localhost:8123

