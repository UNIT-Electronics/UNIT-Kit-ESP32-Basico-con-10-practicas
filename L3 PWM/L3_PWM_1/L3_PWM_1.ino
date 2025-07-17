// Definir el pin donde está conectado el LED
#define ledPin  16 // Puedes cambiar el pin si lo necesitas

// Definir el rango de PWM
int valorPWM = 0;      // Valor inicial del PWM (0 = apagado, 255 = intensidad máxima)

void setup() {
  // Configurar el pin del LED como salida
  pinMode(ledPin, OUTPUT);
}

void loop() {
  // Aumentar la intensidad luminosa
  for (valorPWM = 0; valorPWM <= 255; valorPWM ++) {
    analogWrite(ledPin, valorPWM); // Modificar la intensidad del LED
    delay(10); // Pausa para que el cambio sea gradual
  }

  // Disminuir la intensidad luminosa
  for (valorPWM = 255; valorPWM >= 0; valorPWM --) {
    analogWrite(ledPin, valorPWM); // Modificar la intensidad del LED
    delay(10); // Pausa para que el cambio sea gradual
  }
}

