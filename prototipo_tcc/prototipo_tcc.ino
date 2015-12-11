#include "SPI.h"
#include "Ethernet.h"
#include "sha1.h"
#include "mysql.h"

//constantes para definiçao de portas de conexao dos componentes
const int SensorAgua1 = 2;
const int SensorAgua2 = 3;
const int SensorAgua3 = 4;
const int SensorAgua4 = 5;
const int pump1 = 4;
const int pump2 = 3;
const int valve1 = 6;
const int valve2 = 5;

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

byte mac_addr[] = { 0xDE, 0xBE, 0xAD, 0xEF, 0xFE, 0xED }; //endereço mac do ethernet shield
IPAddress ip(192,168,1,13); //ip do arduino
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

char UPDATE_CISTERN1[] = 
 "UPDATE sistema_de_monitoramento.cistern SET level = %d WHERE id_cistern = 1";

void setup(){
  Ethernet.begin(mac_addr);
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
  
  verificaParametros(); //atualiza parametros de funcionamento do sistema
  
  verificaBombas(); //controla o funcionamento das bombas
  
  verificaValvulas(); //controla o funcionamento das valvulas
  
  atualizaBanco(); //atualiza o stado dos componentes no banco de dados
  
  delay(2000);
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
  char queryCistern1[100];
  
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
  
  dtostrf(nivelCisterna, 1, 1, letterChar);
  sprintf(queryCistern1, UPDATE_CISTERN1, nivelCisterna);
  
  if (my_conn.is_connected()) {
    //atualiza os dados no banco remoto
    my_conn.cmd_query(queryValve1);
    my_conn.cmd_query(queryValve2);
    my_conn.cmd_query(queryPump1);
    my_conn.cmd_query(queryPump2);
    my_conn.cmd_query(querySensor1);
    my_conn.cmd_query(querySensor2);
    my_conn.cmd_query(querySensor3);
    my_conn.cmd_query(querySensor4);
    my_conn.cmd_query(queryCistern1);
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

//atualizaçao dos parametros de funcionamento do sistema
void verificaParametros(){
  int cont = 0;
  char select_values[] = "SELECT value FROM sistema_de_monitoramento.operatingParameter";

  if (my_conn.is_connected()) {
    
    my_conn.cmd_query(select_values);
    my_conn.get_columns();
    
    row_values *row = NULL;
    do {
      row = my_conn.get_next_row();
      
      if (row != NULL) {
        if(cont == 0){
          nivelFuncionamentoBombas = atoi(row->values[0]);
        } else if(cont == 1){
          nivelAberturaValvulas = atoi(row->values[0]);
        } else if(cont == 2){
          nivelFechamentoValvulas = atoi(row->values[0]);
        }
      }
      my_conn.free_row_buffer();
      cont = cont + 1;
    } while (row != NULL);
    my_conn.free_columns_buffer();  
    delay(2000);
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
}
