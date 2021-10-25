
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

	ret = popcl_login(bio);
	CHECK_ret

	return 0;
}


int popcl_login(BIO *bio){
	char buf[BUFFER_SIZE];
	int ret;

	sprintf(buf,"USER %s\r\n",args.user);
	WRITE_buf PRINT_buf EMPTY_buf
	READ_buf PRINT_buf 

	if(buf[0] != '+') return _BAD_USERNAME_OR_PASS;

	EMPTY_buf

	sprintf(buf, "PASS %s\r\n",args.pass);
	WRITE_buf PRINT_buf EMPTY_buf

	READ_buf PRINT_buf
	if(buf[0] != '+') return _BAD_USERNAME_OR_PASS;

	return 0;
}