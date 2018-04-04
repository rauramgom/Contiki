The Contiki Operating System - Get Started Tutorial
============================

[![Build Status](https://travis-ci.org/contiki-os/contiki.svg?branch=master)](https://travis-ci.org/contiki-os/contiki/branches)

Contiki is an open source operating system that runs on tiny low-power
microcontrollers and makes it possible to develop applications that
make efficient use of the hardware while providing standardized
low-power wireless communication for a range of hardware platforms.

Contiki is used in numerous commercial and non-commercial systems,
such as city sound monitoring, street lights, networked electrical
power meters, industrial monitoring, radiation monitoring,
construction site monitoring, alarm systems, remote house monitoring,
and so on.

For more information, see the Contiki website:

[http://contiki-os.org](http://contiki-os.org)

Índice
======
  1. Compilación del código
  2. Volcado de la imagen a la placa
      * UNIFLASH
      * TI SMARTRF FLASH PROGRAMMER v2
  3. Conexión por puerto serie
  4. Creacin dee túnel entre red RPL y PC
  5. Capturador de tráfico y visualización con Wireshark
  6. Protocolo de comunicación a través de UART
      * Funcionamiento del esclavo
  7. Ejemplo de demostración
      * Crear una imágen válida y flashear
      * Iniciar Wireshark
      * Ubicar las placas y contetar por serie Master&Slave
      * Pruebas tipo A - Interactuando con los sensores
        * Prueba A.1 - Establecimiento de un observador
        * Prueba A.2 - Realización de una petición simple
      * Pruebas tipo B - Interactuando con los actuadores
    
 
# 1.Compilación del código
Una vez tenemos terminado nuestro código y queremos probarlo directamente sobre las motas, necesitamos compilarlo para crear la imagen que posteriormente volcaremos. Ello se consigue con el siguiente comando:  
~~~
~$ make TARGET=srf06-cc26xx BOARD=srf06/cc26xx
~~~

![Ver /doc/pictures-GUIDE/pic1](https://github.com/rauramgom/contiki/blob/master/doc/pictures-GUIDE/pic1.png)

Si comprobamos qué ficheros se han creado, observaremos los .hex:
![Ver /doc/pictures-GUIDE/pic2](https://github.com/rauramgom/contiki/blob/master/doc/pictures-GUIDE/pic2.png)


# 2.Volcado de la imagen de la placa
Para volcar la nueva imagen, tan sólo hay que encender la placa, conectarla al USB y descargarle el nuevo firmware a través de uno de los dos programas que se exponene a continuación. Aunque la placa se autoresetea tras el nuevo grabado, conviene presionar el botón de ‘Reset’ para asegurarnos de que lo haya hecho correctamente.
Existen dos programas principalmente, *TI SmartRF Flash Programmer*  para Windows y *UniFlash para Linux*.

 ## UNIFLASH
   Esta herramienta puede ser descargada de [Uniflash Tool](http://www.ti.com/tool/UNIFLASH). Tan sólo hay que seguir unos sencillos pasos:
   * Detectamos la placa conectada
![Ver /doc/pictures-GUIDE/pic3](https://github.com/rauramgom/contiki/blob/master/doc/pictures-GUIDE/pic3.png)
   * Seleccionamos el tipo (CC2650F128)
![Ver /doc/pictures-GUIDE/pic4](https://github.com/rauramgom/contiki/blob/master/doc/pictures-GUIDE/pic4.png)
   * Seleccionamos la imagen en el buscador
![Ver /doc/pictures-GUIDE/pic5](https://github.com/rauramgom/contiki/blob/master/doc/pictures-GUIDE/pic5.png)
   * y cargamos
![Ver /doc/pictures-GUIDE/pic6](https://github.com/rauramgom/contiki/blob/master/doc/pictures-GUIDE/pic6.png)
    
 ## TI SMARTRF FLASH PROGRAMMER v2
   Esta herramienta puede ser descargada de [FLASH-PROGRAMMER Tool](http://www.ti.com/tool/FLASH-PROGRAMMER), y también hay un manual de usuario disponible en [SmartRF Flash Programmer User Manual](http://www.ti.com/lit/ug/swru069g/swru069g.pdf). Es necesario registrarse previamente para descargarlo, y los pasos a realizar son los mismos que en el anterior:
   * Detectamos la placa conectada y seleccionamos la imagen que queremos cargar:
![Ver /doc/pictures-GUIDE/pic7](https://github.com/rauramgom/contiki/blob/master/doc/pictures-GUIDE/pic7.png)
   * Seleccionamos las acciones que queremos realizar (Es bueno escoger ‘*Erase*’ y ‘*Program*’ para borrar todo antes de volcar lo nuevo), seleccionamos el tipo de mota y hacemos click en el botón de ‘*Play*’:
![Ver /doc/pictures-GUIDE/pic8](https://github.com/rauramgom/contiki/blob/master/doc/pictures-GUIDE/pic8.png)

Esta herramienta también es necesaria si queremos hacer un vaciado completo de la placa (*Forced Mass Erase*), algo que no es posible con *UNIFlash*. Ello se consigue en el icono de herramientas que aparece arriba a la derecha:
![Ver /doc/pictures-GUIDE/pic9](https://github.com/rauramgom/contiki/blob/master/doc/pictures-GUIDE/pic9.png)


# 3.Conexión por puerto serie
Una vez que tenemos las motas flasheadas con la imagen correcta, podemos ver la salida de éstas a través de su conexión por puerto serie con el ordenador. Basta con realizar los siguientes pasos: 
~~~
#Con esto comprobamos que el dispositivo está correctamente conectado
~$ lsusb 

#Es necesario escalar privilegios previamente
~$ sudo su

(root)~$ modprobe ftdi_sio vendor=0x403 product=0xa6d1 & modprobe ftdi_sio & echo 0403 a6d1 > /sys/bus/usb-serial/drivers/ftdi_sio/new_id & ll /dev/ttyUSB*

#Nos situamos en el directorio de la imagen
(root)~$ cd contiki/examples/my_example/

#Comando para la conexión
(root)~$ make login TARGET=srf06-cc26xx PORT=/dev/ttyUSB1
~~~

![Ver /doc/pictures-GUIDE/pic10](https://github.com/rauramgom/contiki/blob/master/doc/pictures-GUIDE/pic10.png)

**¡¡IMPORTANTE!!** Consideraciones a tener en cuenta:  
  * Hay que tener habilitados los logs de la aplicación (como por ejemplo en TSCH o RPL). En caso contrario, sólo se mostrarán aquellas salidas estándar/errores que estén habilitadas por defecto.
  * Dado que los recursos de las placas son limitados, habilitar la salida consume muchos recursos, a priori innecesarios para el funcionamiento de la misma, lo cual puede afectar considerablemente, ya sea al *quehacer* de la placa o al tamaño final de la imagen, reduciéndose cuantiosamente si estos están habilitados.
  * Cualquier salida debe ser deshabilitada si la mota va a hacer uso de comunicación por UART, pues todos estos logs se enviarían por los pines empleados para ello, ensuciando la transferencia esperada y provocando un incorrecto funcionamiento de la misma.
  * Para reducir el tamaño de las imágenes, conviene habilitar la siguiente macro al inicio de cada fichero de configuración (project-conf.h):
~~~
      /* 
      *	Disable the name strings from being stored, reducing both
      *	flash and RAM usage
      */
      #define PROCESS_CONF_NO_PROCESS_NAMES 1
~~~


# 4.Creacin de túnel entre red RPL y PC
Cuando queremos establecer un túnel para conectar la red RPL que hemos montado a nuestro ordenador, como por ejemplo hacemos cuando establecemos una de las motas como BorderRouter (en el caso de establecer el Router en el PC y poner la placa como radio había algunos problemas y no siempre funcionaba), hacemos lo siguiente:  
  * Con la imagen ya volcada, abrimos un terminal en **modo root** y nos desplazamos al directorio contiki/tools para ejecutar lo siguiente:
~~~
      # Paramos el servicio 6lbr por si estuviese activo por defecto, para evitar problemas con los recursos a los que acceden
      (root) ~$ service 6lbr stop

      (root) ~$ modprobe ftdi_sio vendor=0x403 product=0xa6d1 && modprobe ftdi_sio && echo 0403 a6d1 > /sys/bus/usb-serial/drivers/ftdi_sio/new_id && ll /dev/tty*

      # Creamos el túnel
      (root) ~$ ./tunslip6 -L -v2 -s /dev/ttyUSB1 aaaa::1/64
~~~
Con el último comando hemos establecido correctamente el túnel entre ambos. El significado de los parámetros es el siguiente:  
~~~
tunslip6  [options]  IPADDRESS
~~~
  * -L: Activa los logs de salida.
  * -v: Nivel de verbosity (con ‘./tunslip6 -h’ se puede ver los distintos niveles que existen).
  * -s: interfaz serie escogida.
  * -t: nombre asociado a la interfaz (por defecto coge tun0)
  * IPADDRESS: prefijo de red que se utilizará para identificar a los nodos.

Ejemplo:  
![Ver /doc/pictures-GUIDE/pic11](https://github.com/rauramgom/contiki/blob/master/doc/pictures-GUIDE/pic11.png)


# 5.Capturador de tráfico y visualización con Wireshark
Se trata de una de las herramientas indispensables para poder capturar el tráfico que se está generando en la red. Para ello, podemos descargarla de [Ccsniffpiper - Github](https://github.com/rauramgom/ccsniffpiper).  
Para usarlo, es necesario colocarlo en un USB3.0 y, **como superusuario**, ejecutar lo siguiente:  
~~~
(root)~$ ./pipe2wireshark.sh 
~~~
Básicamente lo que hace es capturar todo el tráfico resultante y extraer todo a un fichero PCAP a través de una tubería FIFO (por defecto *“/tmp/ccsniffpiper”*), la cual Wireshark toma como interfaz de entrada para mostrarlo por pantalla.
![Ver /doc/pictures-GUIDE/pic12](https://github.com/rauramgom/contiki/blob/master/doc/pictures-GUIDE/pic12.png)

![Ver /doc/pictures-GUIDE/pic13](https://github.com/rauramgom/contiki/blob/master/doc/pictures-GUIDE/pic13.png)


#  6.Protocolo de comunicacin a través de UART
Con objeto de tener una comunicación rápida y efectiva entre el maestro y el esclavo (recordemos que ambos en conjunto actúan como el servidor COAP) por interfaz serie, ha sido necesario indagar en Contiki para poder tratar la información que se envía y recibe a través de dicha interfaz.  
Una vez estudiado el contenido de estas funciones, y sabiendo que sólo puede enviarse información carácter a carácter, se ha optado por elaborar un diccionario o mapa de equivalencias que permita comunicar peticiones COAP de manera efectiva. El diccionario es el siguiente:

| Parámetro           | Valor|
| ----------          | -----|
| TEMP                | '1'  |
| VOLT                | '2'  |
| LED_GREEN_POST_ON   | '3'  |
| LED_GREEN_POST_OFF  | '4'  |
| LED_BLUE_POST_ON    | '5'  |
| LED_BLUE_POST_OFF   | '6'  |
| LED_RED_POST_ON     | '7'  |
| LED_RED_POST_OFF    | '8'  |
| LED_YELLOW_POST_ON  | '9'  |
| LED_YELLOW_POST_OFF | 'a'  |
| LED_ALL_POST_ON     | 'b'  |
| LED_ALL_POST_OFF    | 'c'  | 
| END                 | 0x0a |

## 6.1 Funcionamiento del esclavo
Cuando el servidor COAP reciba (estamos hablando del Maestro, el que termina la comunicación COAP) una petición sobre algún sensor o actuador, este le trasladará la información al esclavo para que ejecute las órdenes, bien sea encender algún led o bien tomar medidas de batería o temperatura, y devolverá de nuevo la información obtenida. Para establecer una comunicación sencilla sobre la interfaz serie, sin tener que reenviar la petición COAP ni tener que levantar un cliente-servidor UDP entre ambos, se ha optado por el **envío de un sólo carácter** como identificador de la URI solicitada, agilizando tiempos y siendo efectivo, en lugar de reenviar la URI completa (lo que supondría enviar todos los caracteres del path uno a uno para, a fin de cuentas, realizar la misma operación).

Cuando el esclavo reciba este identificador por la entrada serie, comparará con el mapa de equivalencias para conocer cual es la operación a realizar. Tras ello, deberá devolver la información obtenida de los sensores por caracteres, más un carácter especial de terminación, de tal forma que el cliente estará preparado para interpretar el mensaje completo que acabará con dicho carácter de terminación de mensaje.


# 7. Ejemplo de demostración
A continuación se muestra un ejemplo de un escenario completo, en el que participan tanto clientes como servidores CoAP, con intercambio de información y la captación del tráfico pertinente.

## Crear una imágen válida y flashear  
Estos dos pasos están ampliamente explicados en los apartados ‘*1.- Compilación del código*’ y  
‘*2.- Volcado de la imagen a la placa*’.  
Los archivos utilizados pueden ser encontrados en /examples/my_apps/server_COAP_\*/ para el Master y Slave, y en /examples/ipv6/rpl-border-router\*/ para el BorderRouter.

**¡¡IMPORTANTE!!** La ruta completa no debe ser escesivamente larga, puede fallar al flashear. Un ejemplo válido puede ser /home/user/contiki/.

## Iniciar Wireshark  
Este paso está ampliamente explicado en el apartado ‘*5.- Capturador de tráfico y visualización con Wireshark*’.

## Ubicar las placas y contetar por serie Master&Slave  
El funcionamiento de la conexión por UART viene explicado en ‘*6.- Protocolo de comunicación a través de UART*’.  
En cuanto a la conexión física, han sido utilizados únicamente 3 pines: **GND**, **EM_UART_TX** y **EM_UART_RX**, siendo estos dos últimos cruzados en su conexión. En la siguiente foto puede verse su conexionado:

![Ver /doc/pictures-GUIDE/pic17](https://github.com/rauramgom/contiki/blob/master/doc/pictures-GUIDE/pic17.png)

Lo primero que tenemos que levantar es el router, para que sirva la red. Esto puede hacerse fácilmente **como superusuario** con los siguientes comandos:
~~~
(root)~$ cd /contiki/tools/
(root)~$ modprobe ftdi_sio vendor=0x403 product=0xa6d1 && modprobe ftdi_sio && echo 0403 a6d1 > /sys/bus/usb-serial/drivers/ftdi_sio/new_id && ll /dev/tty*
(root)~$ ./tunslip6 -L -v2 -s /dev/ttyUSB1 aaaa::1/64
~~~
**¡¡IMPORTANTE!!** Es necesario tener el servicio 6lbr detenido (sudo service 6lbr stop)

![Ver /doc/pictures-GUIDE/pic18](https://github.com/rauramgom/contiki/blob/master/doc/pictures-GUIDE/pic18.png)

## Pruebas tipo A - Interactuando con los sensores
Una vez llegado a este punto, encendemos las placas y esperamos un tiempo determinado para lograr una red estable, antes de mandar peticiones. Para interactuar con los sensores se ha elaborado un cliente en Python (El código puede encontrarse en la ruta
[CoAPclient](https://github.com/rauramgom/CoAPclient/)).

### Prueba A.1 - Establecimiento de un observador
![Ver /doc/pictures-GUIDE/pic19](https://github.com/rauramgom/contiki/blob/master/doc/pictures-GUIDE/pic19.png)

### Prueba A.2 - Realización de una petición simple
![Ver /doc/pictures-GUIDE/pic20](https://github.com/rauramgom/contiki/blob/master/doc/pictures-GUIDE/pic20.png)

## Pruebas tipo B - Interactuando con los actuadores
Con el escenario ya montado y la red establecida, instalamos en Mozilla Firefox el **plug-in Copper** (puede hacerse desde aquí: [Plug-in Copper](https://addons.mozilla.org/es/firefox/addon/copper-270430/)).  
**¡¡IMPORTANTE!! No funciona en nuevas versiones de Firefox, hay que utilizar la versión 55.0.3**
![Ver /doc/pictures-GUIDE/pic21](https://github.com/rauramgom/contiki/blob/master/doc/pictures-GUIDE/pic21.png)

A través del cliente web del router es posible adivinar la dirección IPv6 del nodo con el que queremos comunicarnos:
![Ver /doc/pictures-GUIDE/pic22](https://github.com/rauramgom/contiki/blob/master/doc/pictures-GUIDE/pic22.png)

Ahora abrimos una pestaña nueva en Firefox, y puesto que ya tenemos el plug-in instalado, tecleamos:
~~~
coap://[aaaa::212:4b00:7bb:1384]/
~~~
Nos aparecerá el cliente COAP, donde tendremos que pinchar sobre el botón ‘*Discover*’ o bien hacer un GET sobre *‘well-known/core’* para que nos devuelva todos los recursos disponibles:  
![Ver /doc/pictures-GUIDE/pic23](https://github.com/rauramgom/contiki/blob/master/doc/pictures-GUIDE/pic23.png)  
Puesto que queremos los actuadores, seleccionamos un tipo de leds cualquiera, y rellenamos su Payload (pinchando en el campo ‘*Outgoing*’) con la estructura ‘**mode=valor**’, donde el campo valor puede ser:
~~~
  mode=off
  mode=on
~~~
Una vez rellenado, pinchamos en el botón **POST** o **PUT** (los recursos cuentan con ambos métodos, los dos igualmente configurados. Su configuración puede verse en la macro **RESOURCE** de cada led en la ruta /server_COAP_master\*/resources/res-leds.c ). Un ejemplo:
![Ver /doc/pictures-GUIDE/pic24](https://github.com/rauramgom/contiki/blob/master/doc/pictures-GUIDE/pic24.png)
