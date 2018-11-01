#ifndef EMIT_H_
#define EMIT_H_
#include "tree.h"
void emit_program(tree);
void print_code(void);


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
void emit_code_logical_expr(tree);
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

#endif
