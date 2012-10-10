# AVR-RS485

## What is this?

Sample code to use half-duplex 2wire RS485 with Atmel AVR and a RS485 driver.

This code includes functions to

* initialize internal buffers,
* send a string as null terminated ASCII characters and
* receive a LF terminated line and return it with null terminator.

This is not a library but a mere code example.
Note that the UART setting 38400bps (for 8MHz CPU Clock),
8databit, none parity and 1stopbit are hardcoded.
Change them as you want or write code to change them by yourself.

And the code assumes using AVR-GCC and ATMega328p.
Edit the code as tools and avr you use.

# Connectings

* PD0(RxD), PD1(TxD) and PD2 should be conencted to
RO, DI and DE of RS485 driver respectively.
* PD0(RxD) should be pulled up externally.
* DE should be shortened to \overline{RE}

# How to use

Include the header.

    #include "rs485.h"

Initialize the internal buffers and setting some registers.

	rs485_init();
	sei();

Note that rs485_init() don't do sei(), do it manually when you want.

To send a string, do

    rs485_send("Hello World!");

To read a line terminated with LF(0x0a)

    char buf[256];
    int n;
    n = rs485_readln(buf, sizeof(buf));

# Sample main.c

Output "RS485 test program by Y.Kohyama\r\n" to the RS485 line
on the start up and wait LF terminated characters from the line and echo back the hexadecimal dump
of them and CR+LF to the line.

Thank you.
