#include <Arduino.h>
#include <SPI.h>
#include <avr/sleep.h>
#include <RH_RF95.h>
#include <avr/wdt.h>

#define RFM95_CS 10
#define RFM95_RST 9
#define RFM95_INT 2

// Change to 434.0 or other frequency, must match RX's freq!
#define RF95_FREQ 868.0
#define ID 1
//define led
#define LED 8  //Nano LED
// Singleton instance of the radio driver
RH_RF95 rf95(RFM95_CS, RFM95_INT);

void setup() {
  pinMode(LED, OUTPUT);

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

void flashLED()
{
  // Flash the LED 
 for (byte i = 0; i < 10; i++)
    {
    digitalWrite(LED, HIGH);
    delay(50);
    digitalWrite(LED, LOW);
    delay(50);
    }
 } 

void sleep()
 {
  //Sleep RFM95
  rf95.sleep();
  // disable ADC
  //ADCSRA = 0;  

  // clear all MCUSR flags
  MCUSR = 0;     
  WDTCSR |= 0b00011000;               // see docs, set WDCE, WDE
  WDTCSR =  0b01000000 | 0b100001;    // set WDIE, and appropriate delay
  wdt_reset();  // Start the timer

  set_sleep_mode (SLEEP_MODE_PWR_DOWN);  
  noInterrupts();           // timed sequence follows
  sleep_enable();

  // turn off brown-out detector
  MCUCR = bit (BODS) | bit (BODSE);
  MCUCR = bit (BODS); 
  interrupts();
  sleep_cpu(); 

   // Sleep here in powerdown state
  // Return here after WDT return from ISR 

  // disable sleep
  sleep_disable();
  Serial.flush();
  //ADCSRA = 1;
 }

float* sampleADC()
{
  // read the input on analog pin 0:
  int sensorValue = analogRead(A0);
  sensorValue = analogRead(A0);
  // Convert the analog reading (which goes from 0 - 1023) to a voltage (0 - 5V):
  float v0 = sensorValue * (3.3 / 1023.0);
  // print out the value you read:
  //Serial.println(voltage);
  delay(500);
  int sensorValue1 = analogRead(A1);
  sensorValue1 = analogRead(A1);
  // Convert the analog reading (which goes from 0 - 1023) to a voltage (0 - 5V):
  float v1 = sensorValue1 * (3.3 / 1023.0);
  // print out the value you read:
  //Serial.println(voltage1);
  float v [2];
  v[0] = v0;
  //Serial.println(v0);
  v[1] = v1;
  return v;
}

// Watchdog ISR routine
ISR(WDT_vect) 
{
   wdt_disable();  // disable watchdog
   // Return from ISR
}  


uint8_t buf[RH_RF95_MAX_MESSAGE_LEN];    
uint8_t len = sizeof(buf);

void loop() 
{

  flashLED();
  
  
  //float* v = sampleADC();
  //uint8_t v0 = v[0];
  //uint8_t v1 = v[1];
  //ADCSRA = 1;
  delay(1000);
  int sensorValue0 = analogRead(A0);
  sensorValue0 = analogRead(A0);
  float v0 = sensorValue0 * (3.3 / 1023.0);
  delay(1000);
  int sensorValue1 = analogRead(A1);
  sensorValue1 = analogRead(A1);
  float v1 = sensorValue1 * (3.3 / 1023.0);
  Serial.println(v0);
  Serial.println(v1);
  if (1){
    uint8_t rssi = -rf95.lastRssi();
    uint8_t data[] = {ID, rssi, 100*v0, 100*v1};
    Serial.println(rssi);
    rf95.send(data, 13); //sizeof(data)
    rf95.waitPacketSent();
    int count = 0;
    while (!(rf95.recv(buf, &len)) & (count <100000) ){
      count ++;
      //if(rf95.recv(buf, &len))
      //{
       // Serial.println("ack received");
     // }
    }
  }
  
  for (int i=0; i<99; i++)
  {
      sleep();
  }

  }
// sleep bit patterns:
//  1 second:  0b000110
//  2 seconds: 0b000111
//  4 seconds: 0b100000
//  8 seconds: 0b100001