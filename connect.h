/*
*	Implementácia POP3 klienta
*	Predmet: ISA
*	Autor: Martin Matějka <xmatej55@stud.fit.vutbr.cz>
*	Ročník: 3BIT
*
*/
#ifndef _CONNECT_H
#define _CONNECT_H

#include <string.h>
#include <openssl/bio.h>
#include <openssl/ssl.h>
#include <openssl/err.h>

#include "headers.h"
#include "args.h"


SSL_CTX *ctx;

BIO * popcl_unsecured_connect(int *error);
BIO *popcl_secured_connect(int *error);
BIO *BIO_new_ssl_edited(SSL_CTX *ctx, int client);
int setup_CTX();

int starttls_connection(BIO * bio);
#endif