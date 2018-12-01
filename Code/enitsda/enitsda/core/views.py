from django.shortcuts import render
from rest_framework.permissions import IsAuthenticated
from rest_framework.generics import ListCreateAPIView, RetrieveUpdateDestroyAPIView
from core.models import Dispositivo
from core.serializers import DeviceSerializer
from core.tasks import vigilant
class DeviceListCreateAPIView(ListCreateAPIView):
    serializer_class = DeviceSerializer
    def get_queryset(self):
        return Dispositivo.objects.filter()

    def perform_create(self, serializer):
        serializer.save()
        vigilant()

class DeviceDetailAPIView(RetrieveUpdateDestroyAPIView):
    serializer_class = DeviceSerializer
    queryset = Dispositivo.objects.all()
    permission_classes = (IsAuthenticated,)
