#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>
#include "tree.h"
#include "tcc.tab.h"
#include "util.h"
#include "emit.h"

struct code{
	char *cd;
	struct code *next;
};

char *g_fun_name = NULL;
char *g_ret_label = NULL;
int g_top_alloc = 0;
int g_last_alloc = 0;
int g_fcall_args = 0;
int allocate_loc(void);
void release_loc(void);
char *get_loc_as_str(int);


struct code *g_cdtab = NULL;
struct code *cdtab_add(struct code *c);
struct code *emit(char *inst,char *op1,char *op2);
char *make_label(const char *additional,const char *additional2);
struct code *emit_label(char *lab);
struct code *emit_comment(const char *);
struct code *emit_global(const char *s);
struct code *emit_extern(const char *s);
struct code *emit_common(const char *s);

void emit_code_external_declaration(tree);
void emit_code_declaration(tree);
void emit_code_declarator_list(tree);
void emit_code_declarator(tree);
void emit_code_function_definition(tree);
void emit_code_parameter_type_list(tree);
void emit_code_parameter_declaration(tree);
void emit_code_statement(tree);
void emit_code_compound_statement(tree);
void emit_code_declaration_list(tree);
void emit_code_statement_list(tree);
void emit_code_expression(tree);
void emit_code_assignment_expr(tree);
void emit_code_logical_OR_expr(tree);
void emit_code_logical_AND_expr(tree);
void emit_code_equality_expr(tree);
void emit_code_relational_expr(tree);
void emit_code_additive_expr(tree);
void emit_code_multiplicative_expr(tree);
void emit_code_unary_expr(tree);
void emit_code_postfix_expr(tree);
void emit_code_primary_expr(tree);
void emit_code_argument_expression_list(tree);

void emit_code_identifier(tree);
void emit_code_constant(tree);
char *get_declarator(tree p);
char *get_identifier_loc(tree);
char *get_identifier(tree);
char *get_constant(tree);


int allocate_loc(void){
	g_last_alloc -= 4;
	if(g_last_alloc < g_top_alloc)
		g_top_alloc = g_last_alloc;
	return g_last_alloc;
}

void release_loc(void){
	g_last_alloc += 4;
}

char *get_loc_as_str(int loc){
	char buf[64];
		sprintf(buf," [ebp%s%d] : (temporal variable)",
				((loc >=0) ? "+" : ""),
				loc);
	emit_comment(buf);
	sprintf(buf,"dword [ebp%s%d]",
			((loc >=0) ? "+" : ""),
			loc);
	return strdup(buf);
}

struct code *cdtab_add(struct code *c){
	struct code *it;
	if(g_cdtab == NULL)
		g_cdtab = c;
	else{
		for(it = g_cdtab; it->next != NULL; it = it->next);
		it->next = c;
	}
	return g_cdtab;
}

struct code *emit(char *inst,char *op1,char *op2){
	char buf[80];
	struct code *c = (struct code *)malloc(sizeof(struct code));

