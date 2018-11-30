#!/usr/bin/env python
from scapy.all import *
from time import sleep
import requests
import threading
from threading import Thread
import re
from datetime import datetime
from sys import argv
from setconfig import *

conf.verbose=0
URL = "https://hackaton.hacklab.cx/api/devices/"
(script, device, macdev) = argv
last_packet = time.time()

def SendData(PARAMS):
	try:
                r = requests.post(url = URL, headers={'Authorization': 'Token {}'.format(TOKEN)}, json = PARAMS)
	except AttributeError:
                print("Error en envio")
                return r

# Detect deauth packets and report with SendData()
def PacketDet(pkt):
        global last_packet
	if pkt.haslayer(Dot11Deauth):
                timestamp = time.time()
		send = 0
		if 0 <= pkt.reason <= 9:
			reason = str(pkt.reason)
			send = 1
		elif pkt.sprintf("%pkt.reason%").startswith('class3-from-nonass'):
			reason = "10"
			send = 1
		if send == 1:
                        mac = pkt.addr1
                        mac_n = re.sub('[:]', '', mac)
                        if mac_n == macdev: 
                            print(mac_n, timestamp, last_packet)
                            DATA = {'mac':mac_n,
			            'nodo':'2',  
			    	    'tipo':'1',
                                    'deauth': True,}
                            if timestamp > last_packet+2:
                                SendData(DATA)
                                last_packet = timestamp
             
def Detect(device):
            sniff(iface=device, prn=PacketDet, store=0)

if __name__ == "__main__":
        Thread(target = Detect(device)).start()
