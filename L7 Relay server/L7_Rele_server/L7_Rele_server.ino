#include <WiFi.h>       
#define r1 2       // Definimos r1 como el pin D2 del ESP32    
#define r2 4       // Definimos r2 como el pin D4 del ESP32  

String p="off";         // dato de apagado del relevador1 (r1)
String a="off";         // dato de apagado del relevador2 (r2)

// Credenciales de wifi 
// Reemplazar con datos de tu red
const char *ssid = "Unit_WiFi";
const char *password = "UNIT3l3ctr0n1cs";

// Crea servidor llamado "server" y establece el puerto 80 para acceder
WiFiServer server(80);      

void setup() {
  //Configura D2 y D4 como salidaS e inician en estado alto (HIGH)
  pinMode(r1, OUTPUT);
  digitalWrite(r1, HIGH);

   pinMode(r2, OUTPUT);
  digitalWrite(r2, HIGH);

  //Inicia puerto serial a velocidad de 9600 baudios
  Serial.begin(9600);  
  Serial.println();
  Serial.println("Configurando");

  // Inicia conexión WiFi con las datos de la red especificados
  WiFi.begin(ssid, password);     //configurando las credenciales

  Serial.print("Conectandome");
  while (WiFi.status() != WL_CONNECTED)    // Espera conexión a la red
  {
    delay(500);
    Serial.print("."); // Imprime puntos hats que la ESP32 se conecte a tu red
  }

  Serial.println();
  Serial.print("Conectado, La dirección IP es: ");
  Serial.println(WiFi.localIP()); // Imprime la IP del ESP32 que ingresaremos en nuestro navegador web
 
  server.begin();                                   // Inicia el servidor
  Serial.println("Servidor iniciado");
}

void loop() {
  WiFiClient client = server.available();   // Recibe las conexciones de clientes

  if (client) {                             // Si hay un cliente
    Serial.println("Nuevo cliente.");       // Indica el acceso de un cliente
    String currentLine = "";                // Variable para datos de fin de linea
    while (client.connected()) {            // Cliente conectado
      if (client.available()) {             // Datos disponibles para ser leido
        char c = client.read();             // Lectura de byte en variable C
        Serial.write(c);                    // Muestra los datos
        
        if (c == '\n') {                    // Si el byte es un caracter de nuevo salto de linea

          if (currentLine.length() == 0) {    // Si no hay caracteres, entonces lanza el codgo HTML
            client.println("HTTP/1.1 200 OK");
            client.println("Content-type:text/html");
            client.println("Connection: close");
            client.println();
            
            client.println("<!DOCTYPE html><html>");
            client.println("<head><meta name=\"viewport\" content=\"width=device-width, initial-scale=1\">");
            client.println("<link rel=\"icon\" href=\"data:,\">");

            client.println("<style>html{font-family: Helvetica; display: inline-block; margin: 0px auto; text-align: center;}");
            client.println(".button {border: none;color: white;padding: 15px 32px; text-align: center;");
            client.println("text-decoration: none;display: inline-block;font-size: 16px; margin: 4px 2px;cursor: pointer;}");
            client.println(".button1 {background-color: #4CAF50;} /* Green */");
            client.println(".button2 {background-color: #808080;} /* Grey */");
            client.println("</style></head>");
            
            // Creación de botones 
            client.print("<body><h1>Leccion 7 Control de relevadores desde Servidor local con ESP32 ;)</h1>");

            if(p=="off"){
              client.println("<button type='button' class='button button1' onClick=location.href='/r1=ON'> ON </button><br><br>");                          
                }
            else{
              client.println("<button type='button' class='button button2' onClick=location.href='/r1=OFF'> OFF </button><br><br>");                                               
              }

              
            if(a=="off"){
              client.println("<button type='button' class='button button1' onClick=location.href='/r2=ON'> ON </button><br><br>");                          
                }
            else{
              client.println("<button type='button' class='button button2' onClick=location.href='/r2=OFF'> OFF </button><br><br>");                                               
              }
         
            client.print("</body></html>");
            client.println();
            
            // Rompe el codigo del while-loop
            break;
          } else {    // Limpiando variable
            currentLine = "";
          }
        } else if (c != '\r') {  // Si no hay retorno de carro
          currentLine += c;      // agrega al final de la linea
        }

        // Revisando el datos recibido del url
        if (currentLine.indexOf("GET /r1=ON") != -1) {
          digitalWrite(r1, LOW);               // GET /r1=ON
          p="on";
        }
        if (currentLine.indexOf("GET /r1=OFF") != -1) {
          digitalWrite(r1, HIGH);
                                             // GET /r1=OFF
          p="off";
        }

              if (currentLine.indexOf("GET /r2=ON") != -1) {
                                              // GET /r2=ON
           digitalWrite(r2, LOW);
          a="on";
        }
        if (currentLine.indexOf("GET /r2=OFF") != -1) {
          digitalWrite(r2, HIGH);               // GET /r2=OFF
          a="off";
        }
      }
    }
  }
}