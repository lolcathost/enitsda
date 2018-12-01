# ENITSDA

ENITSDA crea una capa extra de monitorización perimetral para añadir seguridad al entorno. Se trata de la detección de dispositivos intrusos en lugares controlados en base a sus direcciones MAC Wi-Fi y Bluetooth, utilizando como recolectores dispositivos ESP32 y comparando los datos con listas blancas/negras.

En el directorio *Presentacion_Hackaton* se encuentra la presentación inicial.

Dentro del directorio *Code* se irán subiendo todos los cambios y el software al completo:
 - El directorio esp32 corresponde al desarollo del dispositivo ESP32.
 - Los directorios config, deauth y enitsda corresponden al desarollo del servidor.

*TODO*

ESP32:

	- [ ] Añadir descubrimiento bluetooth
	
	- [x] Corregir el random stack overflow en la petición HTTPS 
	
	- [x] Añadir POST + TOKEN
	
	- [x] Generar envio de MACs por HTTPS
	
C&C:

	- [x] Lógica para alertas
	
	- [ ] Mejora de alertas
	
	- [x] Bot telegram
	
	- [x] uwsgi automatico
	
	- [ ] hardeningg rabbitmq
