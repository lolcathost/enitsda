// Basado en el ejemplo "https_request" de espressif

#include "esp_tls.h"

extern const uint8_t server_root_cert_pem_start[] asm("_binary_server_root_cert_pem_start");
extern const uint8_t server_root_cert_pem_end[]   asm("_binary_server_root_cert_pem_end");

// Por modificar, sera POST con TOKEN (esta es la referencia inicial del ejemplo)
static const char *REQUEST = "GET " WEB_URL " HTTP/1.0\r\n"
    "Host: "WEB_SERVER"\r\n"
    "User-Agent: esp-idf/1.0 esp32\r\n"
    "\r\n";

/* Ligeramente modificada del https_request:
 - eliminados parte logs
 - restructurada funcion rtos para que tenga timeout y resetee el BIT WIFI_CONNECTED_BIT
 - quitado bucle original inecesario y gotos
 */
 
void envio_https(void){
    char buf[512];
    int ret, len;
	
	EventBits_t uxBits;
    uxBits=xEventGroupWaitBits(s_wifi_event_group, WIFI_CONNECTED_BIT, true, true, 3000/portTICK_PERIOD_MS);
	if(uxBits == 0){
		printf("No hemos podido conectar a la wifi, salimos\n");
		return; 	
	}
	
	esp_tls_cfg_t cfg_tls={
		.cacert_pem_buf  = server_root_cert_pem_start,
		.cacert_pem_bytes = server_root_cert_pem_end - server_root_cert_pem_start,
	};
	printf("URL: %s, CFG bytes: %d\n", WEB_URL, cfg_tls.cacert_pem_bytes);
	struct esp_tls *tls = esp_tls_conn_http_new(WEB_URL, &cfg_tls);

	if(tls != NULL) {
		printf("Connection established...\n");
	}else{
		printf("Connection failed...\n");
		return;
	}
	
	size_t written_bytes = 0;
	do{
		ret = esp_tls_conn_write(tls, 
								 REQUEST + written_bytes, 
								 strlen(REQUEST) - written_bytes);
		if (ret >= 0) {
			written_bytes += ret;
		} else if (ret != MBEDTLS_ERR_SSL_WANT_READ  && ret != MBEDTLS_ERR_SSL_WANT_WRITE) {
			printf("esp_tls_conn_write  returned 0x%x\n", ret);
			return;
		}
	} while(written_bytes < strlen(REQUEST));

	printf("Reading HTTP response...\n");

	do{
		len = sizeof(buf) - 1;
		bzero(buf, sizeof(buf));
		ret = esp_tls_conn_read(tls, (char *)buf, len);
		
		if(ret == MBEDTLS_ERR_SSL_WANT_WRITE  || ret == MBEDTLS_ERR_SSL_WANT_READ)
			continue;
		
		if(ret < 0){
			printf("esp_tls_conn_read  returned -0x%x\n", -ret);
			break;
		}

		if(ret == 0){
			printf("connection closed\n");
			break;
		}

		len = ret;
		printf("%d bytes read\n", len);

		for(int i = 0; i < len; i++) {
			putchar(buf[i]);
		}
	} while(1);

	esp_tls_conn_delete(tls);    
	putchar('\n'); // JSON output doesn't have a newline at end
}
