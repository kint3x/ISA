/*
*	Implementácia POP3 klienta
*	Predmet: ISA
*	Autor: Martin Matějka <xmatej55@stud.fit.vutbr.cz>
*	Ročník: 3BIT
*
*/
#include "popcl.h"
#include "args.h"
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include "connect.h"
#include "headers.h"
#include "err.h"

#define BUFFER_SIZE 1024

/**
 * 	Funkcia podľa argumentov zapína správny režim klienta. Pri chybe v klientovi vracia kód chyby.
 * 
 */
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


/**
 * Zabezpečená verzia klienta použije na pripojenie zabezpečené pripojenie k serveru.
 * Prihlási sa, stiahne emaily a uvolní bio. Pri chybe vracia kód chyby.
 * 
 **/
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

/**
 * STARTTLS verzia klienta použije na pripojenie nezabezpečené pripojenie k serveru.
 * Potom vyvolá SSL/TLS a prepne do zabezpečeného režimu stiahne emaily a uvolní bio. Pri chybe vracia kód chyby.
 * 
 **/
int popcl_client_starttls(){
	int ret=0;

	BIO * bio = popcl_unsecured_connect(&ret);
	CHECK_ret

	ret = starttls_connection(bio);
	CHECK_ret

	ret=starttls_connection(bio);
	CHECK_ret

	ret = popcl_login(bio);
	CHECK_ret

	ret = download_emails(bio);
	CHECK_ret

	BIO_free_all(bio);

	return 0;
}

/*
*	Nezabezpečená verzia sa prihlási, stiahne emaily a uvolní bio.
*
*/
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
/*
*	Pošle command zadaný do buffera a vráti v ňom odpoveď.
*/
int get_request_reply(char *buf,BIO * bio,int buff_size){
	int ret;
	WRITE_buf PRINT_buf 
	memset(buf, '\0', sizeof(char)*buff_size);
	READ_buf PRINT_buf

	return 0;
}

/*
*	Funkcia stiahne všetky emaily zo servera na základe zadaných argumentov
*
*/
int download_emails(BIO *bio){
	char buf[BUFFER_SIZE];
	int ret;

	//zistí kolko emailov je v schránke
	EMPTY_buf sprintf(buf,"STAT\r\n"); WRITE_buf PRINT_buf EMPTY_buf
	READ_buf PRINT_buf


	
	int msg_cnt,total_size;

	//parsuje odpoved z príkazu STAT
	ret = parse_stats(buf,&msg_cnt,&total_size);
	CHECK_ret

	int email_counts = msg_cnt;

	for(int i=1;i<=email_counts;i++){
		
		// zisti o emaile velkost
		EMPTY_buf sprintf(buf,"LIST %d\r\n",i); 
		get_request_reply(buf,bio,BUFFER_SIZE);

		if((ret = parse_stats(buf,&msg_cnt,&total_size)) != 0) return ret;

		int email_size = total_size;

		//ziska uid emailu
		EMPTY_buf  sprintf(buf,"UIDL %d\r\n",i); 
		get_request_reply(buf,bio,BUFFER_SIZE);
		//parsuje uid 
		char UIDL[80];
		if((ret = parse_reply(buf,&msg_cnt,UIDL)) != 0) return ret;

		// ulozi si nazov do premennej
		char *filename = (char *) malloc(sizeof(char)*strlen(buf)+1);
		strcpy(filename,UIDL);

 		FILE *f;
 		//nastavi cestu k suboru
 		EMPTY_buf sprintf(buf,"%s/%s",args.out,filename);

 		if(check_if_file_exists(buf)){ 
 			if(args.n){ // ak je parameter, ze má pracovat len s novými emailami a subor s tymto menom uz existuje, preskoci ho
 				
				free(filename);
				continue;
 			}

 		}

 		stiahnute_spravy++;

 		//rozsirim o 100 charov pre odpoved servera kvôli rezerve
 		char *email_content = (char *) malloc(sizeof(char)*email_size+100);

 		//stiahni email
 		ret=download_single_email(i,email_content,email_size,100,bio);
 		CHECK_ret

 		// zapisanie do suboru
 		f = fopen(buf,"w");
 		if(f == NULL)
	    {
	        return _FILE_FAILURE;
	    }
		
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
/*
*	Zistí, či súbor už existuje
*
*/
int check_if_file_exists(char *file){
	if( access( file, F_OK ) == 0 ) {
    	return 1;
	} else {
	   	return 0;
	}
}

/**
 * 	Stiahne email po blokoch do premennej message, odstranuje znaky pridané serverom podla protokolu POP3
 * 
 **/
int download_single_email(int num,char *message,int message_size,int spacing,BIO * bio){
	char buf[BUFFER_SIZE];
	int ret;
	int rode_bytes;
	int cutted;

	sprintf(buf,"RETR %d\r\n",num); PRINT_buf;
	WRITE_buf EMPTY_buf;
	
	READ_buf CHECK_buf_reply

	char *p=buf;
	// preskočenie odpovede servera
	while(*(p) != '\n'){	
		ret--;
		p++;
	}
	p++;
	ret--;

	strcpy(message,p);
	rode_bytes=ret;

	EMPTY_buf;
	//pokial neprečítam celú správu
	do {

		READ_buf
		//odstrani bodky, ktoré pridáva protokol POP3
		cutted = cut_buffer_lines(buf);
		strcpy(message+rode_bytes,buf);
		rode_bytes += ret-cutted;
	
		//PRINT_buf
		EMPTY_buf
		
	} while(rode_bytes < message_size);

	//oreže správu na správnu veľkosť a odstráni .CRLF trio 
	*(message+message_size)='\0';
	

	return 0;

}
/*
*	Funkcia hľadá v premennej buf postupnosť znakov \n.. a nahradzuje .. na . 
*	vracia počet nahradených bodiek
*/
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
				}
			}
		}
		buf++;
	}

	return cut;
}
/**
 *	Funkcia sa prihlási pomocou parsovaných údajov odložených v argument štruktúre 
 * 
*/
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

/*
*	Parsuje odpoveď servera v tvare +OK X Y a nastavuje X a Y cez pointer msg_coun a total_size
*	Ak nastane chybná odpoved vracia chybový kod inak 0
*/

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

/*
*	Parsuje odpoveď servera v tvare +OK X Y a nastavuje X a Y cez pointer msg_coun a total_size
*	Ak nastane chybná odpoved vracia chybový kod inak 0, rozdiel od predošlej funkcie je, že druhý parameter je * char
*/
int parse_reply(char *buf,int *msg_count,char *total_size){

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
	help++;
	*msg_count = atoi(count);

	help[strlen(help)-2] = '\0';
	strcpy(total_size,help);

	if(buf[0] == '-') return _REPLAY_BAD;
	return 0;
}