#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include "rs485.h"

int
main()
{
	char ibuf[256], obuf[1024];
	char *p, *q;
	int n;
	char h[] = "0123456789abcdef";

	rs485_init();
	sei();

	rs485_send("RS485 test program by Y.Kohyama\r\n");

	while ((n = rs485_readln(ibuf, sizeof(ibuf)))) {
		q = obuf;
		for (p = ibuf; p < ibuf + n; p++) {
			*q++ = h[*p/16];
			*q++ = h[*p%16];
		}
		*q++ = '\r';
		*q++ = '\n';
		*q = '\0';
		rs485_send(obuf);
		_delay_ms(1);
	}
}