	if(inst == NULL)
		buf[0] = '\0';
	else if(op1 == NULL)
		sprintf(buf,"\t%s\n",inst);
	else if(op2 == NULL)
		sprintf(buf,"\t%s\t%s\n",inst,op1);
	else
		sprintf(buf,"\t%s\t%s, %s\n",inst,op1,op2);
	c->cd = strdup(buf);
	c->next = NULL;
	cdtab_add(c);
	return c;
}
char *make_label(const char *additional,const char *additional2){
	static int n = 0;
	char *buf = (char *)malloc(sizeof(char)*
			(strlen(additional) + strlen(additional2) + 8));
	sprintf(buf,"L%d_%s_%s",n,additional,additional2);
	n++;
	return buf;
}
struct code* emit_label(char *lab){
	struct code *c = (struct code *)malloc(sizeof(struct code));
	char *buf = (char *)malloc(sizeof(char) * strlen(lab) + 8);
	sprintf(buf,"%s:\n",lab);
	c->cd = buf;
	c->next = NULL;
	cdtab_add(c);
	return c;
}
struct code* emit_comment(const char *s){
	struct code *c = (struct code *)malloc(sizeof(struct code));
	char *buf = (char*)malloc(sizeof(char)*(strlen(s) + 5));
	sprintf(buf,";%s\n",s);
	c->cd = buf;
	c->next = NULL;
	cdtab_add(c);
	return c;
}
struct code* emit_global(const char *s){
	struct code *c = (struct code *)malloc(sizeof(struct code));
	char *buf = (char*)malloc(sizeof(char)*(strlen(s) + 16));
	sprintf(buf,"\tGLOBAL\t%s\n",s);
	c->cd = buf;
	c->next = NULL;
	cdtab_add(c);
	return c;
}
struct code* emit_extern(const char *s){
	struct code *c = (struct code *)malloc(sizeof(struct code));
	char *buf = (char*)malloc(sizeof(char)*(strlen(s) + 16));
	sprintf(buf,"\tEXTERN\t%s\n",s);
	c->cd = buf;
	c->next = NULL;
	cdtab_add(c);
	return c;
}
struct code* emit_common(const char *s){
	struct code *c = (struct code *)malloc(sizeof(struct code));
	char *buf = (char*)malloc(sizeof(char)*(strlen(s) + 16));
	sprintf(buf,"\tCOMMON\t%s 4\n",s);
	c->cd = buf;
	c->next = NULL;
	cdtab_add(c);
	return c;
}



void print_code(void){
	struct code *it;
	for(it = g_cdtab; it != NULL; it = it->next)
		if(it->cd != NULL)
			fprintf(stdout,"%s",it->cd);
}

void emit_program(tree p){
	if(p == NULL) fprintf(stderr,"[ERR] %s(): an error occured.\n",__FUNCTION__);
	else if (p->n.op != CONS){
		emit_code_external_declaration(p);
	}else{
		emit_program(p->tp.a[0]);
		emit_code_external_declaration(p->tp.a[1]);
	}
}

void emit_code_external_declaration(tree p){
	if(p == NULL) fprintf(stderr,"[ERR] %s(): an error occured.\n",__FUNCTION__);
	else if(p->n.op == TKN_INT)
		emit_code_declaration(p);
	else if(p->n.op == FUNDEF)
		emit_code_function_definition(p);
	else fprintf(stderr,"[ERR] %s(): an error occured.\n",__FUNCTION__);
}

void emit_code_declaration(tree p){
	if(p == NULL) fprintf(stderr,"[ERR] %s(): an error occured.\n",__FUNCTION__);
	else if(p->n.op == TKN_INT){
		emit_code_declarator_list(p->tp.a[0]);
	}
	else fprintf(stderr,"[ERR] %s(): an error occured.\n",__FUNCTION__);
}

void emit_code_declarator_list(tree p){
	if(p == NULL) fprintf(stderr,"[ERR] %s(): an error occured.\n",__FUNCTION__);
	else if(p->n.op == TKN_IDENTIFIER){
		/* 大域変数の宣言 */
		emit_code_declarator(p);
	} else if(p->n.op == CONS){
		/* 要素が1つに絞られるまで再帰 */
		emit_code_declarator_list(p->tp.a[0]);
		emit_code_declarator_list(p->tp.a[1]);
	}else fprintf(stderr,"[ERR] %s(): an error occured.\n",__FUNCTION__);
}
void emit_code_declarator(tree p){
	char *dec_name;
	if(p == NULL) fprintf(stderr,"[ERR] %s(): an error occured.\n",__FUNCTION__);
	else if(p->n.op == TKN_IDENTIFIER){
		dec_name = get_declarator(p);
		emit_common(dec_name);
	}
	else fprintf(stderr,"[ERR] %s(): an error occured.\n",__FUNCTION__);
}
char* get_declarator(tree p){
	if(p == NULL) fprintf(stderr,"[ERR] %s(): an error occured.\n",__FUNCTION__);
	else if(p->n.op == TKN_IDENTIFIER){
		return get_identifier(p);
	}
	else fprintf(stderr,"[ERR] %s(): an error occured.\n",__FUNCTION__);
	return NULL;
}

