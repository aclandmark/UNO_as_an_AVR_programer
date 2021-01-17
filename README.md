# UNO_as_an_AVR_programer

 Code has been developed that enables an Atmega328 to be used to program a small number of AVR devices.  
The Atmega328 programmer communicates with a terminal program such as Bray++ running on a PC.
The terminal program downloads a hex file to the programmer which parses it and copies the resulting commands
to a target device.
Flash, configuration bytes and EEPROM can all be programmed.
Parts of this code can be cut and paste into other applications as required.

In addition to programming over the standard MOSI.MISO interface it has also been adapted for use as a bootloader.

The UNO is one obvious place to pput this code.
