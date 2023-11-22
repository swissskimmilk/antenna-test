#include <RadioLib.h>
#include <SPI.h>
#include <map>

#define MOSI 11
#define CLK 12
#define MISO 13
#define RF_FREQUENCY                                915000000 // Hz  center frequency
#define TX_OUTPUT_POWER                             22        // dBm tx output power

std::map<int,int> bandwidthMappings = {{1, 31.25E3}, {2, 62.5E3}, {3, 125E3}, {4, 250E3}, {5, 500E3}};

SX1262 radio = new Module(15, 21, 6, 39);

char longString[] = "Space Technology At Berkeley SSEP2 Long Range Antenna Parameter Acquisition | Current parameters <Spreading factor ID %.2hu> <Bandwith ID %hu> <Coding rate ID %hu> | Transmission from the #1 public university in the world! Fiat lux! Go Bears! Let there be light!";
char medString[] = "STAR SSEP2 Long Range Antenna Parameter Acquisition | Current parameters <Spreading factor ID %.2hu> <Bandwith ID %hu> <Coding rate ID %hu>";
char shortString[] = "<Spreading factor ID %.2hu> <Bandwith ID %hu> <Coding rate ID %hu>";

int transmissionsPerSetting = 100;

// 39 is the busy pin, but we are not using it rn 

void setup() 
{
  Serial.begin(9600);

  // initialize SX1262 with default settings
  Serial.print(F("[SX1262] Initializing ... "));
  int state = radio.begin();
  if (state == RADIOLIB_ERR_NONE) {
    Serial.println(F("success!"));
  } else {
    Serial.print(F("failed, code "));
    Serial.println(state);
    while (true);
  }
}

void loop() 
{
  // Serial.println("\nProvide input with the following structure:\n[Spreading factor (07-12)] [Bandwidth (1, 5)] [Coding rate (5, 8)] [String length (1, 3)]\nEx: 08 4 2 2");
  // String dataIn = Serial.readStringUntil('\n');
  // while (dataIn == "") {
  //   dataIn = Serial.readStringUntil('\n');
  // }

  // int i = ((dataIn[0] - 48) * 10 + (dataIn[1] - 48));
  // int j = (dataIn[3] - 48);
  // int k = (dataIn[5] - 48);
  // int m = (dataIn[7] - 48);

  // LoRa.setSpreadingFactor(i);
  // LoRa.setSignalBandwidth(bandwidthMappings[j]);
  // LoRa.setCodingRate4(k);
  
  // uint8_t txData[255];
  // if (m == 0) {
  //   sprintf((char *)txData, shortString, i, j, k);
  // }
  // else if (m == 1) {
  //   sprintf((char *)txData, medString, i, j, k);
  // }
  // else {
  //   sprintf((char *)txData, longString, i, j, k);
  // }

  // for (int p = 0; p < transmissionsPerSetting; p++){
  //   uint8_t len = strlen((char *)txData);
  //   // Serial.println();
  //   // for(int n = 0; n < len; n++) {
  //   //   if (txData[n] > 0x1F && txData[n] < 0x7F) {
  //   //     char myChar = txData[n];
  //   //     Serial.print(myChar);
  //   //   } else {
  //   //     Serial.print("?");
  //   //   }
  //   // }
  //   // Serial.println();

  //   // Wait for transmission to complete
  //   LoRa.beginPacket();
  //   LoRa.write(txData, len);
  //   if (LoRa.endPacket()) {
  //     Serial.println("Send success");
  //   } else {
  //     Serial.println("Send fail");
  //     break;
  //   }
  //   delay(1);

  //   // Don't wait to finish
  //   // lora.Send(txData, len, SX126x_TXMODE_ASYNC);
  //   // delay(100);
  // }
}