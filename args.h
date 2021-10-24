#ifndef _ARGS_H
#define _ARGS_H

#include <stdbool.h>

typedef struct arguments{

	char *server;
	int port;
	bool T,S;
	char *c,*C,*auth_file,*out;
	bool d,n;

	bool b_auth, b_out,b_certfile,b_certdir,b_port;


} ARGUMENTS;

//globalna premenna na ukladanie argumentov
ARGUMENTS args;

int parse_args(int argc,char **argv,ARGUMENTS *args);
void args_init(ARGUMENTS *args);
void print_args(ARGUMENTS args);

#endif