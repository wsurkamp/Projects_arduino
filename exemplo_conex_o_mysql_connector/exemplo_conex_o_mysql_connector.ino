#include "SPI.h"
#include "Ethernet.h"
#include "sha1.h"
#include "mysql.h"

byte mac_addr[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED };
IPAddress ip(192,168,1,3);
IPAddress server_addr(192, 168, 1, 2);

Connector my_conn;

char user[] = "root";
char password[] = "surkamp";
char INSERT_SQL[] = "INSERT INTO test_arduino.hello VALUES ('Hello, MySQL2!', NULL)";
char INSERT_TEMP_SQL[] = "INSERT INTO test_arduino.hello VALUES ('Temperatura obtida no sensor - %d ', NULL)";

void setup() {
  Ethernet.begin(mac_addr, ip);
  Serial.begin(115200);
  Serial.print("Arduino IP ");
  Serial.println(Ethernet.localIP());
  Serial.println("Conecting...");
  
  if (my_conn.mysql_connect(server_addr, 3306, user, password)) {
    delay(500);
    my_conn.cmd_query(INSERT_SQL);
    Serial.println("Query Success!");
  } else
    Serial.println("Connect failed!");
}

void loop() {
  delay(1000);
  if (Serial.available() > 0){
    if (my_conn.is_connected()) {
      Serial.println("Insert into DB...");
      int letter = Serial.read();
      char query[100];
      char letterChar[10];
      dtostrf(letter, 1, 1, letterChar);
      
      sprintf(query, INSERT_TEMP_SQL, letter);
      
      Serial.println(letter);
      Serial.println(query);
      my_conn.cmd_query(query);
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
}
