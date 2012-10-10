#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <stdlib.h>

static volatile struct {
	char *buf;
	int size;
	int read;
	int write;
} rs485_rx, rs485_tx;

void
rs485_init()
{
	rs485_rx.size = 256;
	rs485_rx.buf = (char *)malloc(rs485_rx.size * sizeof(char));
	rs485_rx.read = 0;
	rs485_rx.write = 0;
	rs485_tx.size = 256;
	rs485_tx.buf = (char *)malloc(rs485_tx.size * sizeof(char));
	rs485_tx.read = 0;
	rs485_tx.write = 0;

	DDRD |= 0x04; // PD2:Dir
	PORTD &= ~0x04;
	_delay_us(500);

	UBRR0 = 12; // 38400bps under CPU 8MHz
	UCSR0A = 0;
	UCSR0B = _BV(RXCIE0)|_BV(RXEN0)|_BV(TXEN0);
	UCSR0C = _BV(UCSZ01)|_BV(UCSZ00);
}

void
rs485_send(char *p)
{
	int next;

	if (*p == '\0')
		return;

	cli();
	next = (rs485_tx.write + 1)%rs485_tx.size;
	while (next != rs485_tx.read && *p != '\0') {
		rs485_tx.buf[rs485_tx.write] = *p++;
		rs485_tx.write = next;
		next = (rs485_tx.write + 1)%rs485_tx.size;
	}
	PORTD |= 0x04;
	UCSR0B |= _BV(UDRIE0);
	sei();
}

int
rs485_readln(char *buf, int size)
{
	int n = 0;

	do {
		cli();
		while (rs485_rx.read != rs485_rx.write && n + 1 < size) {
			buf[n++] = rs485_rx.buf[rs485_rx.read];
			rs485_rx.read = (rs485_rx.read + 1)%rs485_rx.size;
		}
		sei();
	} while (n == 0 || buf[n - 1] != 0x0a);
	buf[n] = '\0';

	return n;
}

ISR(USART_RX_vect)
{
	int next;

	PORTB |= 0x02;

	cli();
	next = (rs485_rx.write + 1)%rs485_rx.size;
	rs485_rx.buf[rs485_rx.write] = UDR0;
	if (next != rs485_rx.read)
		rs485_rx.write = next;
	sei();
}

ISR(USART_UDRE_vect)
{
	cli();
	if (rs485_tx.read == rs485_tx.write) {
		UCSR0B &= ~_BV(UDRIE0);
		_delay_us(500);
		PORTD &= ~0x04;
	} else {
		UDR0 = rs485_tx.buf[rs485_tx.read];
		rs485_tx.read = (rs485_tx.read + 1)%rs485_tx.size;
	}
	sei();
}


