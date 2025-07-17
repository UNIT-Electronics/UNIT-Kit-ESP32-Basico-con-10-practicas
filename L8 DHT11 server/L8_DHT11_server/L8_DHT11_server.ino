#include <WiFi.h>
#include <WiFiClient.h>
#include <WebServer.h>
#include <ESPmDNS.h>
#include <DHT.h>

// Credenciales de la red WiFi
const char *ssid = "Escribe aqui el nombre de tu red";
const char *password = "Escribe la contraseña de dicha red";

// Crear servidor web en el puerto 80
WebServer server(80);

// Inicializar el sensor DHT11 en el pin 2
DHT dht(2, DHT11);

// Función para manejar la página principal
void handleRoot() {
  char msg[1500];

  // Crear una página HTML con los datos de temperatura y humedad
  snprintf(msg, 1500,
           "<html>\
  <head>\
    <meta http-equiv='refresh' content='4'/>\
    <meta name='viewport' content='width=device-width, initial-scale=1'>\
    <link rel='stylesheet' href='https://use.fontawesome.com/releases/v5.7.2/css/all.css' integrity='sha384-fnmOCqbTlWIlj8LyTjo7mOUStjsKC4pOpQbqyi7RrhN7udi9RwhKkMHpvLbHG9Sr' crossorigin='anonymous'>\
    <title>Estacion de clima</title>\
    <style>\
    html { font-family: Arial; display: inline-block; margin: 0px auto; text-align: center;}\
    h2 { font-size: 3.0rem; }\
    p { font-size: 3.0rem; }\
    .units { font-size: 1.2rem; }\
    .dht-labels{ font-size: 1.5rem; vertical-align:middle; padding-bottom: 15px;}\
    </style>\
  </head>\
  <body>\
      <h2>Estacion climatica</h2>\
      <p>\
        <i class='fas fa-thermometer-half' style='color:#ca3517;'></i>\
        <span class='dht-labels'>Temperatura</span>\
        <span>%.2f</span>\
        <sup class='units'>&deg;C</sup>\
      </p>\
      <p>\
        <i class='fas fa-tint' style='color:#00add6;'></i>\
        <span class='dht-labels'>Humedad</span>\
        <span>%.2f</span>\
        <sup class='units'>&percnt;</sup>\
      </p>\
  </body>\
</html>",
           readDHTTemperature(), readDHTHumidity()  // Insertar valores de temperatura y humedad
          );
  server.send(200, "text/html", msg);  // Enviar la página HTML
}

void setup(void) {

  // Inicializar la comunicación serial
  Serial.begin(115200);
  dht.begin();  // Inicializar el sensor DHT
  
  WiFi.mode(WIFI_STA);  // Configurar el ESP32 como estación (cliente WiFi)
  WiFi.begin(ssid, password);  // Conectar al WiFi
  Serial.println("");

  // Esperar a que el ESP32 se conecte a la red WiFi
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");  // Mostrar el progreso de la conexión
  }

  Serial.println("");
  Serial.print("Conectado a ");
  Serial.println(ssid);  // Imprimir el nombre de la red WiFi
  Serial.print("IP: ");
  Serial.println(WiFi.localIP());  // Imprimir la dirección IP asignada

  // Iniciar mDNS para que el ESP32 sea accesible por nombre en la red local
  if (MDNS.begin("esp32")) {
    Serial.println("Se inició el respondedor MDNS");
  }

  // Asignar la función handleRoot a la raíz del servidor
  server.on("/", handleRoot);

  server.begin();  // Iniciar el servidor web
  Serial.println("Servidor HTTP iniciado correctamente");
}

void loop(void) {
  server.handleClient();  // Manejar las solicitudes del cliente (navegador)
  delay(2);  // Permitir que el CPU se cambie a otras tareas
}

// Función para leer la temperatura del sensor DHT11
float readDHTTemperature() {
  float t = dht.readTemperature();  // Leer la temperatura en grados Celsius
  if (isnan(t)) {    
    Serial.println("Fallo en la lectura del sensor DHT11");  // Error si la lectura es incorrecta
    return -1;
  }
  else {
    Serial.println(t);  // Imprimir la temperatura leída
    return t;
  }
}

// Función para leer la humedad del sensor DHT11
float readDHTHumidity() {
  float h = dht.readHumidity();  // Leer la humedad
  if (isnan(h)) {
    Serial.println("Fallo en la lectura del sensor DHT11");  // Error si la lectura es incorrecta
    return -1;
  }
  else {
    Serial.println(h);  // Imprimir la humedad leída
    return h;
  }
}

