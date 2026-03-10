#include <Wire.h>
#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x27, 16, 2);

String cpu_temp = "--";
String gpu_temp = "--";
String ram_used = "--";

void actualizarLCD() {
  String linea1 = "C:" + cpu_temp + "C G:" + gpu_temp + "C";
  String linea2 = "RAM: " + ram_used + " GB";

  while (linea1.length() < 16) linea1 += " ";
  while (linea2.length() < 16) linea2 += " ";

  lcd.setCursor(0, 0);
  lcd.print(linea1.substring(0, 16));
  lcd.setCursor(0, 1);
  lcd.print(linea2.substring(0, 16));
}

void setup() {
  Serial.begin(115200);

  Wire.begin(21, 22);
  lcd.init();
  lcd.backlight();
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Esperando datos");
}

void loop() {
  if (Serial.available()) {
    String linea = Serial.readStringUntil('\n');
    linea.trim();

    // Formato esperado: "72.0|65.0|8.4"
    int i1 = linea.indexOf('|');
    int i2 = linea.lastIndexOf('|');

    if (i1 > 0 && i2 > i1) {
      cpu_temp = linea.substring(0, i1);
      gpu_temp = linea.substring(i1 + 1, i2);
      ram_used = linea.substring(i2 + 1);
      actualizarLCD();
    }
  }
}