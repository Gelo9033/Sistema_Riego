<h1>DOCKER COMOPOSE NODE-RED</h1>


version: '3.8'

services:
node-red:
image: nodered/node-red:latest
container_name: node-red
user: "1000:1000"
restart: unless-stopped
ports:
- "1880:1880"
volumes:
- ./data:/data
environment:
- TZ=America/Guayaquil