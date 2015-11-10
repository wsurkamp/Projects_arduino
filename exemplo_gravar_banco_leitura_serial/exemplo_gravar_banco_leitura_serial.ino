/**
* Example: Hello, MySQL!
*
* This code module demonstrates how to create a simple 
* database-enabled sketch.
*/
#include "SPI.h"
#include "Ethernet.h"
#include "sha1.h"
#include "mysql.h"

/* Setup for Ethernet Library */
byte mac_addr[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED };
IPAddress server_addr(192, 168, 1, 7);

/* Setup for the Connector/Arduino */
Connector my_conn; // The Connector/Arduino reference

char user[] = "root";
char password[] = "root";
char INSERT_SQL[] = 
 "INSERT INTO test_arduino.hello VALUES (%s, NULL)";

void setup() {
  Ethernet.begin(mac_addr);
  Serial.begin(9600);
  delay(1000);
  Serial.println("Connecting...");
  if (my_conn.mysql_connect(server_addr, 3306, user, password))
  {
    delay(500);
    char query[100];
    /*preparando query*/
    sprintf(query, INSERT_SQL, "Hello, MySQL! Setup");
    
     /* Write Hello, World to MySQL table test_arduino.hello */
     my_conn.cmd_query(query);
     Serial.println("Query Success!"); 
  } 
  else
    Serial.println("Connection failed.");
}

void loop() {
  if (Serial.available() > 0){
    String novaMensagem = leStringSerial();

    char query[100];
    /*preparando query*/
    sprintf(query, INSERT_SQL, novaMensagem);
    
    Serial.print("Novo comando de inserço SQL = ");
    Serial.println(query);
  }
}

/**
 * Função que lê uma string da Serial
 * e retorna-a
 */
String leStringSerial(){
  String conteudo = "";
  char caractere;
  
  // Enquanto receber algo pela serial
  while(Serial.available() > 0) {
    // Lê byte da serial
    caractere = Serial.read();
    // Ignora caractere de quebra de linha
    if (caractere != '\n'){
      // Concatena valores
      conteudo.concat(caractere);
    }
    // Aguarda buffer serial ler próximo caractere
    delay(10);
  }
    
  Serial.print("Recebi: ");
  Serial.println(conteudo);
    
  return conteudo;
}
