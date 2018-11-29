#!/usr/bin/env python
from scapy.all import *
from time import sleep
import requests
import threading
from threading import Thread

URL = "https://hackaton.hacklab.cx/device/"
conf.verbose=0
device = "wlan1"

def SendData(PARAMS):
	try:
		r = requests.post(url = URL, json = PARAMS)
		#print r.status_code
	except AttributeError:
		pass

# Detect deauth packets and report with SendData()
def PacketDet(pkt):
	if pkt.haslayer(Dot11Deauth):
		send = 0
		if 0 <= pkt.reason <= 9:
			reason = str(pkt.reason)
			send = 1
		elif pkt.sprintf("%pkt.reason%").startswith('class3-from-nonass'):
			reason = "10"
			send = 1
		if send == 1:
                        print(pkt.addr1) #from
			print(pkt.addr2) #to
			print(pkt.addr3) #dst
			print("===========")
                        #SendData()

def Detect(device):
            sniff(iface=device, prn=PacketDet, store=0)

# Modify iface to your monitor iface
#sniff(iface="wlan1", prn=PacketDet, store=False, lfilter=lambda pkt:Dot11 in pkt)

if __name__ == "__main__":
        Thread(target = Detect(device)).start()
