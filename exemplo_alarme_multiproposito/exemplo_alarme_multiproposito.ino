const int LDR = 2;
const int LM35 = 3;
const int Buzzer = 12;
const int led[] = {5,6,7,8,9,10,11};
int valorLDR = 0;
int ADCLido = 0;
float temperatura = 0;
int pwm = 0;

void setup(){
  for(int x=0; x<7; x++){
    pinMode(led[x], OUTPUT);
  }
  pinMode(Buzzer, OUTPUT);
}

void loop(){
  valorLDR = analogRead(LDR);
  ADCLido = analogRead(LM35);
  temperatura = ADCLido * 0.48828125;
  if (temperatura > 20.00){
    digitalWrite(led[0], HIGH);
  } else{
    digitalWrite(led[0], LOW);
  }
  
  if (temperatura > 22.00){
    digitalWrite(led[1], HIGH);
  } else{
    digitalWrite(led[1], LOW);
  }
  
  if (temperatura > 24.00){
    digitalWrite(led[2], HIGH);
  } else{
    digitalWrite(led[2], LOW);
  }
  
  if (valorLDR > 500){
    digitalWrite(led[5], HIGH);
  } else{
    digitalWrite(led[5], LOW);
  }
  
  if (valorLDR > 400){
    digitalWrite(led[4], HIGH);
  } else{
    digitalWrite(led[4], LOW);
  }
  
  if (valorLDR > 350){
    digitalWrite(led[3], HIGH);
    digitalWrite(led[6], LOW);
    digitalWrite(Buzzer, LOW);
  } else{
    digitalWrite(led[3], LOW);
    digitalWrite(led[6], HIGH);
    digitalWrite(Buzzer, HIGH);
  }
}
