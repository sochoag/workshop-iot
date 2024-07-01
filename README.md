# Bienvendios al workshop de despliegue para IoT

## 1. ¿Qué es una VM?
Una máquina virtual (VM) es como tener una computadora dentro de otra computadora. Funciona como una imitación de una computadora real, con su propio sistema operativo, programas y archivos, todo operando de manera independiente dentro de la computadora principal. Lo que sucede en la VM no afecta al sistema principal, proporcionando un entorno aislado y seguro. Esto permite ejecutar diferentes sistemas operativos y configuraciones en una sola máquina, lo cual es útil para probar software, ejecutar aplicaciones específicas y mejorar la seguridad.

## 2. ¿Qué es un contenedor de Docker?
Un contenedor de Docker es una tecnología que permite ejecutar aplicaciones de manera aislada y consistente en cualquier entorno. A diferencia de una máquina virtual, un contenedor no necesita un sistema operativo completo, sino que comparte el núcleo del sistema operativo del host, lo que lo hace más liviano y eficiente. Los contenedores de Docker incluyen todo lo necesario para que una aplicación funcione, como el código, las bibliotecas y las dependencias, asegurando que la aplicación se comporte de la misma manera sin importar dónde se ejecute

![image](https://github.com/sochoag/workshop-iot/assets/19669931/87bfed12-5bf2-4f58-ac20-dcd1640669c2)

## 3. Instalación de Docker
### Windows
- [Documentación de instalación](https://docs.docker.com/desktop/install/windows-install/)
- [Instalador](https://desktop.docker.com/win/main/amd64/Docker%20Desktop%20Installer.exe?utm_source=docker&utm_medium=webreferral&utm_campaign=docs-driven-download-win-amd64)
### Linux
#### Añadir repositorio de Docker al la lista de APT
```sh
# Add Docker's official GPG key:
sudo apt-get update
sudo apt-get install ca-certificates curl
sudo install -m 0755 -d /etc/apt/keyrings
sudo curl -fsSL https://download.docker.com/linux/ubuntu/gpg -o /etc/apt/keyrings/docker.asc
sudo chmod a+r /etc/apt/keyrings/docker.asc

# Add the repository to Apt sources:
echo \
  "deb [arch=$(dpkg --print-architecture) signed-by=/etc/apt/keyrings/docker.asc] https://download.docker.com/linux/ubuntu \
  $(. /etc/os-release && echo "$VERSION_CODENAME") stable" | \
  sudo tee /etc/apt/sources.list.d/docker.list > /dev/null
sudo apt-get update
```
#### Instalación de Docker
```sh
sudo apt-get install docker-ce docker-ce-cli containerd.io docker-buildx-plugin docker-compose-plugin
```

## 5. Desplegando nuestro primer servicio (NodeRed)
Windows
```sh
docker run -d -p 1880:1880 -v NodeREDdata:/data --name nodered nodered/node-red
```
Linux
```sh
sudo docker run -d -p 1880:1880 -v NodeREDdata:/data --name nodered nodered/node-red
```

## 6. Desplegando nuestro segundo servicio (EMQX)
```sh
docker run -d --name emqx -p 18083:18083 -p 1883:1883 -p 8083:8083 emqx:latest
```
Linux
```sh
sudo docker run -d --name emqx -p 18083:18083 -p 1883:1883 -p 8083:8083 emqx:latest
```

## 7. Desplegando nuestro tercer servicio (InfluxDB)
```sh
docker run -d -p 8086:8086 -v "$PWD/data:/var/lib/influxdb2" -v "$PWD/config:/etc/influxdb2" -e DOCKER_INFLUXDB_INIT_MODE=setup -e DOCKER_INFLUXDB_INIT_USERNAME=<USERNAME> -e DOCKER_INFLUXDB_INIT_PASSWORD=<PASSWORD> -e DOCKER_INFLUXDB_INIT_ORG=<ORG_NAME> -e DOCKER_INFLUXDB_INIT_BUCKET=<BUCKET_NAME> influxdb:2
```
Linux
```sh
sudo docker run -d -p 8086:8086 -v "$PWD/data:/var/lib/influxdb2" -v "$PWD/config:/etc/influxdb2" -e DOCKER_INFLUXDB_INIT_MODE=setup -e DOCKER_INFLUXDB_INIT_USERNAME=<USERNAME> -e DOCKER_INFLUXDB_INIT_PASSWORD=<PASSWORD> -e DOCKER_INFLUXDB_INIT_ORG=<ORG_NAME> -e DOCKER_INFLUXDB_INIT_BUCKET=<BUCKET_NAME> influxdb:2
```

## 8. Desplegando nuestro cuarto servicio (Grafana)

```sh
docker run -d --name=grafana -p 3000:3000 grafana/grafana
```
Linux
```sh
sudo docker run -d --name=grafana -p 3000:3000 grafana/grafana
```

## 8. Aplicación de ejemplo de uso

## 9. ¿Como replicar esto en Cloud?
