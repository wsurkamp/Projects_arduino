const int ledPinVerde = 11;
const int ledPinVermelho = 13;
const int ledPinAmarelo = 12;
const int Botao1 = 4;
const int Botao2 = 3;
const int Botao3 = 2;
int EstadoBotao1 = 0;
int EstadoBotao2 = 0;
int EstadoBotao3 = 0;

void setup() {
  pinMode(ledPinVerde, OUTPUT);
  pinMode(ledPinAmarelo, OUTPUT);
  pinMode(ledPinVermelho, OUTPUT);
  pinMode(Botao1, INPUT);
  pinMode(Botao2, INPUT);
  pinMode(Botao3, INPUT);
}

void loop() {
  EstadoBotao1 = digitalRead(Botao1);
  EstadoBotao2 = digitalRead(Botao2);
  EstadoBotao3 = digitalRead(Botao3);
  if (EstadoBotao1 == HIGH){
    digitalWrite(ledPinVerde, HIGH);
  } else {
    digitalWrite(ledPinVerde, LOW);
  }
  if (EstadoBotao2 == HIGH){
    digitalWrite(ledPinAmarelo, HIGH);
  } else {
    digitalWrite(ledPinAmarelo, LOW);
  }
  if (EstadoBotao3 == HIGH){
    digitalWrite(ledPinVermelho, HIGH);
  } else {
    digitalWrite(ledPinVermelho, LOW);
  }
}
