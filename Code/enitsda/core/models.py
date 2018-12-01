from django.db import models
from datetime import datetime
from django.core.validators import RegexValidator

class Dispositivo(models.Model):
    WIFI = 1
    BT = 2
    TIPOS_T = (
        (WIFI, 'Wifi'),
        (BT, 'Bluetooth'),
    )
    mac = models.CharField(max_length=12, blank=False, null=False, verbose_name='MAC Address',
            validators=[
                RegexValidator(
                    regex='^[0-9a-fA-F]*$',
                    message='Valor MAC incorrecto'),])
    nodo = models.ForeignKey('nodo', on_delete=models.PROTECT)
    tipo = models.IntegerField(choices=TIPOS_T, default=1)
    rssi = models.IntegerField(blank=True, null=True)
    deteccion = models.DateTimeField(auto_now_add=True, blank=True)
    deauth = models.BooleanField(blank=False, null=False, default=False)
    procesado = models.BooleanField(blank=False, null=False, default=False)
    def __str__(self):
        return self.mac
    class Meta:
        indexes = [
            models.Index(fields=['mac'], name='mac_det_idx'),
        ]

class Nodo(models.Model):
    mac = models.CharField(max_length=12, blank=False, null=False, verbose_name='MAC Address',
	    validators=[
                RegexValidator(
                    regex='^[0-9a-fA-F]*$',
                    message='Valor MAC incorrecto',
        ),])
    alias = models.CharField(max_length=12, blank=False, null=False)
    app = models.FileField(blank=True, null=True, verbose_name=('Software image'))
    comentario = models.TextField(blank=True, null=True)
    def __str__(self):
        return self.mac

class Blacklist(models.Model):
    mac = models.CharField(max_length=12, blank=False, null=False, verbose_name='MAC Address',
            validators=[
                RegexValidator(
                    regex='^[0-9a-fA-F]*$',
                    message='Valor MAC incorrecto',
        ),
    ])
    alias = models.CharField(max_length=20, blank=False, null=False)
    def __str__(self):
        return self.mac
    class Meta:
        indexes = [
            models.Index(fields=['mac'], name='mac_blk_idx'),
        ]

class Alerta(models.Model):
    mac = models.CharField(max_length=12, blank=False, null=False, verbose_name='MAC Address',
            validators=[
                RegexValidator(
                    regex='^[0-9a-fA-F]*$',
                    message='Valor MAC incorrecto',
        ),])
    fecha = models.DateTimeField(default=datetime.now, blank=False, null=True)
    counter = models.IntegerField(blank=False, null=False)
    def __str__(self):
        return self.mac
    class Meta:
        indexes = [
            models.Index(fields=['mac'], name='mac_alert_idx'),
        ]

class Receptor(models.Model):
    nombre = models.CharField(max_length=20, blank=False, null=False)
    contact_id = models.CharField(max_length=10, blank=False, null=False)
    status = models.BooleanField(blank=False, null=False, default=True)
    def __str__(self):
        return self.nombre
    class Meta:
        verbose_name = 'Receptor'
        verbose_name_plural = 'Receptores'

