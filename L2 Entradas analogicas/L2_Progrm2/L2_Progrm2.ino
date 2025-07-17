#define POT_PIN 34      // Pin analógico donde está conectado el potenciómetro
#define LED1_PIN 25     // Pin del primer LED en D25
#define LED2_PIN 26     // Pin del segundo LED en D26
#define BUZZER_PIN 27   // Pin del buzzer en D27

void setup() {
  Serial.begin(9600); // Iniciamos el monitor serial a 9600 baudios
  
  pinMode(LED1_PIN, OUTPUT);  // Declaramos entradas y salidas digitales
  pinMode(LED2_PIN, OUTPUT);
  pinMode(BUZZER_PIN, OUTPUT);
  // Las entradas analogicas no necesitan declaracion
}

void loop() {
  int valorAnalogico = analogRead(POT_PIN);  // Leer el valor del potenciómetro (0-4095)
  float voltaje = (valorAnalogico / 4095.0) * 3.3;  // Convertir a voltaje (ESP32 usa 3.3V)

  Serial.print("Voltaje: ");
  Serial.println(voltaje);  // Imprime el voltaje en el monitor serie

  // Control del LED 1 (enciende si el voltaje esta entre 0 y 1 volt)
  if (voltaje >= 0.0 && voltaje < 1.0) {
    digitalWrite(LED1_PIN, HIGH);
    digitalWrite(LED2_PIN, LOW);
     digitalWrite(BUZZER_PIN, LOW);
  } 
  // Control del LED 2 (enciende si el voltaje esta entre 1 y 2 volts)
  else if (voltaje >= 1.0 && voltaje <2.0) {
    digitalWrite(LED1_PIN, LOW);
    digitalWrite(LED2_PIN, HIGH);
    digitalWrite(BUZZER_PIN, LOW);
  } 
  // Activar el buzzer si el voltaje alcanza el máximo rango 
  else if (voltaje >= 2.0 && voltaje < 3.3) {  
    digitalWrite(BUZZER_PIN, HIGH);
    delay(50);
    digitalWrite(BUZZER_PIN, LOW);
    digitalWrite(LED1_PIN, LOW);
    digitalWrite(LED2_PIN, LOW);
  } 

  delay(500);  // Pequeña pausa para estabilidad
}

