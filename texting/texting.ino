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
                                                              // 5: 250KHZ
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

void loop() {
  // put your main code here, to run repeatedly:
  uint8_t rxData[255];
  uint8_t txData[255];
  uint8_t rxLen = lora.Receive(rxData, 255);

  if ( rxLen > 0 )
  {
    // Serial.println(rxLen);
    
    // for(int i=0;i< rxLen;i++) {
    //   Serial.print(rxData[i], HEX);
    //   Serial.print(" ");
    // }
    Serial.println();
    Serial.print("Received: ");
    for(int i=0;i< rxLen;i++) {
      if (rxData[i] > 0x1F && rxData[i] < 0x7F) {
        char myChar = rxData[i];
        Serial.print(myChar);
      } else {
        Serial.print("?");
      }
    }
    Serial.println();
    delay(1);
  }
  else {
    String dataIn = Serial.readStringUntil('\n');
    if (dataIn != "") {
      
      for (int i=0; i < dataIn.length() - 1; i++)
        txData[i] = dataIn[i];
      // sprintf((char *)txData, dataIn);
      int len = dataIn.length();
      Serial.print("ASCII transmitted: ");
      for (int i = 0; i < len; i++) {
        Serial.print(txData[i]);
        Serial.print(" ");
      }
      Serial.println();
      if (lora.Send(txData, len, SX126x_TXMODE_SYNC)) {
        Serial.println("Send success");
      } else {
        Serial.println("Send fail");
      }
    }
  }
}
