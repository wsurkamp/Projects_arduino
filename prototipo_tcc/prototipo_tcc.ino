#include <LiquidCrystal.h>
const int SensorAgua1 = 12;
const int SensorAgua2 = 13;
const int SensorAgua3 = 14;
const int SensorAgua4 = 15;
const int pump1 = 20;
const int pump2 = 21;
const int valve1 = 14;
const int valve2 = 15;
int leituraSensor1 = 0;
int leituraSensor2 = 0;
int leituraSensor3 = 0;
int leituraSensor4 = 0;
int statusValve1 = 0;
int statusValve2 = 0;
int statusPump1 = 0;
int statusPump2 = 0;
int nivelCisterna = 0;
int nivelFuncionamentoBombas = 20;
int nivelAberturaValvulas = 40;
int nivelFechamentoValvulas = 60;

LiquidCrystal lcd(13,12,11,10,9,8);

void setup(){
  lcd.begin(16,2);
  lcd.setCursor(0,0);
  lcd.print("Gerenciamento de");
  lcd.setCursor(0,1);
  lcd.print("Cisternas - TCC");
  delay(10000);
  pinMode(pump1, OUTPUT);
  pinMode(pump2, OUTPUT);
  pinMode(valve1, OUTPUT);
  pinMode(valve2, OUTPUT);
}

void loop(){
  leituraSensor1 = analogRead(SensorAgua1);
  leituraSensor2 = analogRead(SensorAgua2);
  leituraSensor3 = analogRead(SensorAgua3);
  leituraSensor4 = analogRead(SensorAgua4);
  
  calculaNivel();
  
  escreveTela();
  
  verificaBombas();
  
  verificaValvulas();
  
  delay(500);
}

void calculaNivel(){
  if (leituraSensor1 > 50){
    if (leituraSensor2 > 50){
      if (leituraSensor3 > 50){
        if (leituraSensor4 > 50){
          nivelCisterna = 80;
        } else {
          nivelCisterna = 60;
        }
      } else {
        nivelCisterna = 40;
      } 
    } else {
      nivelCisterna = 20;
    }
  } else {
    nivelCisterna = 0;
  }
}

//impede o funcionamento das bombas em caso de reservatrio vazio
void verificaBombas(){
  if(nivelCisterna >= nivelFuncionamentoBombas){
    digitalWrite(pump1, HIGH); 
    digitalWrite(pump2, HIGH);
    statusPump1 = 1;
    statusPump2 = 1;
  } else {
    digitalWrite(pump1, LOW); 
    digitalWrite(pump2, LOW);
    statusPump1 = 0;
    statusPump2 = 0;
  }
}

void verificaValvulas(){
  if(nivelCisterna <= nivelAberturaValvulas){
   digitalWrite(valve1, HIGH); 
   digitalWrite(valve2, HIGH);
  }
 
 if (nivelCisterna >= nivelFechamentoValvulas){
   digitalWrite(valve1, LOW);
   digitalWrite(valve2, LOW);
  }
}


//escreve as leituras do sensores em um painel lcd de cristal liquido
void escreveTela(){
  limpaTela();
  
  lcd.setCursor(0,0);
  lcd.print("Nvel Cisterna:");
  lcd.setCursor(0,1);
  lcd.print(nivelCisterna);
  lcd.print(" %");
}

//"limpa" a tela do painel LCD de cristal liquido
void limpaTela(){
  lcd.setCursor(0,0);
  lcd.print("                ");
  lcd.setCursor(0,1);
  lcd.print("                ");
}

