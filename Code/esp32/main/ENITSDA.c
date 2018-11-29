#include <stdio.h>
#include <string.h>
#include "nvs_flash.h"
#include "esp_system.h"
#include "ENITSDA_vars.h"
#include "ENITSDA_https.h"
#include "ENITSDA_wifi.h"

void app_main(){
	nvs_flash_init();
	tcpip_adapter_init();
	wifi_inicializar();
	wifi_promiscuo();
	
	while(true){
		if(canal_seleccionado > canal_maximo){
				mac_mostrar_listados();
				canal_seleccionado=1;
		}
		printf("Canal: %d\n", canal_seleccionado);
		esp_wifi_set_channel(canal_seleccionado, WIFI_SECOND_CHAN_NONE);
		vTaskDelay(500 / portTICK_PERIOD_MS);	
		canal_seleccionado++;
	}
}
