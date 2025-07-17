#include <LiquidCrystal_I2C.h>

// Definir el número de columnas y filas de la pantalla LCD
int lcdColumns = 20;
int lcdRows = 4;

// Definir la dirección I2C de la pantalla LCD, el número de columnas y filas
// Si no conoces la dirección de tu pantalla, puedes utilizar un programa escáner I2C
LiquidCrystal_I2C lcd(0x27, lcdColumns, lcdRows);

void setup(){
  // Inicializar la pantalla LCD
  lcd.init();
  // Activar la retroiluminación de la pantalla LCD
  lcd.backlight();
}

void loop(){
  // Establecer el cursor en la primera columna y fila
  lcd.setCursor(0, 0);
  // Escribir el mensaje en la pantalla
  lcd.print("Hola criaturitas del");
  lcd.setCursor(7, 1);
  lcd.print("bosque");
  // Esperar 1 segundo
  delay(1000);

  // Limpiar la pantalla
  lcd.clear();
  lcd.setCursor(8, 1);
  lcd.print(";)");
  // Esperar 1 segundo
  delay(1000);

  // Limpiar la pantalla
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("UNIT ELECTRONICS <3");
  // Esperar 1 segundo
  delay(1000);

  // Limpiar la pantalla
  lcd.clear();
}

