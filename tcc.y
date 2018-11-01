%{
#include <stdio.h>
#include "tree.h"
#include "util.h"
#include "emit.h"

token symtab;	
int cur_lev;
int semerrs;
int g_current_fun_parms;
int g_current_ref_fun_parms;
%}
%error_verbose
%union{
	int i;
	char *str;
	tree n;
}
%token<str> TKN_IDENTIFIER 
%token<i> CONST
%token<i> FUNDEF CONS CMPDSTM FCALL
%token<i> TKN_INT TKN_IF TKN_ELSE TKN_WHILE TKN_RETURN
%token<i> TKN_OR TKN_AND 
%token<i> TKN_EQ TKN_NEQ TKN_LE TKN_GE

%type <n> program
%type <n> external_declaration
%type <n> declaration
%type <n> declarator_list
%type <n> declarator
%type <n> function_definition
%type <n> parameter_type_list
%type <n> parameter_declaration
%type <n> statement
%type <n> compound_statement
%type <n> declaration_list
%type <n> statement_list
%type <n> expression
%type <n> assignment_expr
%type <n> logical_OR_expr
%type <n> logical_AND_expr
%type <n> equality_expr
%type <n> relational_expr
%type <n> additive_expr
%type <n> multiplicative_expr
%type <n> unary_expr
%type <n> postfix_expr
%type <n> primary_expr
%type <n> argument_expression_list

%type <n> identifier_as_tree constant_as_tree
%%
main:
		{ symtab = NULL; cur_lev = 0; }
	program			       
		{ if (yynerrs == 0 && semerrs == 0){ 
				emit_program($2);
				print_code();
				}
		}
	;
program:
	external_declaration
	| program external_declaration
		 { $$ = make_tuple(CONS,$1,$2,NULL,NULL); }
	;
external_declaration:
	declaration
	| function_definition {$$=$1;}
	;
declaration:
	TKN_INT declarator_list ';'
		{ $$ = make_tuple(TKN_INT,$2,NULL,NULL,NULL); }
	;
declarator_list:
	declarator {$$ = make_decl($1);}
	| declarator_list ',' declarator
		{ $$ = make_tuple(CONS,$1,make_decl($3),NULL,NULL); }
	;
declarator:
	identifier_as_tree
	;
function_definition:
	TKN_INT declarator '(' {cur_lev++; g_current_fun_parms=1;} parameter_type_list {make_fun_def($2);}')' compound_statement
		{ 
			$$ = make_tuple(FUNDEF,$2,$5,$8,NULL); 
			pop_token_lev_gt(cur_lev-1);
			cur_lev--;
		}
	| TKN_INT declarator '(' {cur_lev++;g_current_fun_parms=0;make_fun_def($2);}')' compound_statement
		{ 
			$$=make_tuple(FUNDEF,$2,NULL,$6,NULL);
			pop_token_lev_gt(cur_lev-1);
		       	cur_lev--;
		}
	;
parameter_type_list:
	parameter_declaration 
	| parameter_type_list ',' parameter_declaration
		{ $$=make_tuple(CONS,$1,$3,NULL,NULL);g_current_fun_parms++; }
	;
parameter_declaration:
	TKN_INT declarator
		{ $$=make_tuple(TKN_INT,make_parm_decl($2),NULL,NULL,NULL); }
	;
statement:
	';'
		{ }
	| expression ';'
		{ $$=$1; }
	| compound_statement
	| TKN_IF '(' expression ')' statement
		{ $$=make_tuple(TKN_IF,$3,$5,NULL,NULL); }
	| TKN_IF '(' expression ')' statement TKN_ELSE statement
		{ $$=make_tuple(TKN_IF,$3,$5,$7,NULL); }
	| TKN_WHILE '(' expression ')' statement
		{ $$=make_tuple(TKN_WHILE,$3,$5,NULL,NULL); }
	| TKN_RETURN expression ';'
		{ $$=make_tuple(TKN_RETURN,$2,NULL,NULL,NULL); }
	| TKN_RETURN ';'
		{ $$=make_tuple(TKN_RETURN,NULL,NULL,NULL,NULL); }
	;
