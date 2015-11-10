#include <LiquidCrystal.h>
const int SensorAgua1 = 2;
const int SensorAgua2 = 3;
const int SensorAgua3 = 4;
const int SensorAgua4 = 5;
int leitura1 = 0;
int leitura2 = 0;
int leitura3 = 0;
int leitura4 = 0;

LiquidCrystal lcd(12,11,5,4,3,2);

void setup(){
  lcd.begin(16,2);
  lcd.setCursor(0,0);
  lcd.print("Ola Christian");
  delay(10000);
}

void loop(){
  limpaTela();
  leitura1 = analogRead(SensorAgua1);
  leitura2 = analogRead(SensorAgua2);
  leitura3 = analogRead(SensorAgua3);
  leitura4 = analogRead(SensorAgua4);
  
  lcd.setCursor(0,0);
  lcd.print(leitura1);
  lcd.setCursor(8,0);
  lcd.print(leitura2);
  lcd.setCursor(0,1);
  lcd.print(leitura3);
  lcd.setCursor(8,1);
  lcd.print(leitura4);
  delay(500);
}

void limpaTela(){
  lcd.setCursor(0,0);
  lcd.print("                ");
  lcd.setCursor(0,1);
  lcd.print("                ");
}
