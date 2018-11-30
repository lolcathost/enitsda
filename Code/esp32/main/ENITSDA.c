#include <stdio.h>
#include <string.h>

#include "nvs_flash.h"
#include "esp_system.h"
#include "ENITSDA_vars.h"
#include "ENITSDA_wifi.h"
#include "ENITSDA_https.h"
#include "esp_log.h"

void app_main(void){
	int cuenta_envio_datos=0;
    nvs_flash_init();
	wifi_inicializar();
	wifi_promiscuo();

	while(true){
		if(canal_seleccionado > canal_maximo){
				if(cuenta_envio_datos >= 5){
					wifi_conectar_ap();
					envio_https();
					esp_wifi_stop();
					wifi_promiscuo();
					cuenta_envio_datos=0;
				}
				canal_seleccionado=1;
				cuenta_envio_datos++;
				printf("CUENTA: %d MACs WIFI RECOGIDAS: %d\n",cuenta_envio_datos,mac_wifi_cuenta);
				printf("HEAP: %d\n", esp_get_free_heap_size());
				printf("STACK: %d\n", uxTaskGetStackHighWaterMark(NULL));
		}
		printf("Canal: %d\n", canal_seleccionado);
		esp_wifi_set_channel(canal_seleccionado, WIFI_SECOND_CHAN_NONE);
		vTaskDelay(500 / portTICK_PERIOD_MS);	
		canal_seleccionado++;
	}
}