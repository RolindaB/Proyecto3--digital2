#include <WiFi.h>
#include <WebServer.h>
#include <FS.h>
#include <SPIFFS.h>
#include <Adafruit_NeoPixel.h>
#include <Wire.h>
#include <Arduino.h>

#define PIN_NEOPIXELS 12  // Pin donde están conectados los NeoPixels
#define NUM_PIXELS 8     // Número de NeoPixels (uno para cada espacio de estacionamiento)
Adafruit_NeoPixel pixels(NUM_PIXELS, PIN_NEOPIXELS, NEO_GRB + NEO_KHZ800);

//I2C
#define I2C_SLAVE_ADDRESS 0x10  // Dirección del ESP32 como esclavo I2C

const char* ssid = "ESP32-Estacionamiento";  // Nombre de la red Wi-Fi creada por el ESP32
const char* password = "12345678";           // Contraseña de la red
// Variable global para contar los carros estacionados
int contadorCarros = 0;
WebServer server(80);  // Puerto del servidor web

// Estructura para definir las propiedades de cada imagen
struct Image {
  String rotation;   // Rotación de la imagen en grados (ej. "0deg", "90deg")
  String size;       // Tamaño de la imagen como porcentaje (ej. "110%", "100%")
  String fileName;   // Nombre del archivo de la imagen
};

// Matriz de 8 imágenes para los espacios de estacionamiento
Image parkingSpaces[8] = {
  {"0deg", "115%", "/car1.png"},  // Espacio 0: rotación 90 grados, tamaño 110%
  {"0deg", "100%", "/car2.png"},  // Espacio 1: rotación 90 grados, tamaño 100%
  {"0deg", "155%", "/car3.png"},  // Espacio 2: sin rotación, tamaño 80%
  {"0deg", "145%", "/car4.png"},  // Espacio 3: sin rotación, tamaño 90%
  {"0deg", "110%", "/car5.png"},  // Espacio 4: sin rotación, tamaño 100%
  {"270deg", "140%", "/car6.png"}, // Espacio 5: rotación 180 grados, tamaño 120%
  {"180deg", "120%", "/car7.png"},  // Espacio 6: rotación 90 grados, tamaño 100%
  {"180deg", "170%", "/car8.png"}  // Espacio 7: rotación 90 grados, tamaño 110%
};

// Estado de los espacios (1 = ocupado, 0 = libre)
int estadoEspacios[8] = { 0, 0, 0, 0, 0, 0, 0, 0 };

// Registro de carros estacionados durante el día
String carrosEstacionados = "";

// Funciones
void setup() {
  Serial.begin(115200);  // Configura UART a 115200 bps

  // Configurar ESP32 como punto de acceso
  WiFi.softAP(ssid, password);

  IPAddress IP = WiFi.softAPIP();  // Obtener la IP del punto de acceso
  Serial.print("Dirección IP del ESP32: ");
  Serial.println(IP);

  // Iniciar SPIFFS
  if (!SPIFFS.begin(true)) {
    Serial.println("Error al montar SPIFFS");
    return;
  }

  // Rutas para manejar solicitudes
  server.on("/", handleHomePage);
  server.on("/car", HTTP_GET, handleImageRequest);  
  server.begin();
  Serial.println("Servidor HTTP iniciado en modo punto de acceso");
  
  // Inicializar NeoPixels
  pixels.begin();
  actualizarNeoPixels();  // Establecer estado inicial de los NeoPixels

  // Configuración de I2C como esclavo
  Wire.begin(I2C_SLAVE_ADDRESS);
  Wire.onReceive(receiveI2CCommand);  // Llamar cuando reciba datos por I2C
}

void loop() {
  server.handleClient();
  handleSerialCommands();  // Revisa continuamente si hay comandos en la consola serial
}

// Maneja la solicitud de la página principal
void handleHomePage() {
  String html = SendHTML();
  server.send(200, "text/html", html);
}

