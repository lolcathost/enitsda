#include "esp_wifi.h"
#include "esp_event_loop.h"

#include "lwip/err.h"
#include "lwip/sockets.h"
#include "lwip/sys.h"
#include "lwip/netdb.h"
#include "lwip/dns.h"

#define IP      "192.168.5.2"
#define GW      "192.168.5.1"
#define NETMASK "255.255.255.0"
#define DNS		"192.168.5.1"

#define PAQUETE_CABECERA_TAM 24
#define PAQUETE_POSICION_MAC 16
int canal_seleccionado=1,canal_maximo=13;

// Filtro de paquetes WIFI: gestion y datos
const wifi_promiscuous_filter_t filtro={
	.filter_mask=WIFI_PROMIS_FILTER_MASK_MGMT|WIFI_PROMIS_FILTER_MASK_DATA
};

#define MAC_TOTAL_ARRAY 254
struct DB{
	int mac[6];
	signed int rssi;
}mac_wifi_listado[MAC_TOTAL_ARRAY];
//int mac_bt_listado[MAC_TOTAL_ARRAY][6];
int mac_wifi_cuenta=0,mac_bt_cuenta=0;


// Funcion de interrupcion por cada recepcion de un paquete
void sniffer(void* buf, wifi_promiscuous_pkt_type_t type){
	int mac_temp[6];
	int paquete_tam;
	int mac_existe=0;
	
	wifi_promiscuous_pkt_t *paquete=(wifi_promiscuous_pkt_t*)buf;
	paquete_tam = paquete->rx_ctrl.sig_len-PAQUETE_CABECERA_TAM;

	if (paquete_tam < 0){
		printf("Paquete sin informacion: %d\n",paquete_tam);
		return;
	}else{
		for(int i=0;i<=6;i++){
			mac_temp[i]=paquete->payload[PAQUETE_POSICION_MAC+i];
		}
		for(int i=0;i<=mac_wifi_cuenta;i++){
			if(memcmp(mac_temp,mac_wifi_listado[i].mac,sizeof(mac_temp)) == 0){
				mac_existe=1;
			}
		}
		if(mac_existe == 0){
			if(mac_wifi_cuenta >= MAC_TOTAL_ARRAY){
				printf("Superado el limite de MACs: %d\n", mac_wifi_cuenta);
				// Añadir bandera de saturacion de MAC, posible ataque.
			}else{
				memcpy(mac_wifi_listado[mac_wifi_cuenta].mac,mac_temp,sizeof(mac_temp));	
				mac_wifi_listado[mac_wifi_cuenta].rssi=paquete->rx_ctrl.rssi;
				mac_wifi_cuenta++;
			}
		}  
	}
}

void wifi_inicializar(void){
	tcpip_adapter_ip_info_t esp_ip;
	tcpip_adapter_dns_info_t dnsserver;

	esp_event_loop_init(NULL, NULL);
	wifi_init_config_t cfg = WIFI_INIT_CONFIG_DEFAULT();
    esp_wifi_init(&cfg);
    esp_wifi_set_storage(WIFI_STORAGE_RAM);
    esp_wifi_set_promiscuous_filter(&filtro);
    esp_wifi_set_promiscuous_rx_cb(&sniffer);
    esp_wifi_set_ps(WIFI_PS_NONE);

	tcpip_adapter_init();
	tcpip_adapter_dhcpc_stop(TCPIP_ADAPTER_IF_STA); 
	inet_pton(AF_INET, IP, &esp_ip.ip);
	inet_pton(AF_INET, GW, &esp_ip.gw);
	inet_pton(AF_INET, NETMASK, &esp_ip.netmask);
	inet_pton(AF_INET, DNS, &dnsserver.ip);
	tcpip_adapter_set_ip_info(TCPIP_ADAPTER_IF_STA, &esp_ip);
	tcpip_adapter_set_dns_info(TCPIP_ADAPTER_IF_STA, TCPIP_ADAPTER_DNS_MAIN, &dnsserver);
}

void wifi_conectar_ap(void){
	esp_wifi_set_promiscuous(false);
	
    wifi_config_t wifi_config = {
        .sta = {
            .ssid = WIFI_SSID,
            .password = WIFI_PASS,
        },
    };
    esp_wifi_set_mode(WIFI_MODE_STA);
    esp_wifi_set_config(ESP_IF_WIFI_STA, &wifi_config);
    esp_wifi_start();
	while(true){
		printf("a %s\n",SYSTEM_EVENT_STA_START);
				vTaskDelay(500 / portTICK_PERIOD_MS);
	}
	esp_wifi_connect();
		vTaskDelay(500 / portTICK_PERIOD_MS);
		vTaskDelay(100 / portTICK_PERIOD_MS);
		vTaskDelay(100 / portTICK_PERIOD_MS);
		vTaskDelay(500 / portTICK_PERIOD_MS);
		vTaskDelay(100 / portTICK_PERIOD_MS);
		vTaskDelay(100 / portTICK_PERIOD_MS);

}

void wifi_promiscuo(void){
    esp_wifi_set_mode(WIFI_MODE_NULL);
	esp_wifi_set_promiscuous(true);
	esp_wifi_set_channel(canal_seleccionado, WIFI_SECOND_CHAN_NONE);
}