const int LM35 = 2;
float temperatura = 0;
int ADCLido = 0;
const int Buzzer = 12;
const int LED[] = {2,3,4,5,6,7,8,9,10,11};
const float tempInicial = 20.00;
const float tempIncremento = 0.50;
float tempLED[] = {0,0,0,0,0,0,0,0,0,0};

void setup(){
  //Serial.begin(9600); //para utilizacao da saida serial
  analogReference(INTERNAL);
  pinMode(Buzzer, OUTPUT);
  for(int x = 0; x < 10; x++){
    pinMode(LED[x], OUTPUT);
    tempLED[x] = tempInicial + (tempIncremento * x);
  }
}

void loop(){
  ADCLido = analogRead(LM35);
  temperatura = ADCLido * 0.1075268817204301;
  
  if (temperatura > tempLED[0]){
    digitalWrite(LED[0], HIGH);
  }else{
    digitalWrite(LED[0], LOW);
  }
  
  if (temperatura > tempLED[1]){
    digitalWrite(LED[1], HIGH);
  }else{
    digitalWrite(LED[1], LOW);
  }
  
  if (temperatura > tempLED[2]){
    digitalWrite(LED[2], HIGH);
  }else{
    digitalWrite(LED[2], LOW);
  }
  
  if (temperatura > tempLED[3]){
    digitalWrite(LED[3], HIGH);
  }else{
    digitalWrite(LED[3], LOW);
  }
  
  if (temperatura > tempLED[4]){
    digitalWrite(LED[4], HIGH);
  }else{
    digitalWrite(LED[4], LOW);
  }
  
  if (temperatura > tempLED[5]){
    digitalWrite(LED[5], HIGH);
  }else{
    digitalWrite(LED[5], LOW);
  }
  
  if (temperatura > tempLED[6]){
    digitalWrite(LED[6], HIGH);
  }else{
    digitalWrite(LED[6], LOW);
  }
  
  if (temperatura > tempLED[7]){
    digitalWrite(LED[7], HIGH);
  }else{
    digitalWrite(LED[7], LOW);
  }
  
  if (temperatura > tempLED[8]){
    digitalWrite(LED[8], HIGH);
  }else{
    digitalWrite(LED[8], LOW);
  }
  
  if (temperatura > tempLED[9]){
    digitalWrite(LED[9], HIGH);
    digitalWrite(Buzzer, HIGH);
  }else{
    digitalWrite(LED[9], LOW);
    digitalWrite(Buzzer, LOW);
  }
  /*Serial.println("Temperatura = ");
  Serial.println(tempLED[0]);
  Serial.println(tempLED[1]);
  Serial.println(tempLED[2]);
  Serial.println(tempLED[3]);
  Serial.println(tempLED[4]);
  Serial.println(tempLED[5]);
  Serial.println(tempLED[6]);
  Serial.println(tempLED[7]);
  Serial.println(tempLED[8]);
  Serial.println(tempLED[9]);
  /*
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
  */
}
