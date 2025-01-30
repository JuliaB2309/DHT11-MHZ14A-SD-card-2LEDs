# DHT11-MHZ14A-SD-card-2LEDs
a project including the DHT11 sensor (humidity & temperature), the MH-Z14A sensor (CO2), a SD card (to log the data) and a Memory Card Shield Modul

The sensors measure every four minutes and write the data to the SD card.

The MH-Z14A only outputs correct values if it is calibrated.

Memory Card Shield Modul (→ SD card):

connect sck to GIOP 18

connect miso to GIOP 19

connect mosi to GIOP 23

connect cs to GIOP 5

connect VCC to 5 volt

connect GND to GND (Ground)

DHT11:

connect DATA-Pin to GIOP21

connect VCC to 3.3 volt

connect GND to GND (Ground)

MH-Z14A:

connect R to TX

connect T to RX

connect V+ to 5 volt

connect V- to GND (Ground)
