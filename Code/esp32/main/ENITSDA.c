#include <stdio.h>
#include <string.h>

#include "nvs_flash.h"
#include "esp_system.h"
#include "ENITSDA_vars.h"
#include "ENITSDA_wifi.h"
#include "ENITSDA_https.h"
#include "esp_log.h"

void mac_mostrar_listados(void){
	for(int i=0;i<mac_wifi_cuenta;i++){
		int *p=mac_wifi_listado[i].mac;
		printf("W -> %02x:%02x:%02x:%02x:%02x:%02x  RSSI -> %d\n",p[0],p[1],p[2],p[3],p[4],p[5],mac_wifi_listado[i].rssi);	
	}
	printf("# CONOCIDOS WF:[%d] BT:[%d]\n",mac_wifi_cuenta, mac_bt_cuenta);
}

int cuenta_prueba=0;

void app_main(){
    nvs_flash_init();
	wifi_inicializar();
	wifi_promiscuo();
	
	while(true){
		if(canal_seleccionado > canal_maximo){
				mac_mostrar_listados();
				wifi_conectar_ap();
				envio_https();
				esp_wifi_stop();
				wifi_promiscuo();
				canal_seleccionado=1;
				cuenta_prueba++;
				printf("=============%d\n",cuenta_prueba);
		}
		printf("Canal: %d\n", canal_seleccionado);
		esp_wifi_set_channel(canal_seleccionado, WIFI_SECOND_CHAN_NONE);
		vTaskDelay(500 / portTICK_PERIOD_MS);	
		canal_seleccionado++;
	}
}
