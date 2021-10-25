#include <stdio.h>

#include "headers.h"
#include "args.h"
#include "popcl.h"
#include "err.h"


int main(int argc,char **argv){

	int ret;
	
	args_init(&args);
	if((ret = parse_args(argc,argv,&args)) != 0){
		printf("CHYBA %d\n",ret);
		return 0;
	}
	
	ret = parse_authfile();
	




	ret = popcl_client();

	printf("Aplikacia skoncila s kodom %d\n",ret);

	prg_free_memory();

	return 0;
}