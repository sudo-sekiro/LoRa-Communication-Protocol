# LoRa-Communication-Protocol
Communciation Protocol and testing scripts for RFM95 LoRa module using ATMEGA328P MCU and Raspberry Pi.

ATMEGA client and Test_SERVER.py allow for communication between atmega328 or arduino uno and raspberry pi. The pi sends out a request for information which the client responds to. The server (pi) then sends an Ack.
An encrypted version of the server and client are also included.

Range test server and Atmega client work in a similar way with the server taking keyboard inputs to record the distance between modules and then recording the distance, rssi and packet loss over 10 messages requested.

The sensor module transmitter samples the ATMEGA ADC and then transmits a message every 15 minutes, going into power down mode in between. The stability test server listens for these messages and records the sensor data along with rssi and timestamp before sending an ack.

The mesh script is used to create a routing table between multiple ATMEGA nodes with different IDs.

The Time Test scripts are used to time the duration of messages sent and received. 

rpsreal, Based on RadioHead by Airspayce, was used to program the LoRa module.

The AES library was used for encryption. This also depends on base64 for encoding binary into base64 strings and the reverse operation.

Raspberry Pi Dependencies:
rpsreal and Radiohead
https://github.com/rpsreal/pySX127x

spidev
https://pypi.org/project/spidev/

Can install rpi dependencies using:

sudo apt-get install python-rpi.gpio python3-rpi.gpio
sudo apt-get install python-spidev python3-spidev
sudo apt-get install git
sudo git clone https://github.com/rpsreal/pySX127x

ATMEGA Dependencies:
radiohead
https://www.airspayce.com/mikem/arduino/RadioHead/

rpsreal
https://github.com/rpsreal/LoRa_Ra-02_Arduino

AES
https://github.com/DavyLandman/AESLib

base64
https://github.com/adamvr/arduino-base64
