# ENITSDA - ESP32 

El software captura las mac de los dispositivos WIFI y Bluetooth
constantemente y se la envia al servidor para tener una capa extra
de seguridad perimetral.

La documentación de espressif para las API:
 - https://docs.espressif.com
Se han usado parte de los ejemplos que trae espressif en su SDK:
 - cmd_sniffer
 - freertos
 - https_request
 - bt_discovery (se usará en un futuro, aun no implementado)

Manual para grabar en un esp32:
 - Requisitos:
  - Tener la preinstalación que indica espressif:
    - https://docs.espressif.com/projects/esp-idf/en/latest/get-started/index.html#setup-toolchain
	- Conocer el puerto comX (en windows) o el /dev/ttyXXX (en linux) que genera el esp32.
	- Tener, obviamente, un esp32 :P.
  - Instalación:
	- git clone https://github.com/lolcathost/enitsda/
	- cd enitsda/Code/esp32
	- make menuconfig
	  - Aquí tenéis que configurar el purto com mencionado antes en:
		- Serial flasher config
		- Default serial port
	  - Guardar y salir.
	- Generar un fichero que se llame ENITSDA_vars.h con el siguiente contenido
>#define WEB_SERVER "ip_dns/url_server"
>#define WEB_PORT "443"
>#define WEB_URL "tu_url"

>#define WIFI_SSID "tu_wifi"
>#define WIFI_PASS "tu_contraseña"
    - make
		- En este punto tarda porque compila todo el SDK la primera vez.
	- make flash
		- Quemamos la flash del esp32.
	- make monitor
		- Vemos por uart la salida del dispositivo.
	- (se puede usar "make flash monitor")
			
