#include <LiquidCrystal.h>
#include "SPI.h"
#include "Ethernet.h"
#include "sha1.h"
#include "mysql.h"

//constantes para definiçao de portas de conexao dos componentes
const int SensorAgua1 = 12;
const int SensorAgua2 = 13;
const int SensorAgua3 = 14;
const int SensorAgua4 = 15;
const int pump1 = 20;
const int pump2 = 21;
const int valve1 = 14;
const int valve2 = 15;

//variveis para leitura dos sensores
int leituraSensor1 = 0;
int leituraSensor2 = 0;
int leituraSensor3 = 0;
int leituraSensor4 = 0;

//variveis de controle
int statusSensor1 = 0;
int statusSensor2 = 0;
int statusSensor3 = 0;
int statusSensor4 = 0;
int statusValve1 = 0;
int statusValve2 = 0;
int statusPump1 = 0;
int statusPump2 = 0;

int nivelCisterna = 0;

//parmetros iniciais para funcionamento do sistema
int nivelFuncionamentoBombas = 20;
int nivelAberturaValvulas = 40;
int nivelFechamentoValvulas = 60;

LiquidCrystal lcd(13,12,11,10,9,8);

byte mac_addr[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED }; //endereço mac do ethernet shield
IPAddress ip(192,168,1,3); //ip do arduino
IPAddress server_addr(192, 168, 1, 2); //ip do servidor MySQL

Connector my_conn;

//dados de acesso ao banco de dados
char user[] = "root";
char password[] = "surkamp";

//variveis para atualizacao dos dados no banco
char UPDATE_VALVE1[] = 
 "UPDATE sistema_de_monitoramento.valve SET open = %d WHERE id_valve = 1";
char UPDATE_VALVE2[] = 
 "UPDATE sistema_de_monitoramento.valve SET open = %d WHERE id_valve = 2";
 
char UPDATE_PUMP1[] = 
 "UPDATE sistema_de_monitoramento.pump SET active = %d WHERE id_pump = 1";
char UPDATE_PUMP2[] = 
 "UPDATE sistema_de_monitoramento.pump SET active = %d WHERE id_pump = 2";
 
char UPDATE_SENSOR1[] = 
 "UPDATE sistema_de_monitoramento.sensor SET value = %d WHERE id_sensor = 1";
char UPDATE_SENSOR2[] = 
 "UPDATE sistema_de_monitoramento.sensor SET value = %d WHERE id_sensor = 2";
char UPDATE_SENSOR3[] = 
 "UPDATE sistema_de_monitoramento.sensor SET value = %d WHERE id_sensor = 3";
char UPDATE_SENSOR4[] = 
 "UPDATE sistema_de_monitoramento.sensor SET value = %d WHERE id_sensor = 4";

void setup(){
  //inicializacao do sistema
  lcd.begin(16,2);
  lcd.setCursor(0,0);
  lcd.print("Gerenciamento de");
  lcd.setCursor(0,1);
  lcd.print("Cisternas - TCC");

  Ethernet.begin(mac_addr, ip);
  Serial.begin(115200);
  Serial.print("Arduino IP ");
  Serial.println(Ethernet.localIP());
  Serial.println("Conecting...");

  if (my_conn.mysql_connect(server_addr, 3306, user, password))
    Serial.println("Connect Success!");
  else
    Serial.println("Connect failed!");
    
  pinMode(pump1, OUTPUT);
  pinMode(pump2, OUTPUT);
  pinMode(valve1, OUTPUT);
  pinMode(valve2, OUTPUT);
  delay(500);
}

void loop(){
  //realiza leitura dos sensores
  leituraSensor1 = analogRead(SensorAgua1);
  leituraSensor2 = analogRead(SensorAgua2);
  leituraSensor3 = analogRead(SensorAgua3);
  leituraSensor4 = analogRead(SensorAgua4);
  
  calculaNivel(); //verifica nvel da cisterna
  
  //verificaParametros(); //atualiza parametrso de funcionamento do sistema
  
  escreveTela(); //indica nivel na cisterna no display LCD
  
  verificaBombas(); //controla o funcionamento das bombas
  
  verificaValvulas(); //controla o funcionamento das valvulas
  
  atualizaBanco(); //atualiza o stado dos componentes no banco de dados
  
  delay(500);
}

