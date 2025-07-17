#include <WiFi.h>

#define R 27
#define B 26
#define G 25
//Escribe el nombre de tu red y su respectiva contraseña
const char* ssid     = "Escribe el nombre de tu red";
const char* password = "Escribe la contraseña";
//Variables para configurar el PWM del ESP32
int frec = 5000, res = 8, cycle = 0, t=2;
//Configuramos el servidor web por el puerto 80
WiFiServer server(80);
// Variable para almacenar la petición HTTP
String header;


// Decode HTTP GET value
String valueString1 = String(5);
String valueString2 = String(5);
String valueString3 = String(5);
int pos1 = 0;
int pos2 = 0;


// Tiempo actual
unsigned long currentTime = millis();
// Tiempo pasado
unsigned long previousTime = 0;
// Define el timeout en millisegundos
const long timeoutTime = 2000;


void setup() {
//Iniciamos el monitor serial y configuramos los pines usados por el PWM
  Serial.begin(115200);
pinMode(R, OUTPUT);
ledcAttach(R, frec, res);
pinMode(G, OUTPUT);
ledcAttach(G, frec, res);
pinMode(B, OUTPUT);
ledcAttach(B, frec, res);


  // Nos conectamos al WiFi usando la contraseña y nombre de red
  Serial.print("Conectandose a ");
  Serial.print(ssid);
  WiFi.begin(ssid, password);
  //Mientras que no se conecte al WiFi aparecen puntos
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  // Se imprime la IP y se inicia el servidor
  Serial.println("\n");
  Serial.println("WiFi conectado.");
  Serial.print("IP: ");
  Serial.print(WiFi.localIP());
  server.begin();
  delay(3000); // Esperamos 3 segundos para que se realice correctamente la comunicación
}


