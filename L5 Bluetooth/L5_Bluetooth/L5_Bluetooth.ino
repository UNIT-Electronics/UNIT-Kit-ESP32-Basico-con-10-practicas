#include "BluetoothSerial.h"
#define led 2
BluetoothSerial SerialBT;

char receivedChar;  // Variable para almacenar el carácter recibido
int estado = 0;     // Variable para almacenar el estado, inicialmente 0

void setup() {
  Serial.begin(9600);    // Inicializa la comunicación serie a 9600 baudios para la depuración
  SerialBT.begin("Gerryno");  // Nombre del dispositivo Bluetooth
  pinMode(led, OUTPUT);  // Configura el pin 2 como salida (opcional, por si quieres usar un LED para ver el cambio de estado)
}

void loop() {
  // Verifica si se ha recibido un dato por el Bluetooth
  if (SerialBT.available()) {
    receivedChar = SerialBT.read();  // Lee el carácter recibido
    if (receivedChar == '1') {       // Comprueba si el carácter recibido es '1'
      estado = !estado;              // Cambia el estado (0 a 1 y 1 a 0)
      digitalWrite(led, estado);       // Opcional: enciende o apaga un LED en el pin 2
      SerialBT.println(estado);      // Envía el nuevo estado de vuelta por Bluetooth
      Serial.println(estado);        // Envía el nuevo estado al monitor serie para depuración
    }
  }
  delay(10);  // Añadir un pequeño retardo para evitar que el loop sea demasiado rápido
}
