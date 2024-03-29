/*
*	Implementácia POP3 klienta
*	Predmet: ISA
*	Autor: Martin Matějka <xmatej55@stud.fit.vutbr.cz>
*	Ročník: 3BIT
*
*/
#include <stdbool.h>

#ifndef _HEADERS_H
#define _HEADERS_H

/**
 *	Hlavičkový súbor headers.h obsahuje používané makrá a definíciu defaultnej veľkosti buffera 
 * 
*/

#define BUFFER_SIZE 1024

#define CHECK_ret if(ret!=0) return ret;
#define EMPTY_buf memset(buf, '\0', sizeof(buf));

#define WRITE_buf if(BIO_write(bio, buf, strlen(buf)) <= 0) { if(! BIO_should_retry(bio)){return _CONNECTION_FAILED;} return _CONNECTION_FAILED;}

#define READ_buf ret = BIO_read(bio, buf, BUFFER_SIZE);if(ret == 0){return _CONNECTION_CLOSED;}else if(ret < 0){if(! BIO_should_retry(bio)){return _CONNECTION_FAILED;}return _CONNECTION_FAILED; }

#define PRINT_buf /*printf("%s",buf)*/;

#define CHECK_buf_reply if(buf[0] != '+') return _REPLAY_BAD;

#endif