void emit_code_function_definition(tree p){
	static char buf[80];
	struct code *c;
	g_fun_name = get_declarator(p->tp.a[0]);
	g_ret_label = make_label(g_fun_name,"ret");

	/*
	   		GLOBAL	f
		f	push	ebp
			mov	ebp, esp
			...
	*/
	emit_global(g_fun_name);
	emit_label(g_fun_name);
	emit("push","ebp",NULL);
	emit("mov","ebp","esp");

	/* sub esp, Nlocalの代りの仮コード */
	c = emit(NULL,NULL,NULL);

	emit_code_parameter_type_list(p->tp.a[1]);
	emit_code_compound_statement(p->tp.a[2]);

	if(g_top_alloc){
		sprintf(buf,"\tsub\tesp, %d\n",-g_top_alloc);
		c->cd = strdup(buf);
	}
	emit_label(g_ret_label);
	emit("mov","esp","ebp");
	emit("pop","ebp",NULL);
	emit("ret",NULL,NULL);
}

void emit_code_parameter_type_list(tree p){
	if(p == NULL) fprintf(stderr,"[ERR] %s(): an error occured.\n",__FUNCTION__);
	else if(p->n.op == TKN_INT)
		emit_code_parameter_declaration(p);
	else if(p->n.op == CONS){
		emit_code_parameter_type_list(p->tp.a[0]);
		fprintf(stdout," ");
		emit_code_parameter_type_list(p->tp.a[1]);
	}else fprintf(stderr,"[ERR] %s(): an error occured.\n",__FUNCTION__);
}

void emit_code_parameter_declaration(tree p){
	if(p == NULL) fprintf(stderr,"[ERR] %s(): an error occured.\n",__FUNCTION__);
	else if(p->n.op == TKN_INT){
		/*** nop ***/
	}else fprintf(stderr,"[ERR] %s(): an error occured.\n",__FUNCTION__);
}

void emit_code_statement(tree p){
	char *label_begin = NULL;
	char *label_end = NULL;
	if(p == NULL) fprintf(stderr,"[ERR] %s(): an error occured.\n",__FUNCTION__);
	else if(p->n.op == CMPDSTM)
		emit_code_compound_statement(p);
	else if(p->n.op == TKN_IF){
		/* 
		fprintf(stdout,"(IF \n");
		emit_code_expression(p->tp.a[0]);
		emit_code_statement(p->tp.a[1]);
		if(p->tp.a[2] != NULL){
			emit_code_statement(p->tp.a[2]);
		}
		fprintf(stdout,")\n");
		 */
		label_begin = make_label(g_fun_name,"if_s2");
		label_end = make_label(g_fun_name,"if_end");
		emit_code_expression(p->tp.a[0]);
		emit("cmp","eax","0");
		emit("je",label_begin,NULL);
		emit_code_statement(p->tp.a[1]);
		emit("jmp",label_end,NULL);
		emit_label(label_begin);
		if(p->tp.a[2] != NULL)
			emit_code_statement(p->tp.a[2]);
		emit_label(label_end);
	}else if(p->n.op == TKN_WHILE){
		label_begin = make_label(g_fun_name,"while_begin");
		label_end = make_label(g_fun_name,"while_end");

		emit_label(label_begin);
		emit_code_expression(p->tp.a[0]);
		emit("cmp","eax","0");
		emit("je",label_end,NULL);
		emit_code_statement(p->tp.a[1]);
		emit("jmp",label_begin,NULL);
		emit_label(label_end);
	}else if(p->n.op == TKN_RETURN){
		if(p->tp.a[0] != NULL){
			emit_code_expression(p->tp.a[0]);
		}
		emit("jmp",g_ret_label,NULL);
	}else emit_code_expression(p);
}

