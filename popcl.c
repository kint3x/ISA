
#include "popcl.h"
#include "args.h"
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include "connect.h"
#include "headers.h"
#include "err.h"

#define BUFFER_SIZE 1024

int popcl_client(){

	int ret;

	if((args.S || args.T) == false){
		ret = popcl_client_unsecure();
		CHECK_ret
	}
	
	if(args.T){
		ret = popcl_client_secure();
		CHECK_ret
	}
	if(args.S){
		ret = popcl_client_starttls();
		CHECK_ret
	}
	

	return 0;
}

int popcl_client_secure(){
	int ret=0;

	BIO * bio = popcl_secured_connect(&ret);
	CHECK_ret

	
	ret = popcl_login(bio);
	CHECK_ret

	ret = download_emails(bio);
	CHECK_ret

	BIO_free_all(bio);

	return 0;
}


int popcl_client_starttls(){
	int ret=0;

	printf("STARTING UNSECURED\n");
	BIO * bio = popcl_unsecured_connect(&ret);
	CHECK_ret

	ret = starttls_connection(bio);
	CHECK_ret

	printf("OK\n");
	printf("STARTING SECURED\n");

	ret=starttls_connection(bio);
	CHECK_ret

	printf("SECURED CONNECTION\n");
	ret = popcl_login(bio);
	CHECK_ret

	ret = download_emails(bio);
	CHECK_ret

	BIO_free_all(bio);

	return 0;
}

int popcl_client_unsecure(){
	int ret;

	BIO * bio = popcl_unsecured_connect(&ret);
	CHECK_ret

	ret = popcl_login(bio);
	CHECK_ret

	ret = download_emails(bio);
	CHECK_ret

	BIO_free_all(bio);
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

	EMPTY_buf sprintf(buf,"STAT\r\n"); WRITE_buf PRINT_buf EMPTY_buf
	READ_buf PRINT_buf


	
	int msg_cnt,total_size;

	parse_stats(buf,&msg_cnt,&total_size);

	int email_counts = msg_cnt;

	for(int i=1;i<=email_counts;i++){
		
		EMPTY_buf sprintf(buf,"LIST %d\r\n",i); 
		get_request_reply(buf,bio,BUFFER_SIZE);

		if((ret = parse_stats(buf,&msg_cnt,&total_size)) != 0) return ret;

		printf("%d %d\n",msg_cnt,total_size);

		int email_size = total_size;

		//get uid
		EMPTY_buf  sprintf(buf,"UIDL %d\r\n",i); 
		get_request_reply(buf,bio,BUFFER_SIZE);
		//parse uid
		if((ret = parse_stats(buf,&msg_cnt,&total_size)) != 0) return ret;
		EMPTY_buf sprintf(buf,"%d",total_size);


		char *filename = (char *) malloc(sizeof(char)*strlen(buf)+1);
		strcpy(filename,buf);

		printf("FILENAME %s\n",filename);

 		FILE *f;
 		EMPTY_buf sprintf(buf,"%s/%s",args.out,filename);

 		if(check_if_file_exists(buf)){ 
 			if(args.n){
 				
				free(filename);
				continue;
 			}

 		}

 		stiahnute_spravy++;

 		//rozsirime o 500 charov pre odpoved servera
 		char *email_content = (char *) malloc(sizeof(char)*email_size+100);
 		printf("CREATING %d BYTES\n",email_size+100);

 		//get email
 		download_single_email(i,email_content,email_size,100,bio);

 		
 		f = fopen(buf,"w");
 		if(f == NULL)
	    {
	        return _FILE_FAILURE;
	    }
	    
	    /*//orezanie mailu aby obsahoval len data
	    char *without_header = email_content;
	    while(*without_header != '\n') without_header++;
	    without_header++;
	    *(without_header+email_size)='\0';
		*/
	    fprintf(f,"%s",email_content);

	    fclose(f);
	    //vymaze mail po stiahnuti
	    if(args.d){
	    	EMPTY_buf sprintf(buf,"DELE %d\r\n",i); WRITE_buf PRINT_buf
	    	EMPTY_buf READ_buf PRINT_buf
	    }

 		free(email_content);
		free(filename);
	}


	EMPTY_buf sprintf(buf,"QUIT\r\n");WRITE_buf PRINT_buf
	EMPTY_buf READ_buf PRINT_buf


	return 0;
}

int check_if_file_exists(char *file){
	if( access( file, F_OK ) == 0 ) {
    	return 1;
	} else {
	   	return 0;
	}
}

int download_single_email(int num,char *message,int message_size,int spacing,BIO * bio){
	char buf[BUFFER_SIZE];
	int ret;
	int rode_bytes;
	int cutted;

	sprintf(buf,"RETR %d\r\n",num); PRINT_buf;
	WRITE_buf EMPTY_buf;
	
	READ_buf CHECK_buf_reply

	char *p=buf;

	while(*(p) != '\n'){	
		ret--;
		p++;
	}
	p++;
	ret--;

	strcpy(message,p);
	rode_bytes=ret;

	EMPTY_buf;

	do {

		READ_buf
		cutted = cut_buffer_lines(buf);
		strcpy(message+rode_bytes,buf);
		rode_bytes += ret-cutted;
		printf("RODE %d bytes out of %d\n",rode_bytes,message_size);
		printf("\n ------------------------- BLOCK START-------------\n");
		PRINT_buf
		printf("\n ------------------------- BLOCK END --------------\n");
		EMPTY_buf
		
		//-3 for .CRLF trio
	} while(rode_bytes < message_size);
	*(message+message_size)='\0';
	printf("ENDED %d < %d\n",rode_bytes,message_size);

	return 0;

}
int cut_buffer_lines(char *buf){
	int cut = 0;
	char *h;
	while(*buf != '\0'){
		if((*buf) == '\n'){
			if(*(buf+1) == 46){
				if(*(buf+2) == 46){
					h=buf+1;
					do{
						*h=*(h+1);
						h++;
					}while(*(h+1) != '\0');
					cut++;
					printf("FOUND OCCURENCE %c%c%c %d %d %d\n",*(buf+1),*(buf+2),*(buf+3),*(buf+1),*(buf+2),*(buf+3));
				}
			}
		}
		buf++;
	}

	return cut;
}
int popcl_login(BIO *bio){
	char buf[BUFFER_SIZE];
	int ret=0;
	
	READ_buf PRINT_buf EMPTY_buf


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

	if(buf[0] == '-') return _REPLAY_BAD;
	return 0;
}