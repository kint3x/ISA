#include "connect.h"
#include "err.h"
#include "headers.h"

#include <openssl/bio.h>
#include <openssl/ssl.h>
#include <openssl/err.h>

BIO * popcl_unsecured_connect(int *error){

	(*error) = 0 ;
	SSL_load_error_strings();
	ERR_load_BIO_strings();
	OpenSSL_add_all_algorithms();

	BIO * bio;

	int pocet_char = strlen(args.server);
	char server_port[pocet_char+20];
	sprintf(server_port,"%s:%d",args.server,args.port);


	bio = BIO_new_connect(server_port);

	if(bio == NULL)
	{
	    (*error) = _CONNECTION_FAILED_TO_OPEN; // nastala chyba pri pripojeni
	    return NULL;
	}


	if(BIO_do_connect(bio) <= 0)
	{
	    (*error) = _CONNECTION_FAILED_TO_OPEN; // nepodarilo sa pripojiť k serveru
	    return NULL;
	}

	return bio;
}

BIO *popcl_secured_connect(int *error){

	BIO * bio;
	
	SSL_CTX  *ctx = SSL_CTX_new(SSLv23_client_method());
	SSL  *ssl;

	if((args.b_certfile || args.b_certdir) == 0){
		SSL_CTX_set_default_verify_paths(ctx);
	}
	
	bio = BIO_new_ssl_connect(ctx);
	BIO_get_ssl(bio, &ssl);
	SSL_set_mode(ssl, SSL_MODE_AUTO_RETRY);

	int pocet_char = strlen(args.server);
	char server_port[pocet_char+20];
	sprintf(server_port,"%s:%d",args.server,args.port);


	BIO_set_conn_hostname(bio, server_port);

	if(BIO_do_connect(bio) <= 0)
	{
	    (*error) = _CONNECTION_FAILED_TO_OPEN; // nepodarilo sa pripojiť k serveru
	    return NULL;
	}
	
	return bio;
}