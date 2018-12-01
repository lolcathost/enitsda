from __future__ import absolute_import
from celery import shared_task
import requests
from setconfig import *
from core.models import Nodo, Dispositivo, Alerta, Blacklist, Receptor

def get_url(url):
    response = requests.get(url)
    content = response.content.decode("utf8")
    return content

def alerta(badguy, nodo):
	for guy in Receptor.objects.filter(status=True):
		text = "Detectada presencia de " + str(badguy) + " por " + str(nodo)
		url = URL + "sendMessage?text={}&chat_id={}".format(text, guy.contact_id)	
		get_url(url)

@shared_task
def vigilant():
    for e in Dispositivo.objects.filter(procesado=False):
        for p in Blacklist.objects.filter(mac=e.mac):
            r = Nodo.objects.get(mac=e.nodo)
            z = Blacklist.objects.get(mac=e.mac)
            alerta(z.alias, r.alias)
        e.procesado = True
        e.save()
