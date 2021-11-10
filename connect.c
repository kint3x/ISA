/*
*	Implementácia POP3 klienta
*	Predmet: ISA
*	Autor: Martin Matějka <xmatej55@stud.fit.vutbr.cz>
*	Ročník: 3BIT
*
*/
#include "connect.h"
#include "err.h"
#include "headers.h"
#include <stdbool.h>

#include <openssl/bio.h>
#include <openssl/ssl.h>
#include <openssl/err.h>

/*
*	Nezabezpečené pripojenie
*	Vráti ukazovateľ na štruktúru typu BIO, ktorá je pripojená k serveru.
*	V prípade že sa nepodarí pripojiť nahrá chybu do premennej error a vráti NULL. 
*	
*/
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

/*
*	Zabezpečené pripojenie
*	Vráti ukazovateľ na štruktúru typu BIO, ktorá je pripojená k serveru.
*	V prípade že sa nepodarí pripojiť nahrá chybu do premennej error a vráti NULL. 
*	
*/
BIO *popcl_secured_connect(int *error){
	
	SSL_load_error_strings();
	SSL_library_init();
	ERR_load_BIO_strings();
	OpenSSL_add_all_algorithms();

	BIO * bio; SSL  *ssl;
	int ret;
	if((ret=setup_CTX())!=0){
		(*error)=ret;
		return NULL;
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

/*
*	Funkcia nastaví globálnu CTX premennú, 
*	ak sa nepodarí načítať certifikáty, vráti chybu.
*	
*/
int setup_CTX(){
	ctx = SSL_CTX_new(SSLv23_client_method());

	if((args.b_certfile || args.b_certdir) == 0){
		if(!SSL_CTX_set_default_verify_paths(ctx)){

			return _CANT_LOAD_CERT;
		}
	}

	else if((args.b_certdir && args.b_certfile) == 1){
		if(!SSL_CTX_load_verify_locations(ctx,args.c,args.C)){

			return _CANT_LOAD_CERT;
		}	
	}
	else if(args.b_certfile){
		if(!SSL_CTX_load_verify_locations(ctx,args.c,NULL)){

			return _CANT_LOAD_CERT;
		}
	}
	else{
		if(!SSL_CTX_load_verify_locations(ctx,NULL,args.C)){

			return _CANT_LOAD_CERT;
		}
	}

	return 0;
}



/*
*	Funkcia vytvorí novú bio štruktúru, ktorá komunikuje zabezpečenou formou po
*	zavolaní príkazu STLS na server
*
*/
int starttls_connection(BIO * bio){
	SSL_load_error_strings();
	SSL_library_init();
	ERR_load_BIO_strings();
	OpenSSL_add_all_algorithms();

	// Inicializuje SSL metodu 
	SSL  *ssl;

	int ret;
	if((ret=setup_CTX())!=0){
		return ret;
	}

	BIO *pushed=NULL,*new_ssl=NULL;

	// dva riadky z knižnice
	new_ssl = BIO_new_ssl(ctx,1);
	pushed = BIO_push(new_ssl,bio);

	BIO_get_ssl(pushed,&ssl);

	if(BIO_do_connect(bio) <= 0){
		return _CONNECTION_FAILED_TO_OPEN;
	}

	//overenie či je pripojenie v poriadku
	if(SSL_get_verify_result(ssl) != X509_V_OK)
	{
	    return _CONNECTION_FAILED_TO_OPEN;
	}


	return 0;
}