compound_statement:
	'{' {cur_lev++;} declaration_list statement_list '}'
		{ 
			$$=make_tuple(CMPDSTM,$3,$4,NULL,NULL); 
			pop_token_lev_gt(cur_lev-1);
			cur_lev--;
		}
	| '{' {cur_lev++;} declaration_list '}' 
		{ 
			$$=make_tuple(CMPDSTM,$3,NULL,NULL,NULL); 
			pop_token_lev_gt(cur_lev-1);
			cur_lev--;
		}
	| '{' {cur_lev++;} statement_list '}'
		{ 
			$$=make_tuple(CMPDSTM,NULL,$3,NULL,NULL); 
			pop_token_lev_gt(cur_lev-1);
			cur_lev--;
		}
	| '{' {cur_lev++;} '}'
		{ 
			$$=make_tuple(CMPDSTM,NULL,NULL,NULL,NULL); 
			pop_token_lev_gt(cur_lev-1);
			cur_lev--;
		}
	;
declaration_list:
	declaration 
	| declaration_list declaration
	{ $$=make_tuple(CONS,$1,$2,NULL,NULL); }
	;
statement_list:
	statement
	| statement_list statement
	{ $$=make_tuple(CONS,$1,$2,NULL,NULL); }
	;
expression:
	assignment_expr
	| expression ',' assignment_expr
	{ $$=make_tuple(CONS,$1,$3,NULL,NULL); }
	;
assignment_expr:
	logical_OR_expr
	| identifier_as_tree '=' assignment_expr
	{ $$=make_tuple('=',$1,$3,NULL,NULL); }
	;
logical_OR_expr:
	logical_AND_expr
	| logical_OR_expr TKN_OR logical_AND_expr
	{ $$=make_tuple(TKN_OR,$1,$3,NULL,NULL); }
	;
logical_AND_expr:
	equality_expr
	| logical_AND_expr TKN_AND equality_expr
	{ $$=make_tuple(TKN_AND,$1,$3,NULL,NULL); }
	;
equality_expr:
	relational_expr
	| equality_expr TKN_EQ relational_expr
	{ $$=make_tuple(TKN_EQ,$1,$3,NULL,NULL); }
	| equality_expr TKN_NEQ relational_expr
	{ $$=make_tuple(TKN_NEQ,$1,$3,NULL,NULL); }
	;
relational_expr:
	additive_expr
	| relational_expr '<' additive_expr
	{ $$=make_tuple('<',$1,$3,NULL,NULL); }
	| relational_expr '>' additive_expr
	{ $$=make_tuple('>',$1,$3,NULL,NULL); }
	| relational_expr TKN_LE additive_expr
	{ $$=make_tuple(TKN_LE,$1,$3,NULL,NULL); }
	| relational_expr TKN_GE additive_expr
	{ $$=make_tuple(TKN_GE,$1,$3,NULL,NULL); }
	;
additive_expr:
	multiplicative_expr
	| additive_expr '+' multiplicative_expr
	{ $$=make_tuple('+',$1,$3,NULL,NULL); }
	| additive_expr '-' multiplicative_expr
	{ $$=make_tuple('-',$1,$3,NULL,NULL); }
	;
multiplicative_expr:
	unary_expr
	| multiplicative_expr '*' unary_expr 
	{ $$=make_tuple('*',$1,$3,NULL,NULL); }
	| multiplicative_expr '/' unary_expr
	{ $$=make_tuple('/',$1,$3,NULL,NULL); }
	;
unary_expr:
	postfix_expr
	| '-' unary_expr
	{ $$=make_tuple('-',$2,NULL,NULL,NULL); }
	;
postfix_expr:
	primary_expr
	| identifier_as_tree'(' {} argument_expression_list ')'
	{ $$ = make_tuple(FCALL,ref_fun($1),$4,NULL,NULL);g_current_ref_fun_parms=1;}
	| identifier_as_tree '(' {g_current_ref_fun_parms=0;} ')'
	{ $$ = make_tuple(FCALL,ref_fun($1),NULL,NULL,NULL);}
	;
primary_expr:
	identifier_as_tree { $$ = ref_var($1); }
	| constant_as_tree		
	| '(' expression ')'
	{ $$ = $2; }
	;
argument_expression_list:
	assignment_expr { g_current_ref_fun_parms=1; }
	| argument_expression_list ',' assignment_expr
	{ $$ = make_tuple(CONS,$1,$3,NULL,NULL); g_current_ref_fun_parms++;}
	;
identifier_as_tree:
	TKN_IDENTIFIER { 
		tree defined;
		if((defined = lookup_sym($1))==NULL)
			$$ = make_token_node($1);
		else
			$$ = defined;
	}
	;
constant_as_tree:
	CONST { $$=make_constant_node($1); }
	;
%%
extern int yylineno;
int yyerror(char *s){
	fprintf(stderr,"yyerror(): %d: %s\n",yylineno,s);
	return 0;
}

