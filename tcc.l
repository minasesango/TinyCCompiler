%option noyywrap
%option yylineno
%{
#include <stdio.h>
#include <string.h>
#include "tree.h"
#include "tcc.tab.h"
int yyerror(char*);
%}

digit	[0-9]

%%
"int"	return TKN_INT;
"if"	return TKN_IF;
"else"	return TKN_ELSE;
"while"	return TKN_WHILE;
"return"	return TKN_RETURN;
"||"	return TKN_OR;
"&&"	return TKN_AND;
"=="	return TKN_EQ;
"!="	return TKN_NEQ;
">="	return TKN_GE;	/*greater than or equal to*/
"<="	return TKN_LE;	/*less than or equal to*/

";"|"{"|"}"|"("|")"|","|"+"|"-"|"*"|"/"|">"|"<"|"="	{
	return *yytext;
       }

{digit}+	{
			yylval.i = atoi(yytext);
			return CONST;
       		}

[_a-zA-Z][_a-zA-Z0-9]* {
			yylval.str = strdup(yytext);
			return TKN_IDENTIFIER;
     		}

[ \t\r\n]	;
.	yyerror("Error: incalid character");
%%