void emit_code_compound_statement(tree p){
	if(p == NULL) fprintf(stderr,"[ERR] %s(): an error occured.\n",__FUNCTION__);
	if(p->n.op == CMPDSTM){
		if(p->tp.a[0] != NULL)
			emit_code_declaration_list(p->tp.a[0]);
		if(p->tp.a[1] != NULL)
			emit_code_statement_list(p->tp.a[1]);
	}else fprintf(stderr,"[ERR] %s(): an error occured.\n",__FUNCTION__);
}
void emit_code_declaration_list(tree p){
	if(p == NULL) fprintf(stderr,"[ERR] %s(): an error occured.\n",__FUNCTION__);
	else if(p->n.op == TKN_INT){
		p->tp.a[0]->tk.offset = allocate_loc();
	}else if(p->n.op == CONS){
		emit_code_declaration_list(p->tp.a[0]);
		emit_code_declaration_list(p->tp.a[1]);
	}
}
void emit_code_statement_list(tree p){
	if(p == NULL) fprintf(stderr,"[ERR] %s(): an error occured.\n",__FUNCTION__);
	else if(p->n.op == CONS){
		emit_code_statement_list(p->tp.a[0]);
		emit_code_statement_list(p->tp.a[1]);
	}else emit_code_statement(p);
}
void emit_code_expression(tree p){
	if(p == NULL) fprintf(stderr,"[ERR] %s(): an error occured.\n",__FUNCTION__);
	else if(p->n.op == CONS){
		emit_code_expression(p->tp.a[0]);
		emit_code_expression(p->tp.a[1]);
	}else {
		emit_code_assignment_expr(p);
	}
}
void emit_code_assignment_expr(tree p){
	if(p == NULL) fprintf(stderr,"[ERR] %s(): an error occured.\n",__FUNCTION__);
	else if(p->n.op == '='){
		emit_code_assignment_expr(p->tp.a[1]);
		emit("mov", get_identifier_loc(p->tp.a[0]),"eax");
	}else emit_code_logical_OR_expr(p);
}
void emit_code_logical_OR_expr(tree p){
	int temp;
	char *lab;
	if(p == NULL) fprintf(stderr,"[ERR] %s(): an error occured.\n",__FUNCTION__);
	else if(p->n.op == TKN_OR){
		temp = allocate_loc();
		lab = make_label(g_fun_name,"or");
		emit("mov dword",get_loc_as_str(temp),"1");
		emit_code_logical_OR_expr(p->tp.a[0]);
		emit("cmp","eax","0");
		emit("jne",lab,NULL);
		emit_code_logical_AND_expr(p->tp.a[1]);
		emit("cmp","eax","0");
		emit("jne",lab,NULL);
		emit("mov dword",get_loc_as_str(temp),"0");
		emit_label(lab);
		emit("mov","eax",get_loc_as_str(temp));
		release_loc();
	}else emit_code_logical_AND_expr(p);
}
void emit_code_logical_AND_expr(tree p){
	int temp;
	char *lab;
	if(p == NULL) fprintf(stderr,"[ERR] %s(): an error occured.\n",__FUNCTION__);
	else if(p->n.op == TKN_AND){
		temp = allocate_loc();
		lab = make_label(g_fun_name,"and");
		emit("mov dword",get_loc_as_str(temp),"0");
		emit_code_logical_AND_expr(p->tp.a[0]);
		emit("cmp","eax","0");
		emit("je",lab,NULL);
		emit_code_equality_expr(p->tp.a[1]);
		emit("cmp","eax","0");
		emit("je",lab,NULL);
		emit("mov dword",get_loc_as_str(temp),"1");
		emit_label(lab);
		emit("mov","eax",get_loc_as_str(temp));
		release_loc();
	}else emit_code_equality_expr(p);
}
void emit_code_equality_expr(tree p){
	int temp;
	if(p == NULL) fprintf(stderr,"[ERR] %s(): an error occured.\n",__FUNCTION__);
	else if(p->n.op == TKN_EQ){
		temp = allocate_loc();
		emit_code_relational_expr(p->tp.a[1]);
		emit("mov",get_loc_as_str(temp),"eax");
		emit_code_equality_expr(p->tp.a[0]);
		emit("cmp","eax",get_loc_as_str(temp));
		emit("sete","al",NULL);
		emit("movzx","eax","al");
		release_loc();
	}else if(p->n.op == TKN_NEQ){
		temp = allocate_loc();
		emit_code_relational_expr(p->tp.a[1]);
		emit("mov",get_loc_as_str(temp),"eax");
		emit_code_equality_expr(p->tp.a[0]);
		emit("cmp","eax",get_loc_as_str(temp));
		emit("setne","al",NULL);
		emit("movzx","eax","al");
		release_loc();
	}else emit_code_relational_expr(p);
}

