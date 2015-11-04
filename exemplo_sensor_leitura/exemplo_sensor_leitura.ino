const int LM35 = 2;
const int Buzzer = 12;
float temperatura = 0;
int ADClido = 0;

void setup() {
  Serial.begin(9600);
  analogReference(INTERNAL);
  pinMode(Buzzer, OUTPUT);
}

void loop() {
  ADClido = analogRead(LM35);
  temperatura = ADClido * 0.1075268817204301;
  if (temperatura > 35){
    digitalWrite(Buzzer, HIGH);
  } else {
    digitalWrite(Buzzer, LOW);
  }
  Serial.print("Temperatura = ");
  Serial.print(temperatura);
  Serial.print(" *C");
  Serial.print("    Analogica 0:  ");
  Serial.print(analogRead(A0));
  Serial.print("  Analogica 1:  ");
  Serial.print(analogRead(A1));
  Serial.print("  Analogica 2:  ");
  Serial.print(analogRead(2));
  Serial.print("  Analogica 3:  ");
  Serial.print(analogRead(3));
  Serial.print("  Analogica 4:  ");
  Serial.print(analogRead(4));
  Serial.print("  Analogica 5:  ");
  Serial.println(analogRead(5));
  delay(1000);
}
