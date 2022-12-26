# UNO_as_an_AVR_programer

 Code has been developed that enables an Atmega328 to be used to program a small number of AVR devices.  
The Atmega328 programmer communicates with a terminal program such as Bray++ running on a PC.
The terminal program downloads a hex file to the programmer which parses it and copies the resulting commands
to a target device.
Flash, configuration bytes and EEPROM can all be programmed.
Parts of this code can be cut and pasted into other applications as required.

In addition to programming over the standard MOSI,MISO interface it has also been adapted for use as a bootloader.

The versions given here have been adapted to run on a UNO. 
Many novices will already have a UNO. The purpose of this development is to offer them an easy way of programming new devices
with the minimum need to use new software tools and techniques. 

Links to relevent AVRfreaks projects


https://github.com/MicrochipTech/avrfreaks-projects/tree/9e6bc971411a2d69374012572b3b6144f2f3f7f2/projects/unoavrprogrammer

https://github.com/MicrochipTech/avrfreaks-projects/tree/9e6bc971411a2d69374012572b3b6144f2f3f7f2/projects/unoavrprogrammerv2


