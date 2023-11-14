#include <Ra01S.h>
#include <SPI.h>

#define MOSI 11
#define CLK 12
#define MISO 13
//#define RF_FREQUENCY                                433000000 // Hz  center frequency
//#define RF_FREQUENCY                                866000000 // Hz  center frequency
#define RF_FREQUENCY                                915000000 // Hz  center frequency
#define TX_OUTPUT_POWER                             22        // dBm tx output power
#define LORA_SPREADING_FACTOR                       7         // spreading factor [SF5..SF12]
#define LORA_BANDWIDTH                              4         // bandwidth
                                                              // 2: 31.25Khz
                                                              // 3: 62.5Khz
                                                              // 4: 125Khz
                                                              // 5: 250KHZ
                                                              // 6: 500Khz 
#define LORA_CODINGRATE                             1         // [1: 4/5,
                                                              //  2: 4/6,
                                                              //  3: 4/7,
                                                              //  4: 4/8]

#define LORA_PREAMBLE_LENGTH                        8         // Same for Tx and Rx
#define LORA_PAYLOAD_LENGTH                          0         // 0: Variable length packet (explicit header)
                                                              // 1..255  Fixed length packet (implicit header)
int spreadingFactorIDs[] = {7, 8, 9, 10, 11, 12};
int bandwidthIDs[] = {2, 3, 4, 5, 6};
int codingRate[] = {1, 2, 3, 4};
char longString[] = "Space Technology At Berkeley SSEP2 Long Range Antenna Parameter Acquisition | Current parameters <Spreading factor ID %.2hu> <Bandwith ID %hu> <Coding rate ID %hu> | Transmission from the #1 public university in the world! Fiat lux! Go Bears! Let there be light!";
char medString[] = "STAR SSEP2 Long Range Antenna Parameter Acquisition | Current parameters <Spreading factor ID %.2hu> <Bandwith ID %hu> <Coding rate ID %hu>";
char shortString[] = "<Spreading factor ID %.2hu> <Bandwith ID %hu> <Coding rate ID %hu>";

int transmissionsPerSetting = 100;

SX126x  lora(15,               //Port-Pin Output: SPI select
             21,               //Port-Pin Output: Reset 
             39               //Port-Pin Input:  Busy
             );

void setup() 
{
  delay(1000);
  Serial.begin(115200);

  //lora.DebugPrint(true);

  int16_t ret = lora.begin(RF_FREQUENCY,              //frequency in Hz
                           TX_OUTPUT_POWER);          //tx power in dBm
  if (ret != ERR_NONE) while(1) {delay(1);}

  lora.LoRaConfig(LORA_SPREADING_FACTOR, 
                  LORA_BANDWIDTH, 
                  LORA_CODINGRATE, 
                  LORA_PREAMBLE_LENGTH, 
                  LORA_PAYLOAD_LENGTH, 
                  true,               //crcOn  
                  false);             //invertIrq
}

void loop() 
{
  Serial.println("\nProvide input with the following structure:\n[Spreading factor (07-12)] [Bandwidth (2, 6)] [Coding rate (1, 4)] [String length (1, 3)]\nEx: 08 4 2 2");
  String dataIn = Serial.readStringUntil('\n');
  while (dataIn == "") {
    dataIn = Serial.readStringUntil('\n');
  }

  int i = ((dataIn[0] - 48) * 10 + (dataIn[1] - 48));
  int j = (dataIn[3] - 48);
  int k = (dataIn[5] - 48);
  int m = (dataIn[7] - 48);

  lora.LoRaConfig(i, 
                  j, 
                  k, 
                  LORA_PREAMBLE_LENGTH, 
                  LORA_PAYLOAD_LENGTH, 
                  true,               //crcOn  
                  false);             //invertIrq
  
  uint8_t txData[255];
  if (m == 0) {
    sprintf((char *)txData, shortString, i, j, k);
  }
  else if (m == 1) {
    sprintf((char *)txData, medString, i, j, k);
  }
  else {
    sprintf((char *)txData, longString, i, j, k);
  }

  for (int p = 0; p < transmissionsPerSetting; p++){
    uint8_t len = strlen((char *)txData);
    // Serial.println();
    // for(int n = 0; n < len; n++) {
    //   if (txData[n] > 0x1F && txData[n] < 0x7F) {
    //     char myChar = txData[n];
    //     Serial.print(myChar);
    //   } else {
    //     Serial.print("?");
    //   }
    // }
    // Serial.println();

    // Wait for transmission to complete
    if (lora.Send(txData, len, SX126x_TXMODE_SYNC)) {
      Serial.println("Send success");
    } else {
      Serial.println("Send fail");
      break;
    }
    delay(1);

    // Don't wait to finish
    // lora.Send(txData, len, SX126x_TXMODE_ASYNC);
    // delay(100);
  }
}