void emit_code_relational_expr(tree p){
	int temp;
	if(p == NULL) fprintf(stderr,"[ERR] %s(): an error occured.\n",__FUNCTION__);
	else if(p->n.op == '<'){
		temp = allocate_loc();
		emit_code_additive_expr(p->tp.a[1]);
		emit("mov",get_loc_as_str(temp),"eax");
		emit_code_relational_expr(p->tp.a[0]);
		emit("cmp","eax",get_loc_as_str(temp));
		emit("setl","al",NULL);
		emit("movzx","eax","al");
		release_loc();
	}else if(p->n.op == '>'){
		temp = allocate_loc();
		emit_code_additive_expr(p->tp.a[1]);
		emit("mov",get_loc_as_str(temp),"eax");
		emit_code_relational_expr(p->tp.a[0]);
		emit("cmp","eax",get_loc_as_str(temp));
		emit("setg","al",NULL);
		emit("movzx","eax","al");
		release_loc();
	}else if(p->n.op == TKN_LE){
		temp = allocate_loc();
		emit_code_additive_expr(p->tp.a[1]);
		emit("mov",get_loc_as_str(temp),"eax");
		emit_code_relational_expr(p->tp.a[0]);
		emit("cmp","eax",get_loc_as_str(temp));
		emit("setle","al",NULL);
		emit("movzx","eax","al");
		release_loc();
	}else if(p->n.op == TKN_GE){
		temp = allocate_loc();
		emit_code_additive_expr(p->tp.a[1]);
		emit("mov",get_loc_as_str(temp),"eax");
		emit_code_relational_expr(p->tp.a[0]);
		emit("cmp","eax",get_loc_as_str(temp));
		emit("setge","al",NULL);
		emit("movzx","eax","al");
		release_loc();
	}else emit_code_additive_expr(p);
}
void emit_code_additive_expr(tree p){
	int temp;
	if(p == NULL) fprintf(stderr,"[ERR] %s(): an error occured.\n",__FUNCTION__);
	else if(p->n.op == '+'){
		temp = allocate_loc();
		emit_code_multiplicative_expr(p->tp.a[1]);
		emit("mov",get_loc_as_str(temp),"eax");
		emit_code_additive_expr(p->tp.a[0]);
		emit("add","eax",get_loc_as_str(temp));
		release_loc();
	}else if(p->n.op == '-'){
		temp = allocate_loc();
		emit_code_multiplicative_expr(p->tp.a[1]);
		emit("mov",get_loc_as_str(temp),"eax");
		emit_code_additive_expr(p->tp.a[0]);
		emit("sub","eax",get_loc_as_str(temp));
		release_loc();
	}else emit_code_multiplicative_expr(p);
}

