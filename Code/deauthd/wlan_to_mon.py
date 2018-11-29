#!/usr/bin/env python
# -*- coding: utf-8 -*-​
import os, time, socket, fcntl, struct
from subprocess import call
from platform import system
from sys import argv

device = 'wlan1'

def InitMon(wiface):
	if not os.path.isdir("/sys/class/net/" + wiface):
		#print "WiFi interface %s does not exist! Cannot continue!" %(wiface)
		exit(1)
	else:
		try:
			os.system("ip link set %s down" %wiface)
			time.sleep(1)
			os.system("iwconfig %s mode monitor" %wiface)
			time.sleep(1)
			os.system("ip link set %s up" %wiface)
		except OSError as e:
			#print "Error creating monitor with %s" %wiface
			os.kill(os.getpid(),SIGINT)
			sys.exit(1)

# Check for root privileges
if os.geteuid() != 0:
	exit("You need to be root to run this script!")

InitMon(device);
