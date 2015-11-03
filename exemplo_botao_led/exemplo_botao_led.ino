const int ledPin = 13; //led no pino 13
const int Botao = 2; //botão no pino 2
int EstadoBotao = 0; //Variável para ler o status do pushbutton

void setup() {
  pinMode(ledPin, OUTPUT); //Pino do led será de saída
  pinMode(Botao, INPUT); //Pino com botão será de entrada
}

void loop() {
  EstadoBotao = digitalRead(Botao); /*novo estado do botão vai ser igual ao que o Arduino ler no
                                     pino onde está o botão. Poderá ser ALTO(HIGH) se o botão
                                     estiver pressionado, ou BAIXO (LOW), se o botão estiver solto */
  if (EstadoBotao == HIGH){ //Se o botão estiver pressionado (HIGH)
    digitalWrite(ledPin, HIGH); //acende o led do pino 13.
  } else {                      //se não estiver pressionado
    digitalWrite(ledPin, LOW);  //deixa o led do pino 13 apagado.
  }
}
