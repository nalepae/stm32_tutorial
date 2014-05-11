Tutorial STM32
============

This repository implements almost all the exercices described in the book [Discovering the STM32 Microcontroller] (http://www.cs.indiana.edu/~geobrown/book.pdf), written by Geoffrey Brown (edition January 18, 2014).

*Almost* all the exercices because, instead of GNU toolchain and USB flasher, the IDE [Keil µVision 4] (https://www.keil.com) is used.

The tutorial uses a STM32F4 discovery board. Here a STM32F0 discovery board is used. Between the tutorial and the implementation, some functions are a little bit different.

As prescribed in the tutorial, the Standard Peripheral Library is used. It is available on the ST website.

The repository contains only source code (.c and .h) for each exercice. It does not contain project files or things like this ...
___________

For the moment, available exercices are :

(For all exercices, STM32 discovery board is needed)

**Exercice 4.1 : Blinking lights**

* *Special need* : Nothing
* *Purpose* : Blink the blue LED.

**Exercice 4.2 : Blinking lights and Pushbutton**

* *Special need* : Nothing
* *Purpose* : Blink the blue LED and light the green LED when user button is pushed.

**Exercice 4.3 : Configuration without Standard Peripheral Library**

* *Special need* : Nothing
* *Purpose* : Without the Standard Peripheral Library, toggle the green LED when the user button is pushed.

_______________

From now, a logic analyser is always needed. [This one](http://www.ebay.fr/itm/ANALYSEUR-LOGIQUE-8-VOIES-24-MHZ-COMPATIBLE-SALEAE-VendeurPro-Exp-Flash-Eclair-/321289921961?_trksid=p3984.m1497.l2649&ssPageName=STRK:MEWNX:IT#ht_5177wt_1195) works very well.

______________

**Exercice 5.2 : Hello World!**

* *Special need* : UART ==> USB converter (Arduino UNO board will do the job !)
* *Purpose* : Print "hello world" through USART bus every 250 ms.

**Exercice 5.3 : Echo**

* *Special need* : UART <=> USB converter (Arduino UNO board will do the job !)
* *Purpose* : Read data on USART bus Rx and send the same data on USART bus Tx.

**Exercice 6.1 : SPI Loopback**

* *Special need* : Nothing
* *Purpose* : Send datas from SPI peripheral to itself (MISO pin is connected on MOSI pin), in 8 bits mode and in 16 bits mode.

**Exercice 6.2 : Write and Test an EEPROM Module**
* *Special need* : EEPROM Microchip 25LC160
* *Purpose* : Write datas into a SPI EEPROM and read these same datas from the same EEPROM.
