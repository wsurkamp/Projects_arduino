#include <LiquidCrystal.h>

int freq = 0;
const int Botao1 = 8;
const int Botao2 = 9;
const int Buzzer = 6;
int EstadoBotao1 = 0;
int EstadoBotao2 = 0;
LiquidCrystal lcd(12,11,5,4,3,2);


void setup(){
  lcd.begin(16,2);
  lcd.setCursor(0,0);
  lcd.print("Frequencia = ");
  lcd.setCursor(6,1);
  lcd.print("Hz");
  pinMode(Botao1, OUTPUT);
  pinMode(Botao2, OUTPUT);
  pinMode(Buzzer, OUTPUT);
}

void loop(){
  EstadoBotao1 = digitalRead(Botao1);
  EstadoBotao2 = digitalRead(Botao2);
  
  if (EstadoBotao1 == HIGH){
    freq = freq + 100;
  }
  
  if (EstadoBotao2 == HIGH){
    freq = freq - 100;
  }
  
  if (freq <= 0){
    freq = 0;
  }
  
  if (freq >= 20000){
    freq = 20000;
  }
  
  if (freq <= 99){
    lcd.setCursor(0,1);
    lcd.print(" ");
    lcd.setCursor(1,1);
    lcd.print(" ");
    lcd.setCursor(2,1);
    lcd.print(" ");
    lcd.setCursor(3,1);
  }
  
  if (freq >= 100){
    lcd.setCursor(0,1);
    lcd.print(" ");
    lcd.setCursor(1,1);
    lcd.print(" ");
    lcd.setCursor(2,1);
  }
  
  if (freq >= 1000){
    lcd.setCursor(0,1);
    lcd.print(" ");
    lcd.setCursor(1,1);
  }

  if(freq >= 10000){
    lcd.setCursor(0,1);
  }
  
  lcd.print(freq);
  tone(Buzzer, freq);
  
  delay(100);
}
