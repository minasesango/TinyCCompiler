#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>
#include "tree.h"
#include "util.h"
#include "tcc.tab.h"

extern token symtab;
extern int yylineno;
extern int semerrs;

void error(char *fmt,...){
	va_list argp;
	va_start(argp,fmt);
	semerrs++;
	fprintf(stderr,"%d: error: ",yylineno);
	vfprintf(stderr,fmt,argp);
	fprintf(stderr,"\n");
	va_end(argp);
}
void warn(char *fmt,...){
	va_list argp;
	va_start(argp,fmt);
	fprintf(stderr,"%d: warning: ",yylineno);
	vfprintf(stderr,fmt,argp);
	fprintf(stderr,"\n");
	va_end(argp);
}
char* get_kind_of(int kind){
		switch(kind){
			case FRESH:
				return "FRESH";
				break;
			case VAR:
				return "VAR";
				break;
			case FUN:
				return "FUN";
				break; 
			case PARM:
				return "PARM";
				break;
			case UNDEFFUN:
				return "UNDEFFUN";
				break;
			default:
				return "(UNKNOWN)";
				break;
		}
}
char *itos(int i){
	char buf[32];
	sprintf(buf,"%d",i);
	return strdup(buf);
}


