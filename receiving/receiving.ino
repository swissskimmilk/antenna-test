#include <Ra01S.h>

//#define RF_FREQUENCY                                433000000 // Hz  center frequency
//#define RF_FREQUENCY                                866000000 // Hz  center frequency
#define RF_FREQUENCY                                915000000 // Hz  center frequency
#define TX_OUTPUT_POWER                             22        // dBm tx output power
#define LORA_SPREADING_FACTOR                       7         // spreading factor [SF5..SF12]
#define LORA_BANDWIDTH                              4         // bandwidth
                                                              // 2: 31.25Khz
                                                              // 3: 62.5Khz
                                                              // 4: 125Khz
                                      ` ``  `     `1  ` ``  `   `         ``        ` ` ` `   ` `   `1  ``1           `1    1 `z  z1    ``1   `   1   1 ``    ``        `   ` ` ` ``    ``    ` ``  `     ` ``            `                            // 5: 250KHZ
                                                              // 6: 500Khz 
#define LORA_CODINGRATE                             1         // [1: 4/5,
                                                              //  2: 4/6,
                                                              //  3: 4/7,
                                                              //  4: 4/8]

#define LORA_PREAMBLE_LENGTH                        8         // Same for Tx and Rx
#define LORA_PAYLOAD_LENGTH                          0         // 0: Variable length packet (explicit header)
                                                              // 1..255  Fixed length packet (implicit header)
SX126x  lora(15,               //Port-Pin Output: SPI select
             21,               //Port-Pin Output: Reset 
             39               //Port-Pin Input:  Busy
             );

void setup() 
{
  delay(1000);
  Serial.begin(115200);
  Serial.setTimeout(1);

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

int numTransmissions = 0;
int currSize;
long int t0 = 0;

void loop() {
  // put your main code here, to run repeatedly:
  uint8_t rxData[255];
  uint8_t rxLen = lora.Receive(rxData, 255);

  if ( rxLen > 0 )
  {
    Serial.print("Receive rxLen:");
    Serial.println(rxLen);
    if (numTransmissions == 0) {
      t0 = millis();
    }
    numTransmissions++;
    currSize = rxLen;
    
    // for(int i=0;i< rxLen;i++) {
    //   Serial.print(rxData[i], HEX);
    //   Serial.print(" ");
    // }
    Serial.println();
    for(int i=0;i< rxLen;i++) {
      if (rxData[i] > 0x1F && rxData[i] < 0x7F) {
        char myChar = rxData[i];
        Serial.print(myChar);
      } else {
        Serial.print("?");
      }
    }
    Serial.println();

    int8_t rssi, snr;
    lora.GetPacketStatus(&rssi, &snr);
    Serial.print("rssi: ");
    Serial.print(rssi, DEC);
    Serial.println(" dBm");
    Serial.print("snr: ");
    Serial.print(snr, DEC);
    Serial.println(" dB");
    
    if (numTransmissions == 100) {
      long int t1 = millis();
      float dataRate = (numTransmissions * currSize * 8) / ((t1 - t0) / 1000);
      Serial.print(dataRate); Serial.println(" bits per second");
      numTransmissions = 0;
    }
    delay(1);
  }
  else {
    String dataIn = Serial.readStringUntil('\n');
    if (dataIn != "") {
      int i = ((dataIn[0] - 48) * 10 + (dataIn[1] - 48));
      int j = (dataIn[3] - 48);
      lora.LoRaConfig(i, 
                      j, 
                      LORA_CODINGRATE, 
                      LORA_PREAMBLE_LENGTH, 
                      LORA_PAYLOAD_LENGTH, 
                      true,               //crcOn  
                      false);             //invertIrq
      Serial.println("Reconfig done");
    }
  }
}
