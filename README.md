# LoRa-Communication-Protocol
Communciation Protocol and testing scripts for RFM95 LoRa module using ATMEGA328P MCU and RPI

rpsreal, based on RadioHead by Airspayce, was used to program the LoRa module.

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
