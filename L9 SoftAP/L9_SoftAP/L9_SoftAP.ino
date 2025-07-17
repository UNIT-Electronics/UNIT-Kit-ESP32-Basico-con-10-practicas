#include <WiFi.h>
#include <WebServer.h>

#define r1 2  // Relé 1 en D2
#define r2 4  // Relé 2 en D4

bool estadoRele1 = false;
bool estadoRele2 = false;

const char* ssid = "Escribe el nombre de tu red";  // Nombre de la red Wi-Fi
const char* password = "Escribe la contraseña de tu red";   // Contraseña Wi-Fi

WebServer server(80);

void setup() {
  Serial.begin(115200);

  pinMode(r1, OUTPUT);
  digitalWrite(r1, LOW);  // Apagado por defecto

  pinMode(r2, OUTPUT);
  digitalWrite(r2, LOW);  // Apagado por defecto

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

  server.begin();
  Serial.println("Servidor HTTP iniciado");
  delay(1000);
}

void loop() {
  server.handleClient();

  // Control de los relés
  digitalWrite(r1, estadoRele1 ? LOW : HIGH);
  digitalWrite(r2, estadoRele2 ? LOW : HIGH);
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
</head>
<body>
<div>
  <h1 id="title">Control de reles con ESP32</h1>
  <h3 id="subtitle">Soft AP</h3>
</div>
<h2>Control Relé 1</h2>
<a href="/relayon1" class="onBtn">ON</a>
<a href="/relayoff1" class="offBtn">OFF</a>

<h2>Control Relé 2</h2>
<a href="/relayon2" class="onBtn">ON</a>
<a href="/relayoff2" class="offBtn">OFF</a>

</body>
</html>
)rawliteral";

void handleConnectionRoot() {
  server.send(200, "text/html", respuesta);
}

// Funciones para controlar Relé 1
void handleReleOn1() {
  estadoRele1 = true;
  server.send(200, "text/html", respuesta);
}

void handleReleOff1() {
  estadoRele1 = false;
  server.send(200, "text/html", respuesta);
}

// Funciones para controlar Relé 2
void handleReleOn2() {
  estadoRele2 = true;
  server.send(200, "text/html", respuesta);
}

void handleReleOff2() {
  estadoRele2 = false;
  server.send(200, "text/html", respuesta);
}


