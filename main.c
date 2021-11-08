#include <stdio.h>

#include "headers.h"
#include "args.h"
#include "popcl.h"
#include "err.h"


int main(int argc,char **argv){

	int ret;
	stiahnute_spravy = 0;
	
	args_init(&args);
	if((ret = parse_args(argc,argv,&args)) != 0){
		printf("CHYBA %d\n",ret);
		return 0;
	}
	
	ret = parse_authfile();
	




	ret = popcl_client();

	printf("Aplikacia skoncila s kodom %d a stiahla %d sprav\n",ret,stiahnute_spravy);

	if(args.n){
		printf("Staženo %d nových zpráv\n",stiahnute_spravy);
	}else{
		printf("Staženo %d zpráv\n",stiahnute_spravy);
	}

	prg_free_memory();

	return 0;
}