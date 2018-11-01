#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>
#include "tree.h"
#include "util.h"
#include "tcc.tab.h"

extern token symtab;
extern int cur_lev;
int g_current_var_offset = -4;
int g_current_parm_offset = 8;
int g_current_fun_offset = 0;

tree make_tuple(int op, tree a0, tree a1, tree a2, tree a3){
	tree t = (tree)malloc(sizeof(*t));
	t->tp.op = op;
	t->tp.a[0] = a0;
	t->tp.a[1] = a1;
	t->tp.a[2] = a2;
	t->tp.a[3] = a3;
	return t;
}

tree make_token_node(char *s){
	tree t = (tree)malloc(sizeof(*t));
	t->tk.op = TKN_IDENTIFIER;
	t->tk.name = s;
	t->tk.lev = cur_lev;
	push_token((token)t);
	return t;
}

tree make_constant_node(int n){
	tree t = (tree)malloc(sizeof(*t));
	t->c.op = CONST;
	t->c.v = n;
	return t;
}
tree make_decl(tree n)
{
	switch(n->tk.kind){
		case VAR:
			if (n->tk.lev == cur_lev)
				error("redeclaration of `%s'",n->tk.name);
			n = make_token_node(n->tk.name);
			break;
		case FUN:
		case UNDEFFUN:
			if (n->tk.lev == cur_lev)
				error("`%s' redeclared as different kind of symbol",
						n->tk.name);
			n = make_token_node(n->tk.name);
			break;
		case PARM:
			warn("declaration of `%s' shadows a parameter",n->tk.name);
			n = make_token_node(n->tk.name);
			break;
		case FRESH:
			break;
	}
	n->tk.kind = VAR;
	if(n->tk.lev > 0){
		n->tk.offset = g_current_var_offset;
		g_current_var_offset += -4;
	}
	return n;
}
tree make_parm_decl(tree n){
	switch(n->tk.kind){
		case VAR:
		case FUN:
		case UNDEFFUN:
			n = make_token_node(n->tk.name);
			break;
		case PARM:
			error("redeclaration of `%s'",n->tk.name);
			return n;
		case FRESH:
			break;
	}
	n->tk.kind = PARM;
	n->tk.offset = g_current_parm_offset;
	g_current_parm_offset += 4;
	return n;
}

tree make_fun_def(tree n){
	extern int g_current_fun_parms;
	switch(n->tk.kind){
		case VAR:
			error("`%s' redeclared as different kind of symbol", n->tk.name);
			break;
		case FUN:
			error("redeclaration of `%s'",n->tk.name);
			break;
		case UNDEFFUN:
		case FRESH:
			n->tk.kind = FUN;
			break;
		default:
			error("bad token `%s'",n->tk.name);
			break;
	}
	n->tk.offset = g_current_fun_parms;
	return n;
}
tree ref_var(tree n){
	switch(n->tk.kind){
		case VAR:
		case PARM:
			break;
		case FUN:
		case UNDEFFUN:
			error("function `%s' is used as variable",n->tk.name);
			break;
		case FRESH:
			error("`%s' undeclared variable",n->tk.name);
			n->tk.kind = VAR;
			break;
	}
	return n;
}

tree ref_fun(tree n){
	extern int g_current_ref_fun_parms;
	switch(n->tk.kind){
		case VAR:
		case PARM:
			error("variable `%s' is used as function",n->tk.name);
			break;
		case FUN:
		case UNDEFFUN:
			break;
		case FRESH:
			warn("`%s' undeclared function",n->tk.name);
			n->tk.kind = UNDEFFUN;
			if (n->tk.lev > 0){
			       	globalize_sym(n);
			}
				n->tk.offset = g_current_ref_fun_parms;
			break;
	}
	if(g_current_ref_fun_parms > n->tk.offset)
		error("too many arguments to function `%s'",n->tk.name);
	else if(g_current_ref_fun_parms < n->tk.offset)
		error("too few arguments to function `%s'",n->tk.name);
	return n;
}

token push_token(token tkn){
	tkn->next = symtab;
	symtab = tkn;	
	return symtab;
}

token pop_token(token tkn){
	switch(tkn->kind){
		case VAR:
			g_current_var_offset -= -4;
			break;
		case PARM:
			g_current_parm_offset -= 4;
			break;
		default:
			break;
	}
	symtab = tkn->next;
	return symtab;
}
token pop_token_lev_gt(int lev){
	token tkn;
	token next;
	tkn = symtab;
	while (tkn != NULL){
		next = tkn->next;
		if(lev < tkn->lev) pop_token(tkn);
		tkn = next;
	}
	return symtab;

}

tree lookup_sym(char *s){
	token tkn;
	for (tkn = symtab; tkn != NULL; tkn = tkn->next)
		if(strcmp(tkn->name,s)==0) break;
	return (tree)tkn;
}

void globalize_sym(tree n){
	token tmp;
	token tk = &(n->tk), prev = NULL, next = tk->next;
	tk->lev = 0;

	if (symtab != tk){
		for (prev = symtab; prev!=NULL && prev->next != tk ; prev = prev->next);
		prev->next = next;
	}else
		symtab = next;
	for (tmp = symtab; tmp->next != NULL; tmp = tmp->next); tmp->next = tk;
	tk->next = NULL;
}


