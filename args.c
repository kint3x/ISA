#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <stdlib.h>
#include <getopt.h>
#include <ctype.h>
#include <dirent.h>
#include <errno.h>

#include "headers.h"
#include "args.h"
#include "err.h"

#define BUFFER_SIZE 1024


int isNumber(char s[])
{
    for (int i = 0; s[i]!= '\0'; i++)
    {
        if (isdigit(s[i]) == 0)
              return 0;
    }
    return 1;
}

int parse_args(int argc,char **argv,ARGUMENTS *args){
	// parsovanie argumentov
	int cmd_count=0;

	args->server = argv[1];

	while(1){
		static struct option long_options[]=
		{
			{"T",     no_argument,      0, 'T'},
			{"S",     no_argument,      0, 'S'},
			{"d",     no_argument,      0, 'd'},
			{"n",     no_argument,      0, 'n'},
			{"p",     required_argument,0, 'p'},
			{"a",     required_argument,0, 'a'},
			{"o",     required_argument,0, 'o'},

			{"c",     required_argument,0, 'c'},
			{"C",     required_argument,0, 'C'},



		};

		int option_index = 0;
		int c;
      	c = getopt_long (argc, argv, "TSdnp:a:o:c:C:",
                       long_options, &option_index);

		if (c == -1) break;


		switch(c){
			case 'T':
				args->T = true;
				cmd_count++;
				break;
			case 'S':
				args->S = true;
				cmd_count++;
				break;
			case 'd':
				args->d = true;
				cmd_count++;
				break;
			case 'n':
				args->n = true;
				cmd_count++;
				break;
			case 'p':
				if(!isNumber(optarg)) return 1; // 1 - port nie je číslo
				int port = atoi(optarg);
				if(port<=0) return 2; // neplatny port
				args->port = port;
				args->b_port=true;
				cmd_count= cmd_count +2;
				break;
			case 'a':
				args->auth_file = optarg;
				args->b_auth = true;
				cmd_count= cmd_count +2;
				break;
			case 'o':
				args->out = optarg;
				args->b_out = true;
				cmd_count= cmd_count +2;
				break;	
			case 'c':
				args->c = optarg;
				args->b_certfile = true;
				cmd_count= cmd_count +2;
				break;	
			case 'C':
				args->C = optarg;
				args->b_certdir = true;
				cmd_count= cmd_count +2;
				break;	

			case '?':
				return 3; // neznamy argument alebo zly argument

		}



	}

	//povinne parametre
	if(!args->b_auth) return 12;
	if(!args->b_out) return 13;

	//check out dir
	DIR* dir = opendir(args->out);
	if (dir) {
	    /* Directory exists. */
	    closedir(dir);
	} else if (ENOENT == errno) {
	    return 10; //neda sa otvorit zlozka
	} else {
	    return 10; //neda sa otvorit zlozka
	}



	// nastavenia dynamickej logiky
	if(cmd_count!=argc-2) return 10; // chyba hostname
	

	if(args->T && args->S) return 11; // nemozu byt prepinace t a s naraz

	if((args->C || args->c ) && args->T) return 12; // nemozu byt T a c alebo C argumenty

	//port nastavenie
	if(!args->b_port){
		if(args->T) args->port = 995;
		if(args->S) args->port = 110;
	}
	

	return 0;
}


void args_init(ARGUMENTS *args){

	args->S = false; args->T = false; args->c=NULL;
	args->C = NULL; args->auth_file = NULL;
	args->out = NULL;
	args->d = false; args->n = false;
	args->b_out = false; args->b_auth = false; args->b_certdir = false;
	args->b_port = false;
	args->b_certfile = false;
	args->port = 110;
	args->user = NULL;
	args->pass = NULL;

}

void print_args(ARGUMENTS args){
	printf("SERVER: %s",args.server);
	printf("\nPORT: %d",args.port);
	printf("\nFLAGS: ");
	if(args.S) printf("S ");
	if(args.T) printf("T ");
	if(args.d) printf("d ");
	if(args.n) printf("n ");
	printf("\nCERTFILE: %s",args.c);
	printf("\nCERTDIR: %s",args.C);
	printf("\nAUTHFILE: %s",args.auth_file);
	printf("\nOUTDIR: %s",args.out);

	printf("\n");
}

int parse_authfile(){
	FILE *fp;

	fp = fopen(args.auth_file,"r");
	if(fp == NULL){
		return _FILE_NOT_EXIST;
	}

	char buf[BUFFER_SIZE];
	char *ptr = buf;
	char ch;
	while((ch = fgetc(fp)) != EOF){
      if(ptr == buf+BUFFER_SIZE) return _FILE_TOO_BIG;
      if(ch == ' ') continue;
      sprintf(ptr,"%c", ch);
      ptr++;
	}

	char *help;
	
	//parse username
	
	char string[BUFFER_SIZE/2];
	help = string;
	char *find = strstr(buf,"username=");

	if(find == NULL){
		return _BAD_AUTHFILE_FORMAT;
	}
	//delete username=
	find = find+9;
	while((ch=(char)*(find++))!='\n'){
		if(ch == EOF) return _BAD_AUTHFILE_FORMAT;
		sprintf(help,"%c",ch);
		help++;
	}

	args.user = (char *) malloc(sizeof(char)*strlen(string)+1);
	strcpy(args.user,string);

	//parse password

	memset(string, '\0', sizeof(string));
	help = string;
	find = strstr(buf,"password=");

	if(find == NULL){
		return _BAD_AUTHFILE_FORMAT;
	}
	//delete username=
	find = find+9;
	while((ch=(char)*(find++))!='\n'){
		if(ch == EOF) return _BAD_AUTHFILE_FORMAT;
		sprintf(help,"%c",ch);
		help++;
	}

	args.pass = (char *) malloc(sizeof(char)*strlen(string)+1);
	strcpy(args.pass,string);



  	fclose(fp);

  	return 0;
}

void prg_free_memory(){


	free(args.pass);
	free(args.user);

}