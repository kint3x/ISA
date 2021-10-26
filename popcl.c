
#include "popcl.h"
#include "args.h"
#include <stdio.h>
#include <stdlib.h>
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

	ret = download_emails(bio);
	CHECK_ret

	return 0;
}

int get_request_reply(char *buf,BIO * bio,int buff_size){
	int ret;
	WRITE_buf PRINT_buf 
	memset(buf, '\0', sizeof(char)*buff_size);
	READ_buf PRINT_buf

	return 0;
}

int download_emails(BIO *bio){
	char buf[BUFFER_SIZE];
	int ret;

	EMPTY_buf sprintf(buf,"STAT\r\n"); WRITE_buf EMPTY_buf
	READ_buf PRINT_buf


	
	int msg_cnt,total_size;

	parse_stats(buf,&msg_cnt,&total_size);

	int email_counts = msg_cnt;

	for(int i=1;i<=email_counts;i++){
		
		EMPTY_buf sprintf(buf,"LIST %d\r\n",i); 
		get_request_reply(buf,bio,BUFFER_SIZE);

		parse_stats(buf,&msg_cnt,&total_size);

		printf("%d %d\n",msg_cnt,total_size);

		int email_size = total_size;

		//get uid
		EMPTY_buf  sprintf(buf,"UIDL %d\r\n",i); 
		get_request_reply(buf,bio,BUFFER_SIZE);
		//parse uid
		parse_stats(buf,&msg_cnt,&total_size);
		EMPTY_buf sprintf(buf,"%d",total_size);

		char *filename = (char *) malloc(sizeof(char)*strlen(buf)+1);
		strcpy(filename,buf);

		printf("FILENAME %s\n",filename);

		//rozsirime o 100 charov pre odpoved servera
 		char *email_content = (char *) malloc(sizeof(char)*email_size+100);
 		printf("CREATING %d BYTES\n",email_size+100);

 		//get email
 		download_single_email(i,email_content,email_size+100,bio);

 		FILE *f;
 		EMPTY_buf sprintf(buf,"%s/%s",args.out,filename);
 		f = fopen(buf,"w");
 		if(f == NULL)
	    {
	        return _FILE_FAILURE;
	    }
	    //orezanie prikazu
	    char *without_header = email_content;
	    while(*without_header != '\n') without_header++;
	    without_header++;
	    *(without_header+email_size)='\0';

	    fputs(without_header, f);

	    fclose(f);


 		free(email_content);
		free(filename);
	}



	return 0;
}

int download_single_email(int num,char *buf,int buff_size,BIO * bio){
	char command[BUFFER_SIZE];
	int ret;

	sprintf(command,"RETR %d\r\n",num);

	if(BIO_write(bio, command, strlen(command)) <= 0) {
	 if(! BIO_should_retry(bio)){
	 	return _CONNECTION_FAILED;
	 } 
	 return _CONNECTION_FAILED;
	}

	ret = BIO_read(bio, buf, buff_size);
	if(ret == 0){
		return _CONNECTION_CLOSED;
	}
	else if(ret < 0){
		if(! BIO_should_retry(bio)){
			return _CONNECTION_FAILED;
		}
		return _CONNECTION_FAILED; 
	}


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

int parse_stats(char *buf,int *msg_count,int *total_size){

	char count[50];
	char *chelp=count;

	char *help=buf;
	// pretocit za +OK
	help = help + 4;

	while(*help != ' '){
		*chelp=*help;
		
		chelp++;
		help++;
	}

	*msg_count = atoi(count);
	*total_size = atoi(help++);


	return 0;
}