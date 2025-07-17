#define LED_PIN 2    // LED en pin D2 
#define BUTTON_PIN 4  // Boton en pin D4

void setup() {
  pinMode(LED_PIN, OUTPUT);      // Configura el pin del LED como salida
  pinMode(BUTTON_PIN, INPUT); // Configura el botón como entrada 
}

void loop() {
  int buttonState = digitalRead(BUTTON_PIN); // Lee el estado del botón

  if (buttonState == LOW) { // Si el botón se presiona (LOW debido a la resistencia pull-up externa)
    digitalWrite(LED_PIN, HIGH); // Enciende el LED
  } else {
    digitalWrite(LED_PIN, LOW); // Apaga el LED
  }
}
