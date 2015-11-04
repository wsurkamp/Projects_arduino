const int PinPotenciometro = A2;
const int Led = 13;
int valorPot = 0;

void setup(){
  pinMode(Led, OUTPUT);
}

void loop(){
  valorPot = analogRead(PinPotenciometro);
  digitalWrite(Led, HIGH);
  delay(valorPot);
  digitalWrite(Led, LOW);
  delay(valorPot);
}
