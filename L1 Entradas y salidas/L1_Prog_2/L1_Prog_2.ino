#define LED_PIN 2     // LED en pin D2 
#define BUTTON_PIN 4  // Boton en pin D4

bool ledState = false;  // Estado del LED
bool lastButtonState = HIGH;  // Estado anterior del botón

void setup() {
  pinMode(LED_PIN, OUTPUT);
  // Configura el boton como entrada y habilita una resistencia pull up interna
  pinMode(BUTTON_PIN, INPUT_PULLUP); 
}

void loop() {
  bool buttonState = digitalRead(BUTTON_PIN);
    delay(100); // Pequeño retardo para evitar flasos contactos del boton
  // Detecta cambio de estado (flanco de bajada)
  if (buttonState == LOW && lastButtonState == HIGH) {
    ledState = !ledState; // Cambia el estado del LED y se enclava
    digitalWrite(LED_PIN, ledState);
  }

  lastButtonState = buttonState; // Actualiza el estado anterior del botón
}
