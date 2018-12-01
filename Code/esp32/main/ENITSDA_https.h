// Basado en el ejemplo "https_request" de espressif
#include "esp_tls.h"

extern const uint8_t server_root_cert_pem_start[] asm("_binary_server_root_cert_pem_start");
extern const uint8_t server_root_cert_pem_end[]   asm("_binary_server_root_cert_pem_end");

/* Ligeramente modificada del https_request:
 - eliminados parte logs
 - restructurada funcion rtos para que tenga timeout y resetee el BIT WIFI_CONNECTED_BIT
 - quitado bucle del ejemplo
 - añadida envio de la DB de MACs por cada sesion TLS
 - eliminados goto del codigo de ejemplo
 */
 
void envio_https(void){
	size_t written_bytes;
	int ret;
	char REQUEST[512];
	char JSON[128];
	char mac_cadena[12];
	int *p=NULL;
	int https_cuenta=0,r;
	
	EventBits_t uxBits;
    uxBits=xEventGroupWaitBits(s_wifi_event_group, WIFI_CONNECTED_BIT, true, true, 10000/portTICK_PERIOD_MS);
	if(uxBits == 0){
		printf("No hemos podido conectar a la wifi, salimos\n");
		return;
	}
	
	esp_tls_cfg_t cfg_tls={
		.cacert_pem_buf  = server_root_cert_pem_start,
		.cacert_pem_bytes = server_root_cert_pem_end - server_root_cert_pem_start,
	};
	printf("HEAP: %d\n", esp_get_free_heap_size());
	printf("STACK: %d\n", uxTaskGetStackHighWaterMark(NULL));	
	struct esp_tls *tls = esp_tls_conn_http_new(WEB_URL, &cfg_tls);

	if(tls != NULL) {
		printf("Connection established...\n");
	}else{
		printf("Connection failed...\n");
		esp_tls_conn_delete(tls);    
		return;
	}
	
	// Envio de MACS adaptado. Se envian todas las MACS recolectadas al servidor en una sesion TLS.
	if (TIPO_ESCANEO == ESCANEO_WIFI){
		https_cuenta=mac_wifi_cuenta;
	}else if(TIPO_ESCANEO == ESCANEO_BT){
		https_cuenta=mac_bt_cuenta;
	}

	for(int i=0;i<https_cuenta;i++){
		if (TIPO_ESCANEO == ESCANEO_WIFI){
			*p=mac_wifi_listado[i].mac;
			r=mac_wifi_listado[i].rssi;
		}else if(TIPO_ESCANEO == ESCANEO_BT){
			*p=mac_bt_listado[i].mac;
			r=mac_bt_listado[i].rssi;
		}
		snprintf(mac_cadena,sizeof(mac_cadena),"%02x%02x%02x%02x%02x%02x",p[0],p[1],p[2],p[3],p[4],p[5]);
		snprintf(JSON,sizeof(JSON),"{\"mac\":\"%s\",\"nodo\":\"%s\",\"tipo\":\"%d\",\"rssi\":\"%d\"}\r\n",mac_cadena,ESP_NODO,TIPO_ESCANEO,r);
		snprintf(REQUEST, sizeof(REQUEST), "POST %s HTTP/1.1\r\n"
			"Host: %s\r\n"
			"User-Agent: esp-idf/1.0 esp32\r\n"
			"Authorization: Token %s\r\n" 
			"Content-Type: application/json\r\n"
			"Accept: application/json\r\n"
			"Content-Length: %d\r\n"
			"\r\n"
			"%s"	
			"\r\n",WEB_URL,WEB_SERVER,TOKEN,strlen(JSON)-2,JSON);
			
		written_bytes = 0;
		printf("%s\n",REQUEST);
		/*
		do{
			ret = esp_tls_conn_write(tls, 
			REQUEST + written_bytes, 
			strlen(REQUEST) - written_bytes);
			if (ret >= 0) {
				written_bytes += ret;
			} else if (ret != MBEDTLS_ERR_SSL_WANT_READ  && ret != MBEDTLS_ERR_SSL_WANT_WRITE) {
				printf("esp_tls_conn_write  returned 0x%x\n", ret);
				esp_tls_conn_delete(tls);    
				return;
			}
		} while(written_bytes < strlen(REQUEST));
		*/
	}
	printf("Reseteo de variables de conteo wifi y bt\n");
	mac_wifi_cuenta=0;
	mac_bt_cuenta=0;
	
	printf("Cerramos TLS.\n");
	esp_tls_conn_delete(tls);    
}
