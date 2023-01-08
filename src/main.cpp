#include <Arduino.h>
#include <ModbusMaster.h>
#include <SPI.h>
#include <SD.h>

ModbusMaster node;

void ModbusRead();
void dataSD();

const int chipSelect = D8;

void setup() {
  Serial.begin(9600); //SERIAL_8E1
 // node.begin(1, Serial); // ModBus Slave adress 1

  if (!SD.begin(chipSelect)) {
    Serial.println("Card failed, or not present");
    // don't do anything more:
    return;
  }
  Serial.println("card initialized.");
}

void loop() {
  //dataSD();
  
}

void ModbusRead(){
  uint8_t result;  
  result = node.readHoldingRegisters(0xE, 2); // 0xE + 0xE+1 2 registers uint16_t

// maak van 2 uint16_t register een uint32_t waarde
  uint16_t data[2];
  data[0] = node.getResponseBuffer(0);
  data[1] = node.getResponseBuffer(1);
  float x;
  unsigned long *p;

  p = (unsigned long*)&x;
  *p = (unsigned long)data[0] <<16 | data[1];

  if(result == node.ku8MBSuccess){
    Serial.print("Voltage L1: ");
    Serial.println(x, 1);
  
  } else{
    Serial.print("fout ");
  }
}


void dataSD(){
  String dataString = "";
  uint8_t result;
  result = node.readHoldingRegisters(0xE, 2); // 0xE Voltage

  uint16_t data[2];
  data[0] = node.getResponseBuffer(0);
  data[1] = node.getResponseBuffer(1);
  float x;
  unsigned long *p;

  p = (unsigned long*)&x;
  *p = (unsigned long)data[0] <<16 | data[1];

  dataString += String(x);

  File datafile = SD.open("datalog.txt", FILE_WRITE);

  if(datafile){
    datafile.println(dataString);
    datafile.close();

  }else{
    Serial.println("Error opening datalog.txt");
  }
  delay(1000);
}





