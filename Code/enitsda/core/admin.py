from django.contrib import admin
from .models import Dispositivo, Nodo, Alerta, Receptor, Blacklist
from django.utils.safestring import mark_safe
from django.utils.html import format_html_join
import string

class MACAdmin(admin.ModelAdmin):
    list_display = ('mac', 'nodo_alias', 'tipo', 'deteccion', 'rssi', 'deauth', 'procesado')
    search_fields = ['mac']
    list_filter = ['nodo__alias', 'deauth', 'procesado']
    list_per_page = 25
    def nodo_alias(self, obj):
        return obj.nodo.alias

class NODOAdmin(admin.ModelAdmin):
    list_display = ('id', 'mac', 'alias', 'app', 'comentario')
    list_per_page = 25

class ALERTAdmin(admin.ModelAdmin):
    list_display = ('mac', 'fecha', 'counter')
    list_per_page = 25

class RECAdmin(admin.ModelAdmin):
    list_display = ('nombre', 'contact_id', 'status')
    list_per_page = 25

class PERAdmin(admin.ModelAdmin):
    list_display = ('mac', 'alias')
    list_per_page = 25
    def queryset(self, request):
        qs = super(PERAdmin, self).queryset(request)
        qs = qs.distinct('PERAdmin')
        return qs

admin.site.register(Dispositivo, MACAdmin)
admin.site.register(Nodo, NODOAdmin)
admin.site.register(Alerta, ALERTAdmin)
admin.site.register(Receptor, RECAdmin)
admin.site.register(Blacklist, PERAdmin)
admin.site.site_header = "ENITSDA"
admin.site.site_title = "ENITSDA"
admin.site.index_title = "Panel de herramientas"
