
#include "popcl.h"
#include "args.h"
#include <stdio.h>
#include "connect.h"
#include "headers.h"
#include "err.h"

#define BUFFER_SIZE 1024

int popcl_client(){

	int ret;

	if((args.S || args.T) == false){
		ret =popcl_client_unsecure();
		CHECK_ret
	}
	

	return 0;
}

int popcl_client_unsecure(){
	int ret;

	BIO * bio = popcl_unsecured_connect(&ret);
	CHECK_ret

	char buf[BUFFER_SIZE];


	READ_buf

	printf("RESPONSE: %s",buf);
	
	EMPTY_buf

	sprintf(buf,"USER skauti@80zbor.skauting.sk\r\n");

	WRITE_buf

	EMPTY_buf

	READ_buf

	if(buf[0] != '+') return _REPLAY_BAD;

	printf("RESPONSE: %s",buf);
	EMPTY_buf 
	sprintf(buf,"PASS Skautingnasbavi1\r\n");
	WRITE_buf
	EMPTY_buf 
	READ_buf

	printf("RESPONSE: %s",buf);

	if(buf[0] != '+') return _REPLAY_BAD;

	EMPTY_buf 

	return 0;
}
