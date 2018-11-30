#include <stdio.h>
#include <string.h>

#include "nvs_flash.h"
#include "esp_system.h"
#include "ENITSDA_vars.h"
#include "ENITSDA_wifi.h"
#include "ENITSDA_https.h"
#include "esp_log.h"

int cuenta_prueba=0;

void app_main(void){
    nvs_flash_init();
	wifi_inicializar();
	wifi_promiscuo();

	while(true){
		if(canal_seleccionado > canal_maximo){
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