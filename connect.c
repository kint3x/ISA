#include "connect.h"
#include "err.h"

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