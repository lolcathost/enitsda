#include <stdio.h>
#include <string.h>
#include "nvs_flash.h"
#include "esp_system.h"
#include "esp_event_loop.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/event_groups.h"

// Tipo de escaneo que realizara el ESP32: WIFI o BT
#define ESCANEO_WIFI 1
#define ESCANEO_BT 2
#define TIPO_ESCANEO ESCANEO_BT

// Con 512 deberia ser suficiente, 512 macs en un minuto..
#define MAC_TOTAL_ARRAY 512

struct DB{
	int mac[6];
	signed int rssi;
};