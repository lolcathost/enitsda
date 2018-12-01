from .models import Dispositivo
from rest_framework import serializers

class DeviceSerializer(serializers.ModelSerializer):
    class Meta:
        model = Dispositivo
        fields = ('mac', 'nodo', 'tipo', 'rssi', 'deauth', 'deteccion')
