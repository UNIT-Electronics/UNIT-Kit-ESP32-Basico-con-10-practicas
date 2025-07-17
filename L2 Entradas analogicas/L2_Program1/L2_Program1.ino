#define POT_PIN 34      // Pin analógico donde está conectado el potenciómetro

void setup() {
  Serial.begin(9600);  // Iniciamos el monitor serial a 9600 baudios
  
}

void loop() {
  int valorAnalogico = analogRead(POT_PIN);  // Leer el valor del potenciómetro (0-4095)
  float voltaje = (valorAnalogico / 4095.0) * 3.3;  // Convertir a voltaje (ESP32 usa 3.3V)

  Serial.print("Voltaje: ");
  Serial.println(voltaje);  // Imprime el voltaje en el monitor serie
  delay(500);  // Pequeña pausa para estabilidad
}