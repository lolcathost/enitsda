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

def SendData(PARAMS):
	try:
                r = requests.post(url = URL, headers={'Authorization': 'Token {}'.format(TOKEN)}, json = PARAMS)
	except AttributeError:
		pass

def time_diff(time_start, time_end):
    start = datetime.strptime(time_start, "%H%M%S")
    end = datetime.strptime(time_end, "%H%M%S")
    difference = end - start
    seconds = difference.total_seconds()
    return int(seconds)

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
                        mac = pkt.addr1
                        mac_n = re.sub('[:]', '', mac)
                        if mac_n == macdev: 
                            print(mac_n)
                            DATA = {'mac':mac_n, 
			            'nodo':'2',   
			    	    'tipo':'1',
                                    'deauth': True,}
                            SendData(DATA)

def Detect(device):
            sniff(iface=device, prn=PacketDet, store=0)

if __name__ == "__main__":
        Thread(target = Detect(device)).start()
