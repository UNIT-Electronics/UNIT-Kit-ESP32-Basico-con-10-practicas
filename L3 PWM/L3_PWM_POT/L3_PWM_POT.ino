// Definir el pin donde está conectado el LED
#define ledPin  16 // Puedes cambiar el pin si lo necesitas
// Definir el pin donde esta conectado el potenciometro
#define potenciometro 4


void setup() {
  // Configurar el pin del LED como salida
  pinMode(ledPin, OUTPUT);
}

void loop() {
// Asignamos la conversion del adc del ESP32 a la variable pot
// Dicha conversion se obtiene de la lectura analogica del pin "potenciometro"
// Dividimos entre 16 para hacer un ajuste a la resolucion por defecto del pwm que es de 8 bits osea 255
// mientras que la resolucion por defecto del ADC del ESP32 es de 12 bits osea 4095
// por lo que 4096 / 16 = 255
  float pot = (analogRead(potenciometro) / 16);
// La intensidad luminosa del LED depende del potenciometro
    analogWrite(ledPin, pot); // Modificar la intensidad del LED
    delay(10); // Pausa para que el cambio sea gradual
}