void atualizaBanco(){
  //variveis utilizadas na preparaço das querys
  char letterChar[10];
  char queryValve1[100];
  char queryValve2[100];
  char queryPump1[100];
  char queryPump2[100];
  char querySensor1[100];
  char querySensor2[100];
  char querySensor3[100];
  char querySensor4[100];
  
  //preparando querys para atualizaço do banco
  dtostrf(statusValve1, 1, 1, letterChar);
  sprintf(queryValve1, UPDATE_VALVE1, statusValve1);
  
  dtostrf(statusValve2, 1, 1, letterChar);
  sprintf(queryValve2, UPDATE_VALVE2, statusValve2);
  
  dtostrf(statusPump1, 1, 1, letterChar);
  sprintf(queryPump1, UPDATE_PUMP1, statusPump1);
  
  dtostrf(statusPump2, 1, 1, letterChar);
  sprintf(queryPump2, UPDATE_PUMP2, statusPump2);
  
  dtostrf(statusSensor1, 1, 1, letterChar);
  sprintf(querySensor1, UPDATE_SENSOR1, statusSensor1);
  
  dtostrf(statusSensor2, 1, 1, letterChar);
  sprintf(querySensor2, UPDATE_SENSOR2, statusSensor2);
  
  dtostrf(statusSensor3, 1, 1, letterChar);
  sprintf(querySensor3, UPDATE_SENSOR3, statusSensor3);
  
  dtostrf(statusSensor4, 1, 1, letterChar);
  sprintf(querySensor4, UPDATE_SENSOR4, statusSensor4);
  
  Serial.println(queryValve1);
  Serial.println(queryValve2);
  Serial.println(queryPump1);
  Serial.println(queryPump2);
  Serial.println(querySensor1);
  Serial.println(querySensor2);
  Serial.println(querySensor3);
  Serial.println(querySensor4);
  
  
   if (my_conn.is_connected()) {
    //my_conn.cmd_query("UPDATE sistema_de_monitoramento.valve set open = '1' where id_valve = 2");
    my_conn.cmd_query(queryValve1);
    my_conn.cmd_query(queryValve2);
    my_conn.cmd_query(queryPump1);
    my_conn.cmd_query(queryPump2);
    my_conn.cmd_query(querySensor1);
    my_conn.cmd_query(querySensor2);
    my_conn.cmd_query(querySensor3);
    my_conn.cmd_query(querySensor4);
    Serial.println("Updating database OK!");
   } else {
    my_conn.disconnect();
    Serial.println("Connecting...");
    if (my_conn.mysql_connect(server_addr, 3306, user, password)) {
      delay(500);
      Serial.println("Success!");
    } else {
      Serial.println("Connect failed!");
    }
   }
  
  
  
  /*
  //preparando querys
  char queryValve1[100];
  char queryValve2[100];
  char queryPump1[100];
  char queryPump2[100];
  char querySensor1[100];
  char querySensor2[100];
  char querySensor3[100];
  char querySensor4[100];
  
  int statusSensor1 = 1;
  int statusSensor2 = 1;
  int statusSensor3 = 1;
  int statusSensor4 = 1;
  
  sprintf(queryValve1, UPDATE_VALVE1, statusValve1);
  sprintf(queryValve2, UPDATE_VALVE2, statusValve2);
  sprintf(queryPump1, UPDATE_PUMP1, statusPump1);
  sprintf(queryPump2, UPDATE_PUMP2, statusPump2);
  sprintf(querySensor1, UPDATE_SENSOR1, statusSensor1);
  sprintf(querySensor2, UPDATE_SENSOR2, statusSensor2);
  sprintf(querySensor3, UPDATE_SENSOR3, statusSensor3);
  sprintf(querySensor4, UPDATE_SENSOR4, statusSensor4);
  *//*
  if (statusValve1 == 1)
    char UPDATE_VALVE1[] = "UPDATE sistema_de_monitoramento.valve SET open = 1 WHERE id_valve = 1";
  else 
    char UPDATE_VALVE1[] = "UPDATE sistema_de_monitoramento.valve SET open = 0 WHERE id_valve = 1";
    
  if (statusValve2 == 1)
    char UPDATE_VALVE2[] = "UPDATE sistema_de_monitoramento.valve SET open = 1 WHERE id_valve = 2";
  else 
    char UPDATE_VALVE2[] = "UPDATE sistema_de_monitoramento.valve SET open = 0 WHERE id_valve = 2";

  if (statusPump1 == 1)
    char UPDATE_PUMP1[] = "UPDATE sistema_de_monitoramento.pump SET open = 1 WHERE id_pump = 1";
  else 
    char UPDATE_PUMP1[] = "UPDATE sistema_de_monitoramento.pump SET open = 0 WHERE id_pump = 1";
    
  if (statusPump2 == 1)
    char UPDATE_PUMP2[] = "UPDATE sistema_de_monitoramento.pump SET open = 1 WHERE id_pump = 2";
  else 
    char UPDATE_PUMP2[] = "UPDATE sistema_de_monitoramento.pump SET open = 0 WHERE id_pump = 2";

  if (nivelCisterna >= 80){
    char UPDATE_SENSOR1[] = 
     "UPDATE sistema_de_monitoramento.sensor SET value = 1 WHERE id_sensor = 1";
    char UPDATE_SENSOR2[] = 
     "UPDATE sistema_de_monitoramento.sensor SET value = 1 WHERE id_sensor = 2";
    char UPDATE_SENSOR3[] = 
     "UPDATE sistema_de_monitoramento.sensor SET value = 1 WHERE id_sensor = 3";
    char UPDATE_SENSOR4[] = 
     "UPDATE sistema_de_monitoramento.sensor SET value = 1 WHERE id_sensor = 4";
  } else if (nivelCisterna >= 60){
    char UPDATE_SENSOR1[] = 
     "UPDATE sistema_de_monitoramento.sensor SET value = 1 WHERE id_sensor = 1";
    char UPDATE_SENSOR2[] = 
     "UPDATE sistema_de_monitoramento.sensor SET value = 1 WHERE id_sensor = 2";
    char UPDATE_SENSOR3[] = 
     "UPDATE sistema_de_monitoramento.sensor SET value = 1 WHERE id_sensor = 3";
    char UPDATE_SENSOR4[] = 
     "UPDATE sistema_de_monitoramento.sensor SET value = 0 WHERE id_sensor = 4";
  } else if (nivelCisterna >= 40){
    char UPDATE_SENSOR1[] = 
     "UPDATE sistema_de_monitoramento.sensor SET value = 1 WHERE id_sensor = 1";
    char UPDATE_SENSOR2[] = 
     "UPDATE sistema_de_monitoramento.sensor SET value = 1 WHERE id_sensor = 2";
    char UPDATE_SENSOR3[] = 
     "UPDATE sistema_de_monitoramento.sensor SET value = 0 WHERE id_sensor = 3";
    char UPDATE_SENSOR4[] = 
     "UPDATE sistema_de_monitoramento.sensor SET value = 0 WHERE id_sensor = 4";
  } else if (nivelCisterna >= 20){
    char UPDATE_SENSOR1[] = 
     "UPDATE sistema_de_monitoramento.sensor SET value = 1 WHERE id_sensor = 1";
    char UPDATE_SENSOR2[] = 
     "UPDATE sistema_de_monitoramento.sensor SET value = 0 WHERE id_sensor = 2";
    char UPDATE_SENSOR3[] = 
     "UPDATE sistema_de_monitoramento.sensor SET value = 0 WHERE id_sensor = 3";
    char UPDATE_SENSOR4[] = 
     "UPDATE sistema_de_monitoramento.sensor SET value = 0 WHERE id_sensor = 4";
  } else {
    char UPDATE_SENSOR1[] = 
     "UPDATE sistema_de_monitoramento.sensor SET value = 0 WHERE id_sensor = 1";
    char UPDATE_SENSOR2[] = 
     "UPDATE sistema_de_monitoramento.sensor SET value = 0 WHERE id_sensor = 2";
    char UPDATE_SENSOR3[] = 
     "UPDATE sistema_de_monitoramento.sensor SET value = 0 WHERE id_sensor = 3";
    char UPDATE_SENSOR4[] = 
     "UPDATE sistema_de_monitoramento.sensor SET value = 0 WHERE id_sensor = 4";
  }
  
  //executa as inserçes no banco de dados
  if (my_conn.is_connected()) {
    my_conn.cmd_query(UPDATE_VALVE1);
    my_conn.cmd_query(UPDATE_VALVE2);
    my_conn.cmd_query(UPDATE_PUMP1);
    my_conn.cmd_query(UPDATE_PUMP2);
    my_conn.cmd_query(UPDATE_SENSOR1);
    my_conn.cmd_query(UPDATE_SENSOR2);
    my_conn.cmd_query(UPDATE_SENSOR3);
    my_conn.cmd_query(UPDATE_SENSOR4);
    Serial.println("Inserts finsh!!!!!!!!!!!!!!!");
  } else {
    my_conn.disconnect();
    Serial.println("Connecting...");
    if (my_conn.mysql_connect(server_addr, 3306, user, password)) {
      delay(500);
      Serial.println("Success!");
    } else {
      Serial.println("Connect failed!");
    }
  }*/
}

