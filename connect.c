#include "connect.h"
#include "err.h"
#include "headers.h"
#include <stdbool.h>

#include <openssl/bio.h>
#include <openssl/ssl.h>
#include <openssl/err.h>

BIO * popcl_unsecured_connect(int *error){

	(*error) = 0 ;
	SSL_load_error_strings();
	SSL_library_init();
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
	
	SSL_load_error_strings();
	SSL_library_init();
	ERR_load_BIO_strings();
	OpenSSL_add_all_algorithms();
	BIO * bio;
	SSL_CTX  *ctx = SSL_CTX_new(SSLv23_client_method());
	SSL  *ssl;

	if((args.b_certfile || args.b_certdir) == 0){
		if(!SSL_CTX_set_default_verify_paths(ctx)){
			*(error) = _CANT_LOAD_CERT;
			return NULL;
		}
	}

	else if((args.b_certdir && args.b_certfile) == 1){
		if(!SSL_CTX_load_verify_locations(ctx,args.c,args.C)){
			*(error) = _CANT_LOAD_CERT;
			return NULL;
		}	
	}
	else if(args.b_certfile){
		if(!SSL_CTX_load_verify_locations(ctx,args.c,NULL)){
			*(error) = _CANT_LOAD_CERT;
			return NULL;
		}
		if(!SSL_CTX_set_default_verify_dir(ctx)){
			*(error) = _CANT_LOAD_CERT;
			return NULL;
		}
	}
	else{
		if(!SSL_CTX_load_verify_locations(ctx,NULL,args.C)){
			*(error) = _CANT_LOAD_CERT;
			return NULL;
		}
		if(!SSL_CTX_set_default_verify_file(ctx)){
			*(error) = _CANT_LOAD_CERT;
			return NULL;
		}
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


	if(SSL_get_verify_result(ssl) != X509_V_OK)
	{
	    (*error) = _CONNECTION_FAILED_TO_OPEN;
	    return NULL;
	}
	
	
	return bio;
}




int starttls_connection(BIO * bio){
	SSL_load_error_strings();
	SSL_library_init();
	ERR_load_BIO_strings();
	OpenSSL_add_all_algorithms();

	SSL_CTX  *ctx = SSL_CTX_new(SSLv23_client_method());
	SSL  *ssl;
	SSL_CTX_set_default_verify_paths(ctx);

	BIO *ret=NULL,*sslC=NULL;

	sslC = BIO_new_ssl(ctx,1);
	ret = BIO_push(sslC,bio);

	BIO_get_ssl(ret,&ssl);

	if(BIO_do_connect(bio) <= 0){
		return _CONNECTION_FAILED_TO_OPEN;
	}

	if(SSL_get_verify_result(ssl) != X509_V_OK)
	{
	    return _CONNECTION_FAILED_TO_OPEN;
	}


	return 0;
}