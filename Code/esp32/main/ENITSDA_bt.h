// Alta modificación del ejemplo bt_discovery de espressif
#include "esp_bt.h"
#include "esp_bt_main.h"
#include "esp_bt_device.h"
#include "esp_gap_bt_api.h"

typedef enum {
    APP_GAP_STATE_IDLE = 0,
    APP_GAP_STATE_DEVICE_DISCOVERING,
    APP_GAP_STATE_DEVICE_DISCOVER_COMPLETE,
    APP_GAP_STATE_SERVICE_DISCOVERING,
    APP_GAP_STATE_SERVICE_DISCOVER_COMPLETE,
} app_gap_state_t;

typedef struct {
    bool dev_found;
    uint8_t bdname_len;
    uint8_t eir_len;
    uint8_t rssi;
    uint32_t cod;
    uint8_t eir[ESP_BT_GAP_EIR_DATA_LEN];
    uint8_t bdname[ESP_BT_GAP_MAX_BDNAME_LEN + 1];
    esp_bd_addr_t bda;
    app_gap_state_t state;
} app_gap_cb_t;

static app_gap_cb_t m_dev_info;

struct DB mac_bt_listado[MAC_TOTAL_ARRAY];
int mac_bt_cuenta=0;

static void update_device_info(esp_bt_gap_cb_param_t *param){
    int32_t rssi = 0;
    esp_bt_gap_dev_prop_t *p;
	int mac_temp[6];
	int mac_existe=0;

    for (int i = 0; i < param->disc_res.num_prop; i++) {
        p = param->disc_res.prop + i;
        switch (p->type) {
        case ESP_BT_GAP_DEV_PROP_RSSI:
            rssi = *(int8_t *)(p->val);
			for(int i=0;i<=6;i++){
				mac_temp[i]=param->disc_res.bda[i];
			}
			for(int i=0;i<=mac_bt_cuenta;i++){
				if(memcmp(mac_temp,mac_bt_listado[i].mac,sizeof(mac_temp)) == 0){
					mac_existe=1;
				}
			}
			if(mac_existe == 0){
				if(mac_bt_cuenta >= MAC_TOTAL_ARRAY){
					printf("Superado el limite de MACs: %d\n", mac_bt_cuenta);
					// Añadir bandera de saturacion de MAC, posible ataque.
				}else{
					memcpy(mac_bt_listado[mac_bt_cuenta].mac,mac_temp,sizeof(mac_temp));	
					mac_bt_listado[mac_bt_cuenta].rssi=rssi;
					mac_bt_cuenta++;
					printf("MAC BT AÑADIDA\n");
				}
			}  
            break;
        default:
            break;
        }
    }
}

void bt_app_gap_cb(esp_bt_gap_cb_event_t event, esp_bt_gap_cb_param_t *param){
    app_gap_cb_t *p_dev = &m_dev_info;

    switch (event) {
		case ESP_BT_GAP_DISC_RES_EVT: {
			update_device_info(param);
			break;
		}
		case ESP_BT_GAP_DISC_STATE_CHANGED_EVT: {
			if (param->disc_st_chg.state == ESP_BT_GAP_DISCOVERY_STOPPED) {
				printf( "Device discovery stopped.\n");
				printf("TOTAL: %d\n",mac_bt_cuenta);
				if ( (p_dev->state == APP_GAP_STATE_DEVICE_DISCOVER_COMPLETE ||
						p_dev->state == APP_GAP_STATE_DEVICE_DISCOVERING)
						&& p_dev->dev_found) {
					p_dev->state = APP_GAP_STATE_SERVICE_DISCOVERING;
					printf( "Discover services ...\n");
					esp_bt_gap_get_remote_services(p_dev->bda);
				}
			} else if (param->disc_st_chg.state == ESP_BT_GAP_DISCOVERY_STARTED) {
				printf( "Discovery started.\n");
			}
			break;
		}
		default: {
			printf("Event: %d\n", event);
			break;
		}
    }
    return;
}

void bt_inicializar(void){
    esp_bt_controller_mem_release(ESP_BT_MODE_BLE);
    esp_bt_controller_config_t bt_cfg = BT_CONTROLLER_INIT_CONFIG_DEFAULT();
	esp_bt_controller_init(&bt_cfg);
	esp_bt_controller_enable(ESP_BT_MODE_CLASSIC_BT);
	esp_bluedroid_init();
	printf("BT INIT\n");
}	

void bt_escaneo(void){
		printf("BT ENABLE\n");
	esp_bluedroid_enable();
    esp_bt_gap_set_scan_mode(ESP_BT_SCAN_MODE_CONNECTABLE_DISCOVERABLE);
    esp_bt_gap_register_callback(bt_app_gap_cb);
    app_gap_cb_t *p_dev = &m_dev_info;
    memset(p_dev, 0, sizeof(app_gap_cb_t));
    p_dev->state = APP_GAP_STATE_DEVICE_DISCOVERING;
    esp_bt_gap_start_discovery(ESP_BT_INQ_MODE_GENERAL_INQUIRY, 10, 0);
		printf("BT DISCO\n");
	//esp_bluedroid_disable();	
}
