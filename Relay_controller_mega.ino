#include "CommandLine.h"



void setup() {
  // Pin for relay module set as output and turn off
  for(int i=1; i<=16; i++){
    pinMode(i+21, OUTPUT);
    digitalWrite(i+21, HIGH);
  }
  
  Serial.begin(115200);

}

void loop() {
  bool received = getCommandLineFromSerialPort(CommandLine);      //global CommandLine is defined in CommandLine.h
  if (received) ExecuteCommand(CommandLine);

}
