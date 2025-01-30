#include <SPI.h>
#include <SD.h>
#include <DHT.h>
#include <Wire.h>
#include "DHT11.h"
#include "DHTesp.h"
#include "FS.h"
#include <HardwareSerial.h>

#define sck 18
#define miso 19
#define mosi 23
#define cs 5
#define DHTPIN 21       
#define DHTTYPE DHT11          

DHT dht(DHTPIN, DHTTYPE);
HardwareSerial mySerial(2); 

byte requestRead[] = {0xFF, 0x01, 0x86, 0x00, 0x00, 0x00, 0x00, 0x00, 0x79};
byte response[9];
long lfdNr = 0;

const byte led1_gpio = 32; //red
const byte led2_gpio = 33; //green

File dataFile;

void setup() {
  Serial.begin(115200);
  dht.begin();
  pinMode (led1_gpio, OUTPUT);
  pinMode (led2_gpio, OUTPUT);
  mySerial.begin(9600, SERIAL_8N1, 16, 17); 
  Serial.println("Start MH-Z14A");
  SPI.begin(sck, miso, mosi, cs);

  if (!SD.begin(cs)) {
    Serial.println("Card Mount Failed");
    while (1);
  }

  Serial.println("Initialization complete. Logging data...");
  
  if (!SD.exists("/Test.csv")) {
    Serial.println("Creating new data file...");
  }
}

void appendFile(fs::FS &fs, const char *path, const char *message) {
  Serial.printf("Appending to file: %s\n", path);

  File file = fs.open(path, FILE_APPEND);
  
  if (!file) {
    Serial.println("Failed to open file for appending");
    return;
  }

  if (file.print(message)) {
    Serial.println("Message appended");
  } 
  else {
    Serial.println("Append failed");
  }
  file.close();
}


void loop() {
 sendRequest();
 if (readResponse()) {
   int co2 = getCO2Value();
   float humidity = dht.readHumidity();
   float temperature = dht.readTemperature();

   Serial.print("Temperature: ");
   Serial.print(temperature);
   Serial.print(" °C, Humidity: ");
   Serial.print(humidity);
   Serial.print(" %, CO2: ");
   Serial.print(co2);
   Serial.println(" ppm");

   char charVal0[50];
   dtostrf(lfdNr, 10, 0, charVal0);
   strcat(charVal0, ", ");
   appendFile(SD, "/Test.csv", charVal0);

   char charVal1[10];
   dtostrf(temperature, 4, 3, charVal1);
   appendFile(SD, "/Test.csv", charVal1);
   char ziel1[50] = ",  ";
   char quelle1[] = "   ";
   strcat(ziel1, quelle1);
   appendFile(SD, "/Test.csv", ziel1);
    
   char charVal2[10];
   dtostrf(humidity, 4, 3, charVal2);
   appendFile(SD, "/Test.csv", charVal2);
   char ziel2[50] = ",  ";
   char quelle2[] = "          ";
   strcat(ziel2, quelle2);
   appendFile(SD, "/Test.csv", ziel2);
    
   char charVal3[10];
   dtostrf(co2, 4, 3, charVal3);
   appendFile(SD, "/Test.csv", charVal3);
   char ziel3[50] = ",    ";
   char quelle3[] = "\n";
   strcat(ziel3, quelle3);
   appendFile(SD, "/Test.csv", ziel3);

   digitalWrite (led2_gpio, HIGH);
   delay (200);
   
   delay(240000);
 }  

 else {
  Serial.println("Error reading the data");
  delay(1000);
  digitalWrite (led1_gpio, HIGH);
  delay (200);
  digitalWrite (led1_gpio, LOW);
  delay (200);
 }
 lfdNr++;
}


void sendRequest() {
 mySerial.write(requestRead, 9); 
}

bool readResponse() {
 int index = 0;

 while (mySerial.available() == 0) {
  delay(100);
  }

  while (index < 9) {
   byte b = mySerial.read();
    if (index == 0 && b != 0xFF) { 
    }
    if (index < 9) {
     response[index] = b;
    }
    index++;
    if (index == 9) break; 
  }
  if (index != 9) {
    Serial.print("Response less than 9 byte (");
    Serial.print(index);
    Serial.println(")");
  } 
  else if (response[0] != 0xFF) {
    Serial.print("response[0] != 0xFF (");
    Serial.print(response[0]);
    Serial.println(")");
  } 
  else if (response[1] != 0x86) {
    Serial.print("response[1] != 0x86 (");
    Serial.print(response[1]);
    Serial.println(")");
  }
  return index == 9 && response[0] == 0xFF && response[1] == 0x86;
}

int getCO2Value() {
 return (response[2] << 8) + response[3]; 
}


