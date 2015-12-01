#include "SPI.h"
#include "Ethernet.h"
#include "sha1.h"
#include "mysql.h"
#include <LiquidCrystal.h>

const int LM35 = 2;
float temperatura = 0;
int ADClido = 0;
int cont = 10;

LiquidCrystal lcd(12,11,5,4,3,2);

/* Setup for Ethernet Library */
byte mac_addr[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED };
IPAddress server_addr(192, 168, 1, 7);

/* Setup for the Connector/Arduino */
Connector my_conn; // The Connector/Arduino reference

char user[] = "root";
char password[] = "root";
char INSERT_SQL[] = 
 "INSERT INTO test_arduino.hello VALUES (%s, NULL)";
char INSERT_TEMP_SQL[] = 
 "INSERT INTO test_arduino.temperatura VALUES (Temperatura obtida no sensor = %s *C, NULL)";

void setup() {
  lcd.begin(16,2);
  lcd.setCursor(0,0);
  lcd.print("Ola Christian W.");
  lcd.setCursor(0,1);
  lcd.print("Surkamp");
  delay(10000);
  
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
  ADClido = analogRead(LM35);
  temperatura = ADClido * 0.48828125;
  
  lcd.setCursor(0,0);
  lcd.print("Temperatura =   ");
  lcd.setCursor(0,1);
  lcd.print(temperatura);
  lcd.print(" *C");
  delay(500);
  
  cont++;
  
  if (cont > 10){

    char query[100];
    char temperature[10];
    dtostrf(temperatura, 1, 1, temperature);
    /*preparando query*/
    sprintf(query, INSERT_TEMP_SQL, temperature);
    
    Serial.print("Novo comando de inserço SQL = ");
    Serial.println(query);
  }
}

