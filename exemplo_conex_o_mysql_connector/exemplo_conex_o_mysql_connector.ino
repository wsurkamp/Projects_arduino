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
 "INSERT INTO test_arduino.hello VALUES ('Hello, MySQL2!', NULL)";

void setup() {
  Ethernet.begin(mac_addr);
  Serial.begin(115200);
  delay(1000);
  if (my_conn.is_connected()) {
    Serial.println("Conection Active!");
    Serial.println("Insert into DB...");
    my_conn.cmd_query(INSERT_SQL);
    Serial.println("Success!");
  } else {
    my_conn.disconnect();
    Serial.println("Reconnecting...");
    if (my_conn.mysql_connect(server_addr, 3306, user, password)) {
      delay(500);
      my_conn.cmd_query(INSERT_SQL);
      Serial.println("Success!");
    } else
      Serial.println("Connect failed!");
  }
}

void loop() {
  delay(1000);
  if (Serial.available() > 0){
    if (my_conn.is_connected()) {
      Serial.println("Insert into DB...");
      Serial.println(Serial.read());
      my_conn.cmd_query(INSERT_SQL);
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
    Serial.println("Connecting new...");
    Serial.println(Serial.read());
    
      delay(500);
      /* Write Hello, World to MySQL table test_arduino.hello 
      my_conn.cmd_query(INSERT_SQL);
      Serial.println("Query Success!!!!!!!!!!!!"); */
  }
}
