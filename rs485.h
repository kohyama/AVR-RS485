#ifndef _RS485_H
#define _RS485_H

void rs485_init();
void rs485_send(char *p);
int rs485_readln(char *buf, int size);

#endif /* _RS485_H */
