const int LDR = 2;
const int Led = 6;
int valorLido = 0;
int pwm = 0;

void setup(){
  pinMode(Led, OUTPUT);
}

void loop(){
  valorLido = analogRead(LDR);
  
  if (valorLido < 500){
    analogWrite(Led, pwm);
    pwm++;
    delay(100);
  }
  else{
    digitalWrite(Led, LOW);
    pwm = 0;
  }
  
  if (pwm > 255){
    pwm = 255;
  }
}
