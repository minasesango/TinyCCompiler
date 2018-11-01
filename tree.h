#ifndef TREE_H_
#define TREE_H_
typedef struct c{
	int op;
	int v;
} *constant;
typedef struct tk{
	int op;
	struct tk *next;           
	char *name;
	int lev;
	enum {FRESH,VAR,FUN,PARM,UNDEFFUN} kind;
	int offset;
} *token;
typedef struct tp{
	int op;
	union nd *a[4];
} *tuple;
typedef union nd{
	struct{
		int op;
	}n;
	struct tp tp;
	struct tk tk;
	struct c  c;
} *tree;

tree make_tuple(int,tree,tree,tree,tree);
tree make_token_node(char *);
tree make_constant_node(int);
tree make_decl(tree);
tree make_parm_decl(tree);
tree make_fun_def(tree);
tree ref_var(tree);
tree ref_fun(tree);
token push_token(token);
token pop_token(token);
token pop_token_lev_gt(int);
tree lookup_sym(char *);
void globalize_sym(tree);

void print_program(tree);
#endif

