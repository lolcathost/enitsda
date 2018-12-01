from django.contrib import admin
from django.urls import path
from django.conf.urls.static import static
from django.conf import settings
from django.conf.urls import url, include
from rest_framework import routers
from core import views
from django.views.generic.base import TemplateView

api_urls = [
    path('devices/', include('core.urls')),
]

urlpatterns = [
    path('admin/', admin.site.urls),
    path('api/', include(api_urls)),
    path('', TemplateView.as_view(template_name='home.html')),
] 