// Genera el HTML para la página
String SendHTML() {
  String html = "<!DOCTYPE html><html lang='es'><head><meta charset='UTF-8'>"
                "<meta name='viewport' content='width=device-width, initial-scale=1.0'>"
                "<title>Estacionamiento Controlado</title>"
                "<link href='https://fonts.googleapis.com/css2?family=Langosta&display=swap' rel='stylesheet'>"
                "<style>:root { --bg-color: #2c3e50; --lot-bg-color: #34495e; --space-bg-color: #d6d8d870;"
                "--occupied-bg-color: #95a5a6; --highlight-color: #f1c40f; --text-color: #ffffff;"
                "--display-bg-color: #16a085; --space-width: 120px; --space-height: 180px; }"
                "body { font-family: 'Langosta', sans-serif; display: flex; align-items: center; "
                "justify-content: center; min-height: 100vh; margin: 0; background-color: var(--bg-color); }"
                ".container { text-align: center; padding: 20px; border-radius: 10px; "
                "box-shadow: 0 10px 30px rgba(0, 0, 0, 0.5); background-color: rgba(255, 255, 255, 0.1); }"
                "h1 { font-size: 40px; color: var(--text-color); margin-bottom: 20px; text-shadow: 2px 2px 5px rgba(0, 0, 0, 0.7); }"
                ".parking-lot { display: grid; grid-template-columns: repeat(4, var(--space-width)); gap: 20px; "
                "padding: 30px; background-color: var(--lot-bg-color); border-radius: 10px;} "
                ".space { position: relative; width: var(--space-width); height: var(--space-height);"
                "background-color: var(--space-bg-color); border: 5px solid var(--highlight-color); display: flex; "
                "align-items: center; justify-content: center; border-radius: 8px; overflow: hidden; }"
                ".space.occupied { background-color: var(--occupied-bg-color); }"
                ".display { font-size: 28px; color: var(--text-color); margin-top: 15px; font-weight: bold; "
                "background-color: var(--display-bg-color); padding: 10px; border-radius: 8px; display: inline-block;} "
                ".space-number { position: absolute; font-size: 20px; color: var(--text-color); font-weight: bold; }"
                "</style></head><body><div class='container'><h1>Estacionamiento Controlado</h1>"
                "<div id='display' class='display'>Espacios disponibles: " + String(contarLibres()) + "</div>"
                "<div class='parking-lot' id='parkingLot'>";

  // Generar los espacios de estacionamiento
  for (int i = 0; i < 8; i++) {
    html += "<div class='space";
    if (estadoEspacios[i] == 1) {
      html += " occupied'>";  // Si el espacio está ocupado
      // Cambiar tamaño y orientación según la matriz
      html += "<img src='/car?index=" + String(i) + "' alt='Ocupado' style='width:" + parkingSpaces[i].size + "; height:" + parkingSpaces[i].size + "; transform: rotate(" + parkingSpaces[i].rotation + "); object-fit:contain;'>"; 
    } else {
      html += "'>";
      // Si el espacio está libre, muestra el número del espacio
      html += "<span class='space-number'>" + String(i + 1) + "</span>";  // Mostrar el número del espacio (i + 1 para que empiece desde 1)
    }
    html += "</div>";
  }

  html += "</div><div class='container'>"
        "<div class='counter-card'>"
        "<h2>Carros estacionados durante el día</h2>"
        "<ul class='car-count-list'>";

  if (contadorCarros > 0) {
    String str = String(contadorCarros);
    html += "<li class='car-count-item'><span class='car-icon'>&#128663;</span> " + str + " carros estacionados</li>";
  } else {
    html += "<li class='car-count-item'><span class='car-icon'>&#128663;</span> No hay carros estacionados</li>";
  }
  
    html += "</ul></div></div><script>setInterval(() => { location.reload(); }, 5000);</script></body></html>";
  return html;
}

