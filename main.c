#include <stdio.h>
#include "tree.h"
#include "util.h"
void report(void);
int yyparse(void);
int main(int argc,char *argv[]){
	int ret;
	ret = yyparse();
	return ret;
}
