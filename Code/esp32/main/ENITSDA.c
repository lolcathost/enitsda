#include "ENITSDA.h"
#include "ENITSDA_vars.h"
#include "ENITSDA_wifi.h"
#include "ENITSDA_bt.h"
#include "ENITSDA_https.h"
 
void app_main(void){
	int cuenta_envio_datos=0;	
    nvs_flash_init();
	wifi_inicializar();	
	if (TIPO_ESCANEO == ESCANEO_WIFI){
		printf("Seleccionado escaneo WIFI.\n");
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
	}else if(TIPO_ESCANEO == ESCANEO_BT){
		printf("Seleccionado escaneo Bluetooth.\n");
		bt_inicializar();
		bt_escaneo();
					vTaskDelay(10000 / portTICK_PERIOD_MS);		
		wifi_conectar_ap();
		envio_https();
		esp_wifi_stop();
				vTaskDelay(10000 / portTICK_PERIOD_MS);		
		esp_restart();
	}else{
		printf("Variable TIPO_ESCANEO mal defeinida, solo puede ser 1 o 2: %d\n", TIPO_ESCANEO);
	}
}