// Maneja la solicitud de la imagen
void handleImageRequest() {
  if (server.hasArg("index")) {
    int index = server.arg("index").toInt();  // Obtener el índice del parámetro 'index'
    
    // Verifica que el índice sea válido
    if (index >= 0 && index < 8) {
      String filename = parkingSpaces[index].fileName;  // Obtener el nombre del archivo de la imagen
      File file = SPIFFS.open(filename, "r");
      if (!file) {
        server.send(404, "text/plain", "Archivo no encontrado");
        return;
      }
      server.streamFile(file, "image/png");  // Enviar la imagen como respuesta
      file.close();
    } else {
      server.send(404, "text/plain", "Índice de imagen no válido");
    }
  } else {
    server.send(400, "text/plain", "Falta el parámetro 'index'");
  }
}

// Cuenta los espacios libres
int contarLibres() {
  int libres = 0;
  for (int i = 0; i < 8; i++) {
    if (estadoEspacios[i] == 0) libres++;
  }
  return libres;
}

void handleSerialCommands() {
  if (Serial.available() > 0) {
    String command = Serial.readString();
    command.trim();  // Limpiar cualquier espacio extra o salto de línea
    Serial.println("Comando recibido: " + command);

    // Comprobar si el comando es un número (1-8) para ocupar o letra (a-h) para desocupar
    if (command.length() == 1) {
      char commandChar = command.charAt(0);

      // Si el comando es un número (1-8), ocupar el espacio correspondiente
      if (commandChar >= '1' && commandChar <= '8') {
        int space = commandChar - '1';  // Convertir el número al índice (0-7)
        marcarEspacioOcupado(space);
        Serial.println("Parqueo " + String(space + 1) + " marcado como ocupado.");

      // Si el comando es una letra (a-h), desocupar el espacio correspondiente
      } else if (commandChar >= 'a' && commandChar <= 'h') {
        int space = commandChar - 'a';  // Convertir la letra al índice (0-7)
        marcarEspacioLibre(space);
        Serial.println("Parqueo " + String(space + 1) + " marcado como libre.");

      } else {
        Serial.println("Comando no válido. Usa números (1-8) para ocupar o letras (a-h) para desocupar.");
      }
    } else {
      Serial.println("Comando no válido. Asegúrate de ingresar un solo carácter (ejemplo: '1' o 'a').");
    }
  }
}
//-----------------------I2C----------------------------------------------------------------------------------
// Función que se llama al recibir un comando I2C
void receiveI2CCommand(int byteCount) {
  while (Wire.available()) {
    char commandChar1 = Wire.read();
     Serial.println(commandChar1);
    // Comprobar si el comando es para ocupar (1-8) o desocupar (a-h)
    if (commandChar1 >= '1' && commandChar1 <= '8') {
      int space = commandChar1 - '1';
      marcarEspacioOcupado(space);
      Serial.println("I2C - Parqueo " + String(space + 1) + " marcado como ocupado.");
      
    } else if (commandChar1 >= 'a' && commandChar1 <= 'h') {
      int space = commandChar1 - 'a';
      marcarEspacioLibre(space);
      Serial.println("I2C - Parqueo " + String(space + 1) + " marcado como libre.");
    } else {
      Serial.println("I2C - Comando no válido. Usa números (1-8) para ocupar o letras (a-h) para desocupar.");
    }
  }
}
//------------------------- Gestionamiento--------------------------------------------------------------
void marcarEspacioLibre(int index) {
  if (estadoEspacios[index] == 1) {  // Si está ocupado
    estadoEspacios[index] = 0;  // Marcar como libre
    actualizarNeoPixels();
  }
}

void marcarEspacioOcupado(int index) {
  if (estadoEspacios[index] == 0) {  // Si está libre
    estadoEspacios[index] = 1;  // Marcar como ocupado
    contadorCarros++;  // Aumentar el contador de carros
    actualizarNeoPixels();
  }
}
void actualizarNeoPixels() {
  for (int i = 0; i < NUM_PIXELS; i++) {
    if (estadoEspacios[i] == 1) {
      pixels.setPixelColor(i, pixels.Color(255, 0, 0));  // Rojo para ocupado
    } else {
      pixels.setPixelColor(i, pixels.Color(0, 255, 0));  // Verde para libre
    }
  }
  pixels.show();
}