void emit_code_multiplicative_expr(tree p){
	int temp;
	if(p == NULL) fprintf(stderr,"[ERR] %s(): an error occured.\n",__FUNCTION__);
	else if(p->n.op == '*'){
		temp = allocate_loc();
		emit_code_multiplicative_expr(p->tp.a[1]);
		emit("mov",get_loc_as_str(temp),"eax");
		emit_code_additive_expr(p->tp.a[0]);
		emit("imul","eax",get_loc_as_str(temp));
		release_loc();

	}else if(p->n.op == '/'){
		temp = allocate_loc();
		emit_code_multiplicative_expr(p->tp.a[1]);
		emit("mov",get_loc_as_str(temp),"eax");
		emit_code_additive_expr(p->tp.a[0]);
		emit("cdq",NULL,NULL);
		emit("idiv",get_loc_as_str(temp),NULL);
		release_loc();
	}else emit_code_unary_expr(p);
}
void emit_code_unary_expr(tree p){
	if(p == NULL) fprintf(stderr,"[ERR] %s(): an error occured.\n",__FUNCTION__);
	else if(p->n.op == '-'){
		emit_code_unary_expr(p->tp.a[0]);
		emit("nex","eax",NULL);
	}else emit_code_postfix_expr(p);
}


void emit_code_postfix_expr(tree p){
	if(p == NULL) fprintf(stderr,"[ERR] %s(): an error occured.\n",__FUNCTION__);
	else if(p->n.op == FCALL){
		g_fcall_args = 0;
		if(p->tp.a[1] != NULL)
			emit_code_argument_expression_list(p->tp.a[1]);
		if(p->tp.a[0]->tk.kind == UNDEFFUN)
			emit_extern(p->tp.a[0]->tk.name);
		emit("call",p->tp.a[0]->tk.name,NULL);
		emit("add","esp",itos(4 * g_fcall_args));
	}else emit_code_primary_expr(p);
}
void emit_code_primary_expr(tree p){
	if(p == NULL) fprintf(stderr,"[ERR] %s(): an error occured.\n",__FUNCTION__);
	else if(p->n.op == TKN_IDENTIFIER){
		emit_code_identifier(p);
	} else if(p->n.op == CONST){
		emit_code_constant(p);
	} else{
		emit_code_expression(p);
	}
}

void emit_code_argument_expression_list(tree p){
	if (p == NULL) fprintf(stderr,"[ERR] %s(): an error occured.\n",__FUNCTION__);
	else if(p->n.op == CONS){
		emit_code_argument_expression_list(p->tp.a[1]);
		emit_code_argument_expression_list(p->tp.a[0]);
	}else{
		g_fcall_args++;
		emit_code_assignment_expr(p);
		emit("push","eax",NULL);
	}
}

void emit_code_identifier(tree p){
	if(p == NULL) fprintf(stderr,"[ERR] %s(): an error occured.\n",__FUNCTION__);
	else if(p->n.op == TKN_IDENTIFIER)
		emit("mov","eax",get_identifier_loc(p));
	else fprintf(stderr,"[ERR] %s(): an error occured.\n",__FUNCTION__);
}
void emit_code_constant(tree p){
	if(p==NULL) fprintf(stderr,"[ERR] %s(): an error occured.\n",__FUNCTION__);
	else if(p->n.op == CONST)
		emit("mov","eax",get_constant(p));
	else fprintf(stderr,"[ERR] %s(): an error occured.\n",__FUNCTION__);
}
char *get_identifier_loc(tree p){
	char buf[80];
	if(p->tk.lev == 0)
		sprintf(buf,"[%s]",p->tk.name);
	else{
		sprintf(buf," [ebp%s%d] : %s",
				((p->tk.offset >=0) ? "+" : ""),
				p->tk.offset,p->tk.name);
		emit_comment(buf);
		sprintf(buf,"dword [ebp%s%d]",
				((p->tk.offset >=0) ? "+" : ""),
				p->tk.offset);
	}
	return strdup(buf);
}

char *get_identifier(tree p){
	return (p->tk.name);
}
char *get_constant(tree p){
	char *buf = (char *)malloc(sizeof(char)*32);
	sprintf(buf,"%d",p->c.v);
	return buf;
}

