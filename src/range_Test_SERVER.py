#!/usr/bin/env python3

""" This program asks a client for data and waits for the response, then sends an ACK. """


# This code uses the rpsreal/pySX127x, fork of mayeranalytics/pySX127x library.


import time
from SX127x.LoRa import *
#from SX127x.LoRaArgumentParser import LoRaArgumentParser
from SX127x.board_config import BOARD

BOARD.setup()
BOARD.reset()
#parser = LoRaArgumentParser("Lora tester")
timeOut = 0
currentRssi = ""

class mylora(LoRa):
    def __init__(self, verbose=False):
        super(mylora, self).__init__(verbose)
        self.set_mode(MODE.SLEEP)
        self.set_dio_mapping([0] * 6)
        self.var=0
	self.set_freq(868)
	print(self.get_freq())

    def on_rx_done(self):
        BOARD.led_on()
        global currentRssi
        #print("\nRxDone")
        self.clear_irq_flags(RxDone=1)
        payload = self.read_payload(nocheck=True)
        print ("Receive: ")
        msg = bytes(payload).decode("utf-8",'ignore')# Receive DATA
	print(msg)
        BOARD.led_off()
	
        time.sleep(3) # Wait for the client be ready
        print ("Send: ACK")
        self.write_payload([255, 255, 0, 0, 65, 67, 75, 0]) # Send ACK
        self.set_mode(MODE.TX)
        self.var=1
        out.write(currentRssi + ", ")
        out.write(str(self.get_rssi_value()) + ", ")
        out.write(str(-payload[8]))
        #out.write(", ")

    def on_tx_done(self):
        print("\nTxDone")
        print(self.get_irq_flags())

    def on_cad_done(self):
        print("\non_CadDone")
        print(self.get_irq_flags())

    def on_rx_timeout(self):
        print("\non_RxTimeout")
        print(self.get_irq_flags())

    def on_valid_header(self):
        print("\non_ValidHeader")
        print(self.get_irq_flags())

    def on_payload_crc_error(self):
        print("\non_PayloadCrcError")
        print(self.get_irq_flags())

    def on_fhss_change_channel(self):
        print("\non_FhssChangeChannel")
        print(self.get_irq_flags())

    def write(self):
        print("write")

    def start(self):
        global timeOut
        global currentRssi
        timeOut -=1
        if(1):
            self.write()
            while (self.var==0):
                timeOut+=1
                print ("Send: INF")
                self.write_payload([255, 255, 0, 0, 73, 78, 70, 0]) # Send INF
                self.set_mode(MODE.TX)
                time.sleep(3) # there must be a better solution but sleep() works
                self.reset_ptr_rx()
                self.set_mode(MODE.RXCONT) # Receiver mode
                currentRssi = str(self.get_rssi_value())
               #out.write(str(self.get_rssi_value()))
               #out.write(", ")
                start_time = time.time()
                while (time.time() - start_time < 10): # wait until receive data or 10s
                    pass;
            
            self.var=0
            self.reset_ptr_rx()
            self.set_mode(MODE.RXCONT) # Receiver mode
            time.sleep(3)

lora = mylora(verbose=False)
#args = parser.parse_args(lora) # configs in LoRaArgumentParser.py

#     Slow+long range  Bw = 125 kHz, Cr = 4/8, Sf = 4096chips/symbol, CRC on. 13 dBm
lora.get_freq()
lora.set_freq(868)
lora.set_pa_config(pa_select=1, max_power=23, output_power=23)
lora.set_bw(BW.BW125)
lora.set_coding_rate(CODING_RATE.CR4_8)
lora.set_spreading_factor(12)
lora.set_rx_crc(True)
#lora.set_lna_gain(GAIN.G1)
#lora.set_implicit_header_mode(False)
lora.set_low_data_rate_optim(True)

#  Medium Range  Defaults after init are 434.0MHz, Bw = 125 kHz, Cr = 4/5, Sf = 128chips/symbol, CRC on 13 dBm
#lora.set_pa_config(pa_select=1)h

out = open("testData.txt","a")


assert(lora.get_agc_auto_on() == 1)

try:
    print("START")
    while(1):
        distance = eval(str(input("Enter Distance: " )))
        print(distance)
        out.write("\n" + str(distance))
        for i in range(5):
            out.write(", ")
            lora.start()
        print(timeOut)
        out.write(", ")
        out.write(str(timeOut))
        timeOut = 0
        out.write("\n")
        print("written")
except KeyboardInterrupt:
    sys.stdout.flush()
    print("Exit")
    sys.stderr.write("KeyboardInterrupt\n")
finally:
    sys.stdout.flush()
    print("Exit")
    out.close()
    lora.set_mode(MODE.SLEEP)
    BOARD.teardown()
