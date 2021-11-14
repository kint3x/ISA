/*
*	Implementácia POP3 klienta
*	Predmet: ISA
*	Autor: Martin Matějka <xmatej55@stud.fit.vutbr.cz>
*	Ročník: 3BIT
*
*/
#include <stdio.h>

#include "headers.h"
#include "args.h"
#include "popcl.h"
#include "err.h"
#include "connect.h"

int main(int argc,char **argv){

	int ret;
	stiahnute_spravy = 0;
	
	args_init(&args);
	if((ret = parse_args(argc,argv,&args)) != 0){
		printf("Program skončil s chybou %d a stiahol %d správ\n",ret,stiahnute_spravy);
		return 0;
	}
	
	ret = parse_authfile();
	if(ret != 0){
		printf("Program skončil s chybou %d a stiahol %d správ\n",ret,stiahnute_spravy);
		prg_free_memory();
		SSL_CTX_free(ctx);
		return 0;

	}




	ret = popcl_client();

	if(ret != 0){
		printf("Program skončil s chybou %d a stiahol %d správ\n",ret,stiahnute_spravy);

	}
	else{
		if(args.n){
			printf("Staženo %d nových zpráv\n",stiahnute_spravy);
		}else{
			printf("Staženo %d zpráv\n",stiahnute_spravy);
		}
	}

	prg_free_memory();
	SSL_CTX_free(ctx);
	return 0;
}