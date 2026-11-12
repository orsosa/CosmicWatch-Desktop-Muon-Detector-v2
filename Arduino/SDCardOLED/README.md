# SDCardOLED

Abre SDCardOLED.ino en Arduino IDE y conserva StartupOLED.h en la misma carpeta.
Selecciona el Nano, procesador y puerto que usas normalmente. Requiere SD de
Arduino, SPI, EEPROM y Wire. No requiere Adafruit ni TimerOne.
Monitor serial: 9600 baudios.

Esta versión conserva el programa SDCard.ino original y añade únicamente la
pantalla SSD1306 128x64 I2C en 0x3C, con la orientación del OLED original.
No modifica las instrucciones del LED, las esperas originales, la adquisición,
los pulsos de coincidencia ni la escritura de eventos.

Al abrir el archivo muestra:

    SD ok!
    File_M001.txt

El nombre mostrado es el archivo real, M o S. Si falla SD.begin() o no se abre
un archivo, muestra SD failed. El mensaje indica solamente el estado inicial;
no supervisa fallos posteriores. Los modos read/remove no crean archivo y no
muestran SD ok!.

La pantalla se inicializa antes de SD.begin() y muestra el resultado después
de intentar abrir el archivo. No se actualiza durante la adquisición ni usa
interrupciones o framebuffer. El pin 10 queda reservado para la SD; no se usa
como reset del OLED. Nano clásico: SDA=A4, SCL=A5.

Carga el sketch con la SD insertada, revisa la pantalla y deja registrar eventos.
Apaga antes de retirar la tarjeta y comprueba en el computador el archivo indicado.
Abrir el monitor serial puede reiniciar el Nano y generar otro archivo.

Se preserva también el manejo de errores original: si falla SD.begin(), setup()
retorna y loop() sigue ejecutándose; el OLED indica el fallo, pero el programa
original puede seguir contando sin guardar. Reinicia con la tarjeta instalada.

Validado mediante compilación para Arduino Nano ATmega328P. Falta la prueba
física del detector. Los sketches originales permanecen intactos.
