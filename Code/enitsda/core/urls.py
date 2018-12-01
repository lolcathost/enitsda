from django.conf.urls import url
from django.urls import path
from core.views import DeviceListCreateAPIView, DeviceDetailAPIView

app_name = 'core'

urlpatterns = [
    path('', DeviceListCreateAPIView.as_view(), name="list"),
    path('<int:pk>/', DeviceDetailAPIView.as_view(), name="detail"),
]

