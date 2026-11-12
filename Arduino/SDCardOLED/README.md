# SDCardOLED — listo para probar

## Carga en Arduino IDE

1. Abre `SDCardOLED.ino`. Mantén `StartupOLED.h` en esta misma carpeta.
2. Selecciona **Arduino Nano**, el procesador y el puerto que usas con tu detector.
   Si tu Nano requiere **ATmega328P (Old Bootloader)**, conserva esa opción.
3. Instala **SD, de Arduino** desde el gestor de bibliotecas si falta.
   SPI, EEPROM y Wire vienen con el paquete Arduino AVR Boards.
   Este programa no necesita Adafruit SSD1306, Adafruit GFX ni TimerOne.
4. Carga el programa. El monitor serial funciona a **9600 baudios**.

## Prueba

- Con el equipo apagado, inserta la SD y enciende el detector.
- Espera el arranque: incluye 4 segundos antes de inicializar la tarjeta,
  posibles reintentos y la espera original de comandos seriales.
- Si crea y vuelve a abrir el archivo, la pantalla mostrará solamente:

  ```text
  SD ok!
  File_M001.txt
  ```

  El nombre real cambia en cada arranque y puede comenzar con `File_S`.
- Deja registrar algunos eventos. Apaga el detector y revisa en el computador
  el archivo indicado: debe contener el encabezado y los eventos registrados.
- Para probar el error, apaga el detector, retira la SD y vuelve a encender.
  Debe mostrar `SD failed` y detenerse. Reinstala la tarjeta con el equipo
  apagado y reinicia para volver a probar.

Abrir el monitor serial puede reiniciar el Nano y crear otro archivo.
`SD ok!` confirma la creación y reapertura inicial; no comprueba cada escritura
posterior ni cambia si se retira la tarjeta durante la adquisición.

## Pantalla y adquisición

Preparado para el mismo SSD1306 **128×64, I2C 0x3C**, girado 180 grados como en
`OLED.ino`. En el Nano clásico: SDA=A4, SCL=A5. El pin 10 queda reservado para
la SD; no se usa como reset del OLED. Se supone que el módulo OLED dispone de
reset al encender. Si no responde el OLED, el programa continúa y conserva los
mensajes seriales.

La pantalla se limpia y escribe solo durante el arranque. No hay refrescos,
interrupciones de pantalla ni framebuffer de 1024 bytes. El muestreo de pulsos,
coincidencias, formato de eventos y `flush()` por evento conservan el código
original. El tiempo de referencia usa `unsigned long`.

Si falla la inicialización, apertura o disponibilidad de nombres 001–200,
muestra `SD failed` y espera un reinicio. Los comandos seriales `read` y `remove`
siguen disponibles, sin mostrar éxito de creación porque no crean archivos.
`read` no borra archivos; `remove` termina sin iniciar una adquisición sin archivo.
Los sketches originales SDCard y OLED permanecen intactos.

## Validación

Compilado para Arduino Nano ATmega328P con Arduino AVR Boards 1.8.7 y la
biblioteca SD instalada. Falta verificar la pantalla, tarjeta y coincidencias
en el detector físico. El programa conserva las concatenaciones String del
registro original y tiene un margen de RAM limitado.
