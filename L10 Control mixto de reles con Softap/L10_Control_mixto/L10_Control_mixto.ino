#include <WiFi.h>  
#include <WebServer.h>

#define r1 2  // Relé 1 en D2
#define r2 4  // Relé 2 en D4
#define btn1 22 // Botón físico para el Relé 1
#define btn2 23 // Botón físico para el Relé 2

bool estadoRele1 = true;
bool estadoRele2 = true;

const char* ssid = "Coloca aqui el nombre de tu red";  // Nombre de la red Wi-Fi (para SoftAP)
const char* password = "Establece una contraseña para tu red";  // Contraseña Wi-Fi

WebServer server(80);

void setup() {
  Serial.begin(115200);

  pinMode(r1, OUTPUT);
  pinMode(r2, OUTPUT);
  pinMode(btn1, INPUT_PULLUP); // Configuramos el botón con resistencia pullup
  pinMode(btn2, INPUT_PULLUP); // Configuramos el botón con resistencia pullup

  // Configurar el ESP32 en modo SoftAP
  WiFi.softAP(ssid, password);
  IPAddress ip = WiFi.softAPIP();

  Serial.println("Soft AP configurado.");
  Serial.print("Dirección IP del Soft AP: ");
  Serial.println(ip);

  // Definir rutas del servidor
  server.on("/", handleConnectionRoot);

  // Control del Relé 1
  server.on("/relayon1", handleReleOn1);
  server.on("/relayoff1", handleReleOff1);

  // Control del Relé 2
  server.on("/relayon2", handleReleOn2);
  server.on("/relayoff2", handleReleOff2);

  // Ruta para obtener el estado de los relés
  server.on("/estado_rele1", handleEstadoRele1);
  server.on("/estado_rele2", handleEstadoRele2);

  server.begin();
  Serial.println("Servidor HTTP iniciado");
}

void loop() {
  server.handleClient();  // Manejar peticiones de los clientes

  static unsigned long lastDebounceTime = 0;
  const unsigned long debounceDelay = 280;

  if (millis() - lastDebounceTime > debounceDelay) {
    if (digitalRead(btn1) == LOW) {
      estadoRele1 = !estadoRele1;
      digitalWrite(r1, estadoRele1 ? HIGH : LOW);
      lastDebounceTime = millis();
    }

    if (digitalRead(btn2) == LOW) {
      estadoRele2 = !estadoRele2;
      digitalWrite(r2, estadoRele2 ? HIGH : LOW);
      lastDebounceTime = millis();
    }
  }

  // Verificar si WiFi sigue activo
  if (WiFi.softAPgetStationNum() == 0) {  // Si no hay clientes conectados
    static unsigned long lastWiFiCheck = millis();
    if (millis() - lastWiFiCheck > 30000) {  // Cada 30s revisar si el AP sigue activo
      Serial.println("No hay clientes conectados. Reiniciando AP...");
      WiFi.softAP(ssid, password);
      lastWiFiCheck = millis();
    }
  }
}


