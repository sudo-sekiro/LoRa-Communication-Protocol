#include <Arduino.h>
#include <SPI.h>
#include <avr/sleep.h>
#include <RH_RF95.h>

#define RFM95_CS 10
#define RFM95_RST 9
#define RFM95_INT 2

// Change to 434.0 or other frequency, must match RX's freq!
#define RF95_FREQ 868.0
#define ID 1

// Singleton instance of the radio driver
RH_RF95 rf95(RFM95_CS, RFM95_INT);

// Blinky on receipt
#define LED 8

void setup() 
{
  pinMode(LED, OUTPUT);     
  digitalWrite(LED, HIGH);
  pinMode(RFM95_RST, OUTPUT);
  digitalWrite(RFM95_RST, HIGH);

  while (!Serial);
  Serial.begin(9600);
  delay(100);
  
  // manual reset
  digitalWrite(RFM95_RST, LOW);
  delay(10);
  digitalWrite(RFM95_RST, HIGH);
  delay(10);

  while (!rf95.init()) {
    Serial.println("LoRa radio init failed");
    while (1);
  }

  // Defaults after init are 868.0MHz, modulation GFSK_Rb250Fd250, +13dbM
  if (!rf95.setFrequency(RF95_FREQ)) {
    Serial.println("setFrequency failed");
    while (1);
  }
  
  // The default transmitter power is 13dBm, using PA_BOOST.
  // If you are using RFM95/96/97/98 modules which uses the PA_BOOST transmitter pin, then 
  // you can set transmitter powers from 5 to 23 dBm:

  
  // Bw = 125 kHz, Cr = 4/8, Sf = 4096chips/symbol, CRC on. 
  // Slow+long range. 
  rf95.setModemConfig(RH_RF95::Bw125Cr48Sf4096);

  
  // Defaults after init are 434.0MHz, Bw = 125 kHz, Cr = 4/5, Sf = 128chips/symbol, CRC on
  // Medium Range
  
  rf95.setTxPower(23);


  
  Serial.println("START");
}
    
uint8_t buf[RH_RF95_MAX_MESSAGE_LEN];    
uint8_t len = sizeof(buf);

void loop() 
{  
  //digitalWrite(LED, HIGH);
  //uint8_t rTime = millis();
  
  if (rf95.available()){    
    //unsigned long rTime = millis();
    if (rf95.recv(buf, &len)){  
      unsigned long rTime = millis();
      //Serial.println((char*)buf);
      //Serial.println(buf[1]);
      unsigned long msb = buf[1];
      //Serial.println(buf[2]);
      unsigned long mid = buf[2];
     // Serial.println(buf[3]);
      unsigned long lsb = buf[3];
      Serial.print("Time Received: ");
      Serial.println(rTime);
      unsigned long tTime = lsb + (mid<<8) + (msb<<16);
      Serial.print("Time of Transmission: ");
      Serial.println(tTime);
      unsigned long pTime = rTime - tTime;
      Serial.print("Elapsed Time: ");
      Serial.println(pTime);
      
    }
  } 
}   
