// Basado en el ejemplo "https_request" de espressif

#include "esp_tls.h"

extern const uint8_t server_root_cert_pem_start[] asm("_binary_server_root_cert_pem_start");
extern const uint8_t server_root_cert_pem_end[]   asm("_binary_server_root_cert_pem_end");

// Por modificar, sera POST con TOKEN y hay que meterlo con la DB de las mac wifi
static const char *REQUEST="POST "WEB_URL" HTTP/1.1\r\n"
    "Host: "WEB_SERVER"\r\n"
    "User-Agent: esp-idf/1.0 esp32\r\n"
	"Authorization: Token "TOKEN"\r\n" 
	"Content-Type: application/json\r\n"
	"Accept: application/json\r\n"
	"Content-Length: 33\r\n"
	"\r\n"
    "{\"mac\":\"ffffffffffff\",\"nodo\":\"1\"}\r\n"	
	"\r\n";

/* Ligeramente modificada del https_request:
 - eliminados parte logs
 - restructurada funcion rtos para que tenga timeout y resetee el BIT WIFI_CONNECTED_BIT
 - quitado bucle del ejemplo
 */
 
void envio_https(void){
    char buf[512];
    int ret, len;
	printf("%s\n\n",REQUEST);
	EventBits_t uxBits;
    uxBits=xEventGroupWaitBits(s_wifi_event_group, WIFI_CONNECTED_BIT, true, true, 3000/portTICK_PERIOD_MS);
	if(uxBits == 0){
		printf("No hemos podido conectar a la wifi, salimos\n");
		goto exit_evento;	
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
		goto exit;
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
			goto exit;
		}
	} while(written_bytes < strlen(REQUEST));
	
	exit:
	esp_tls_conn_delete(tls);    
	putchar('\n'); // JSON output doesn't have a newline at end
	
	exit_evento: ;
	printf("Salimos del envio_https\n");
}
