#include <LiquidCrystal.h>
const int LM35 = 2;
float temperatura = 0;
int ADClido = 0;

LiquidCrystal lcd(12,11,5,4,3,2);

void setup(){
  lcd.begin(16,2);
  lcd.setCursor(0,0);
  lcd.print("Ola Christian W.");
  lcd.setCursor(0,1);
  lcd.print("Surkamp");
  delay(10000);
}

void loop(){
  ADClido = analogRead(LM35);
  temperatura = ADClido * 0.48828125;
  
  lcd.setCursor(0,0);
  lcd.print("Temperatura = ");
  lcd.setCursor(0,1);
  lcd.print(temperatura);
  lcd.print(" *C");
  delay(500);
}
