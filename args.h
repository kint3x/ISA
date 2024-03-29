/*
*	Implementácia POP3 klienta
*	Predmet: ISA
*	Autor: Martin Matějka <xmatej55@stud.fit.vutbr.cz>
*	Ročník: 3BIT
*
*/
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

	char *user,*pass;


} ARGUMENTS;

//globalna premenna na ukladanie argumentov
ARGUMENTS args;

//premenna na ukladanie stiahnutych sprav
int stiahnute_spravy;

int parse_args(int argc,char **argv,ARGUMENTS *args);
void args_init(ARGUMENTS *args);
void print_args(ARGUMENTS args);
int parse_authfile();
void prg_free_memory();

#endif