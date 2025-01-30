#include <HardwareSerial.h>

HardwareSerial mySerial(2); 

void setup() {
  
 Serial.begin(115200);      
 mySerial.begin(9600, SERIAL_8N1, 16, 17); 
 Serial.println("Start the calibration of the MH-Z14A");
  
 delay(20000); 

 byte calibrateCommand[] = {0xFF, 0x01, 0x87, 0x00, 0x00, 0x00, 0x00, 0x00, 0x78};
 mySerial.write(calibrateCommand, 9); 
 Serial.println("Calibration command sent. Please wait...");

 delay(5000); 

 Serial.println("Calibration completed");
}

void loop() {}