void calculaNivel(){
  if (leituraSensor1 > 50)
    statusSensor1 = 1;
  else
    statusSensor1 = 0;
    
  if (leituraSensor2 > 50)
    statusSensor2 = 1;
  else
    statusSensor2 = 0;
    
  if (leituraSensor3 > 50)
    statusSensor3 = 1;
  else
    statusSensor3 = 0;
    
  if (leituraSensor4 > 50)
    statusSensor4 = 1;
  else
    statusSensor4 = 0;
    
  //verifica o nivel propriamente dito
  if (statusSensor4 == 1)
    nivelCisterna = 80;
  else if (statusSensor3 == 1)
    nivelCisterna = 60;
  else if (statusSensor2 == 1)
    nivelCisterna = 40;
  else if (statusSensor1 == 1)
    nivelCisterna = 20;
  else
    nivelCisterna = 00;
    
    
    
  /*
  if (leituraSensor1 > 50){
    statusSensor1 = 1;
    if (leituraSensor2 > 50){
      statusSensor2 = 1;
      if (leituraSensor3 > 50){
        statusSensor3 = 1;
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
  }*/
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
   statusValve1 = 1;
   statusValve2 = 1;
  }
 
 if (nivelCisterna >= nivelFechamentoValvulas){
   digitalWrite(valve1, LOW);
   digitalWrite(valve2, LOW);
   statusValve1 = 0;
   statusValve2 = 0;
  }
}

/*
void verificaParametros(){
  int cont = 0;
  char select_values[] = "SELECT value FROM operatingParameter";
//  char selec_value_pump[] = "SELECT value FROM operatingParameter WHERE id_parameter = '1'";
//  char selec_value_open_valve[] = "SELECT value FROM operatingParameter WHERE id_parameter = '2'";
//  char selec_value_close_valve[] = "SELECT value FROM operatingParameter WHERE id_parameter = '3'";

  my_conn.cmd_query(select_values);
  my_conn.get_columns();
  
  row_values *row = NULL;
  do {
    row = my_conn.get_next_row();
    if (row != NULL) {
      if(cont == 0){
        nivelFuncionamentoBombas = atol(row->values[0]);
      } else if(cont == 1){
        nivelAberturaValvulas = atol(row->values[0]);
      } else if(cont == 2){
        nivelFechamentoValvulas = atol(row->values[0]);
      }
    }
    my_conn.free_row_buffer();
  } while (row != NULL);
  my_conn.free_columns_buffer();
  
  
}
*/

//escreve as leituras do sensores em um painel lcd de cristal liquido
void escreveTela(){
  limpaTela();
  
  lcd.setCursor(0,0);
  lcd.print("Nivel Cisterna:");
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