// Página HTML con botones para controlar ambos relés
String respuesta = R"rawliteral(
<!DOCTYPE html>
<html lang="en">
<head>
<meta charset="UTF-8">
<meta name="viewport" content="width=device-width, initial-scale=1.0">
<title>Relay Control | AP</title>
<style>
body {
  margin: 0px auto;
  text-align: center;
  font-family: Helvetica;
}
#title { margin-top: 50px; margin-bottom: 15px; color: rgb(14, 14, 14); }
#subtitle { margin-top: 0; margin-bottom: 25px; color: dimgray; }
.onBtn, .offBtn {
  border-radius: 28px; display: inline-block; cursor: pointer;
  color: #ffffff; font-family: Arial; font-size: 17px; padding: 16px 35px;
  text-decoration: none; margin: 10px;
}
.onBtn { background-color: #44c767; border: 1px solid #18ab29; }
.onBtn:hover { background-color: #04e335; }
.onBtn:active { position: relative; top: 1px; }
.offBtn { background-color: #f05d67; border: 1px solid #e84165; }
.offBtn:hover { background-color: #f22954; }
.offBtn:active { position: relative; top: 1px; }
</style>
<script>
// Función para actualizar los botones según el estado del relé
function actualizarBotones() {
  // Botón Relé 1
  if (document.getElementById("estado_rele1").innerText == "Encendido") {
    document.getElementById("btn_on1").style.backgroundColor = "#44c767";
    document.getElementById("btn_off1").style.backgroundColor = "#e0e0e0";
  } else {
    document.getElementById("btn_on1").style.backgroundColor = "#e0e0e0";
    document.getElementById("btn_off1").style.backgroundColor = "#f05d67";
  }

  // Botón Relé 2
  if (document.getElementById("estado_rele2").innerText == "Encendido") {
    document.getElementById("btn_on2").style.backgroundColor = "#44c767";
    document.getElementById("btn_off2").style.backgroundColor = "#e0e0e0";
  } else {
    document.getElementById("btn_on2").style.backgroundColor = "#e0e0e0";
    document.getElementById("btn_off2").style.backgroundColor = "#f05d67";
  }
}

// Llamar a la función al cargar la página
window.onload = function() {
  actualizarBotones();
};

// Función para actualizar el estado de los relés desde el servidor
function actualizarEstado() {
  fetch("/estado_rele1")
    .then(response => response.text())
    .then(data => {
      document.getElementById("estado_rele1").innerText = data;
      actualizarBotones();
    });

  fetch("/estado_rele2")
    .then(response => response.text())
    .then(data => {
      document.getElementById("estado_rele2").innerText = data;
      actualizarBotones();
    });
}

// Llamar a la función para actualizar el estado cada segundo
setInterval(actualizarEstado, 1500); // Actualizar cada 1 segundo
</script>
</head>
<body>
<div>
  <h1 id="title">Control de reles con ESP32</h1>
  <h3 id="subtitle">Soft AP</h3>
</div>
<h2>Control Relé 1</h2>
<a href="/relayon1" id="btn_on1" class="onBtn" onclick="actualizarBotones()">ON</a>
<a href="/relayoff1" id="btn_off1" class="offBtn" onclick="actualizarBotones()">OFF</a>

<h2>Estado Relé 1: <span id="estado_rele1">%RELE1%</span></h2>

<h2>Control Relé 2</h2>
<a href="/relayon2" id="btn_on2" class="onBtn" onclick="actualizarBotones()">ON</a>
<a href="/relayoff2" id="btn_off2" class="offBtn" onclick="actualizarBotones()">OFF</a>

<h2>Estado Relé 2: <span id="estado_rele2">%RELE2%</span></h2>

</body>
</html>
)rawliteral";

// Actualizar la página web con el estado actual del relé
void handleConnectionRoot() {
  String pagina = respuesta;
  pagina.replace("%RELE1%", estadoRele1 ? "Encendido" : "Apagado");
  pagina.replace("%RELE2%", estadoRele2 ? "Encendido" : "Apagado");
  server.send(200, "text/html", pagina);
}

// Funciones para proporcionar el estado de los relés a través de una solicitud fetch
void handleEstadoRele1() {
  server.send(200, "text/plain", estadoRele1 ? "Encendido" : "Apagado");
}

void handleEstadoRele2() {
  server.send(200, "text/plain", estadoRele2 ? "Encendido" : "Apagado");
}

// Funciones para controlar el Relé 1 desde la página web
void handleReleOn1() {
  estadoRele1 = true;
  digitalWrite(r1, HIGH);  // Encender relé 1
  delay(100);  // Agregar un pequeño retraso para asegurar que la acción se complete
  server.sendHeader("Location", "/");  // Redirigir a la página principal
  server.send(303);  // Redirección HTTP 303
}

void handleReleOff1() {
  estadoRele1 = false;
  digitalWrite(r1, LOW);   // Apagar relé 1
  delay(100);  // Agregar un pequeño retraso para asegurar que la acción se complete
  server.sendHeader("Location", "/");  // Redirigir a la página principal
  server.send(303);  // Redirección HTTP 303
}

// Funciones para controlar el Relé 2 desde la página web
void handleReleOn2() {
  estadoRele2 = true;
  digitalWrite(r2, HIGH);  // Encender relé 2
  delay(100);  // Agregar un pequeño retraso para asegurar que la acción se complete
  server.sendHeader("Location", "/");  // Redirigir a la página principal
  server.send(303);  // Redirección HTTP 303
}

void handleReleOff2() {
  estadoRele2 = false;
  digitalWrite(r2, LOW);   // Apagar relé 2
  delay(100);  // Agregar un pequeño retraso para asegurar que la acción se complete
  server.sendHeader("Location", "/");  // Redirigir a la página principal
  server.send(303);  // Redirección HTTP 303
}