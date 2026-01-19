# 🌐 LoRaWAN WSN con The Things Stack y Node-RED

Implementación de una red de sensores inalámbricos (WSN) basada en **LoRaWAN**, utilizando nodos **Heltec WiFi LoRa V3**, un **gateway Heltec HT-M7603**, la plataforma en la nube **The Things Stack (TTS)** y una integración adicional con **Node-RED** mediante **MQTT**.

Este repositorio forma parte de un taller académico orientado al estudio práctico de redes LPWAN y su integración con servicios en la nube.

---

## 📌 Arquitectura del sistema

La arquitectura implementada sigue el modelo clásico de LoRaWAN:

- **Nodos finales (End Devices):**
  - Dos nodos LoRaWAN que transmiten datos simulados:
    - Nodo 1: Temperatura
    - Nodo 2: Humedad
  - Activación mediante **OTAA**
  - Clase A
  - Región US915

- **Gateway LoRaWAN:**
  - Heltec HT-M7603
  - Configurado en modo **Packet Forwarder**
  - Conexión a Internet vía WiFi

- **Servidor de red:**
  - The Things Stack (Cloud)
  - Gestión de gateways, dispositivos y payloads

- **Integración adicional:**
  - Node-RED
  - Protocolo MQTT sobre TLS
  - Visualización en dashboard en tiempo real

---

## 📂 Contenido del repositorio

### 🔹 Código de los nodos LoRaWAN
Ubicado en la carpeta `nodes/`:

- `nodo1_temperatura.ino`  
  Código del nodo LoRaWAN que envía datos simulados de temperatura.

- `nodo2_humedad.ino`  
  Código del nodo LoRaWAN que envía datos simulados de humedad.

Ambos códigos utilizan la librería oficial de Heltec y están configurados para activación OTAA.

---

### 🔹 Flujo de Node-RED
Ubicado en la carpeta `node-red/`:

- `flows.json`  
  Flujo de Node-RED que:
  - Se suscribe al broker MQTT de The Things Stack
  - Procesa mensajes uplink
  - Separa datos por nodo
  - Visualiza temperatura y humedad en un dashboard

---

## 🔐 Requisitos

- Cuenta en **The Things Stack**
- Gateway LoRaWAN compatible
- Node-RED instalado (puerto 1880)
- Arduino IDE
- Librerías:
  - Heltec ESP32 LoRaWAN
  - Wire / SSD1306

---

## 🚀 Uso básico

1. Registrar gateway en The Things Stack
2. Crear aplicación LoRaWAN
3. Registrar nodos con OTAA
4. Cargar los archivos `.ino` en cada nodo
5. Configurar payload formatters en TTS
6. Importar `flows.json` en Node-RED
7. Visualizar datos en el dashboard

---

## 📊 Resultados

- Comunicación correcta nodo → gateway → TTS
- Decodificación exitosa de temperatura y humedad
- Recepción de datos en Node-RED vía MQTT
- Visualización en tiempo real mediante gauges

---

## 👥 Autores

- **Israel Delgado**
- **Anthony Dominguez**
- **Sebastián Guazhima**

---

## 📜 Licencia

Este proyecto se distribuye con fines académicos y educativos.

