#include "esp_wifi.h"

#include "lwip/err.h"
#include "lwip/sockets.h"
#include "lwip/sys.h"
#include "lwip/netdb.h"
#include "lwip/dns.h"

#define PAQUETE_CABECERA_TAM 24
#define PAQUETE_POSICION_MAC 16
int canal_seleccionado=1,canal_maximo=13;

#define MAC_TOTAL_ARRAY 512
struct DB{
	int mac[6];
	signed int rssi;
}mac_wifi_listado[MAC_TOTAL_ARRAY];
int mac_bt_listado[MAC_TOTAL_ARRAY][6];
int mac_wifi_cuenta=0,mac_bt_cuenta=0;

// Filtro de paquetes WIFI: gestion y datos
const wifi_promiscuous_filter_t filtro={
	.filter_mask=WIFI_PROMIS_FILTER_MASK_MGMT|WIFI_PROMIS_FILTER_MASK_DATA
};

wifi_config_t wifi_config={
	.sta={
		.ssid=WIFI_SSID,
		.password=WIFI_PASS,
	},
};

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

// DEBUG: Funcion temporal para ver que funciona el llenado de macs del array WIFI y BT
void mac_mostrar_listados(void){
	for(int i=0;i<mac_wifi_cuenta;i++){
		int *p=mac_wifi_listado[i].mac;
		printf("W -> %02x:%02x:%02x:%02x:%02x:%02x  RSSI -> %d\n",p[0],p[1],p[2],p[3],p[4],p[5],mac_wifi_listado[i].rssi);	
	}
	printf("# CONOCIDOS WF:[%d] BT:[%d]\n",mac_wifi_cuenta, mac_bt_cuenta);
}

void wifi_inicializar(void){
	wifi_init_config_t cfg = WIFI_INIT_CONFIG_DEFAULT();
    esp_wifi_init(&cfg);
    esp_wifi_set_storage(WIFI_STORAGE_RAM);
    esp_wifi_set_promiscuous_filter(&filtro);
    esp_wifi_set_promiscuous_rx_cb(&sniffer);
    esp_wifi_set_ps(WIFI_PS_NONE);
    esp_wifi_set_channel(canal_seleccionado, WIFI_SECOND_CHAN_NONE);
}

void wifi_conectar_ap(void){
	esp_wifi_set_promiscuous(false);
    esp_wifi_set_mode(WIFI_MODE_STA);
    esp_wifi_set_config(ESP_IF_WIFI_STA, &wifi_config);
    esp_wifi_start();
}

void wifi_promiscuo(void){
    esp_wifi_set_mode(WIFI_MODE_NULL);
	esp_wifi_set_promiscuous(true);
}