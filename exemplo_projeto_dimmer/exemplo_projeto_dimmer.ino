const int PinPotenciometro = A2;
const int Led = 11;
int valorPot = 0;
int pwm = 0;

void setup(){
  pinMode(Led, OUTPUT);
}

void loop(){
  valorPot = analogRead(PinPotenciometro);
  pwm = map(valorPot, 0, 1023, 0, 255);
  analogWrite(Led, pwm);
}