void loop(){
  WiFiClient client = server.available();   // Esperamos clientes


  if (client) {     // Si un nuevo cliente se conecta
    currentTime = millis();                // se empieza a contar el tiempo
    previousTime = currentTime;  
      Serial.println("  \n ");        
    Serial.println("Nuevo Cliente.");          
    String currentLine = "";                // guardamos los datos del cliente en una cadena
    while (client.connected() && currentTime - previousTime <= timeoutTime) { // mientras el cliente esté conectado
      currentTime = millis();
      if (client.available()) {             // Si el cliente está disponible,
        char c = client.read();             // entonces lee un byte
        Serial.write(c);                    // imprime dicho byte
        header += c;
        if (c == '\n') {                    // si el byte es un caracter de nueva linea
          if (currentLine.length() == 0) {
            // Las cabeceras HTTP siempre inician con un código de respuesta (e.g. HTTP/1.1 200 OK)
            // y el tipo de contenido, así el cliente sabrá que recibir
            Serial.println("  \n ");
            client.println("HTTP/1.1 200 OK");
            client.println("Content-type:text/html");
            client.println("Connection: close");
            client.println();


            // Desplegamos la página web
          client.println("<!DOCTYPE html><html>");
            client.println("<head><meta name=\"viewport\" content=\"width=device-width, initial-scale=1\">");
            client.println("<link rel=\"icon\" href=\"data:,\">");
            // Añadimos estilo con CSS para el color, el tipo de letra etc
            client.println("<style>body { text-align: center; font-family: \"Trebuchet MS\", Arial; margin-left:auto; margin-right:auto;}");
            client.println(".slider { width: 300px; }</style>");
            client.println("<script src=\"https://ajax.googleapis.com/ajax/libs/jquery/3.3.1/jquery.min.js\"></script>");
                     
            // Página web
            // Título      
            client.println("</head><body><h1>LED RGB con ESP32</h1>");
            // Creamos tres sliders para cada color
            client.println("<p>Brillo Rojo: <span id=\"servoPos1\"></span></p>");
            client.println("<input type=\"range\" min=\"0\" max=\"255\" class=\"slider\" id=\"servoSlider1\" onchange=\"servo1(this.value)\" value=\""+valueString1+"\" />");


            client.println("<p>Brillo Verde: <span id=\"servoPos2\"></span></p>");
            client.println("<input type=\"range\" min=\"0\" max=\"255\" class=\"slider\" id=\"servoSlider2\" onchange=\"servo2(this.value)\" value=\""+valueString2+"\" />");


            client.println("<p>Brillo Azul: <span id=\"servoPos3\"></span></p>");
            client.println("<input type=\"range\" min=\"0\" max=\"255\" class=\"slider\" id=\"servoSlider3\" onchange=\"servo3(this.value)\" value=\""+valueString3+"\" />");
// Configuramos el comportamiento de cada slider
            client.println("<script>");
            client.println("    // Primer slider");
            client.println("    var slider1 = document.getElementById(\"servoSlider1\");");
            client.println("    var servoP1 = document.getElementById(\"servoPos1\");");
            client.println("    servoP1.innerHTML = slider1.value;");
            client.println("    slider1.oninput = function() {");
            client.println("        servoP1.innerHTML = this.value;");
            client.println("    };");
            client.println("    function servo1(pos) {");
            client.println("        $.get(\"/?value1=\" + pos);");
            client.println("    }");


            client.println("    // Segundo slider");
            client.println("    var slider2 = document.getElementById(\"servoSlider2\");");
            client.println("    var servoP2 = document.getElementById(\"servoPos2\");");
            client.println("    servoP2.innerHTML = slider2.value;");
            client.println("    slider2.oninput = function() {");
            client.println("        servoP2.innerHTML = this.value;");
            client.println("    };");
            client.println("    function servo2(pos) {");
            client.println("        $.get(\"/?value2=\" + pos);");
            client.println("    }");


            client.println("    // Tercer slider");
            client.println("    var slider3 = document.getElementById(\"servoSlider3\");");
            client.println("    var servoP3 = document.getElementById(\"servoPos3\");");
            client.println("    servoP3.innerHTML = slider3.value;");
            client.println("    slider3.oninput = function() {");
            client.println("        servoP3.innerHTML = this.value;");
            client.println("    };");
            client.println("    function servo3(pos) {");
            client.println("        $.get(\"/?value3=\" + pos);");
            client.println("    }");


            client.println("    $.ajaxSetup({ timeout: 10000 });");
            client.println("</script>");


            client.println("</body>");
            client.println("</html>");  
             
            //evaluamos el valor que arroja la URL de la página web
            //GET /?value=180& HTTP/1.1
            if(header.indexOf("GET /?value1=")>=0) {
              pos1 = header.indexOf('=');
              pos2 = header.indexOf('&');
              valueString1 = header.substring(pos1+1, pos2);
              //El pwm estara en funcion de la cadena que arroje la página web
              ledcWrite(R,valueString1.toInt());
              Serial.println("  \n\n ");
              Serial.print("ROJO: ");
              Serial.print(valueString1);
              Serial.println("  \n\n\n ");
            }    


               //GET /?value=180& HTTP/1.1
            if(header.indexOf("GET /?value2=")>=0) {
              pos1 = header.indexOf('=');
              pos2 = header.indexOf('&');
              valueString2 = header.substring(pos1+1, pos2);
             
              ledcWrite(B,valueString2.toInt());
               Serial.println("  \n\n ");
              Serial.print("VERDE: ");
               Serial.print(valueString2);
              Serial.println("  \n\n\n ");
            }


                         //GET /?value=180& HTTP/1.1
            if(header.indexOf("GET /?value3=")>=0) {
              pos1 = header.indexOf('=');
              pos2 = header.indexOf('&');
              valueString3 = header.substring(pos1+1, pos2);
             
              ledcWrite(G,valueString3.toInt());
               Serial.println(" \n\n ");
              Serial.print("AZUL: ");
              Serial.print(valueString3);
              Serial.println("  \n\n\n ");
            }


            // La respuesta HTTP termina con otro espacio en blanco
            client.println();
            // Rompemos el bucle While
            break;
          } else { // Si tienes una nueva línea rompe la línea actual
            currentLine = "";
          }
        } else if (c != '\r') {  
          currentLine += c;      
      }
     }
    }
    // Limpia la variable donde almacenamos las peticiones HTTP
    header = "";
    // Close the connection
    client.stop();
    Serial.println("Cliente desconectado.");
  }
}




