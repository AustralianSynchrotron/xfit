
%token EDGE EXAFSW FTW DATA KWEIGHT RESTORE REFINE LIST
%token DELETE GROUP EXIT C_MODEL SAVE CHECK_DERIVS PRINT MONTECARLO
%token CLEAN SYSTEM INVALID INCLUDE LOG_FILE HELP

%token <nval> IDENTIFIER VECTOR_ID
%token <nval> SC_F0 SC_F1 SC_F2 POW_OP

%token <dval> CONSTANT
%token <pval> STRING_LIT HELP_TOPIC
%token <lval> WINDOW_EDGE EXAFS_EDGE LIST_PARAMETER

%token EOL LE_OP GE_OP EQ_OP AEQ_OP CROSS

%token <pval> MODEL_THEORY

%{

#define __Y_TAB_C__

extern int lex_state;

#define lex(state) lex_state = state;

#include <malloc.h>

#include "console.h"

#if !defined(WIN32)
#include <unistd.h>
#endif

#include "matrix.h"
#include "models.h"

#include "expressi.h"

%}

%union {

  char    cval;
  short   sval;
  int     ival;
  long    lval;
  char *  pval;
  double  dval;
  vector4 vval;
  node *  nval;

  }

%{

#include "c_constr.h"

#include "save_exa.h"

#include "actions.h"
#include "minimise.h"
#include "commands.h"

typedef struct {

  parameter_type data;

  double factor;

  } linear_el_type;

void prompt (const char *);

#define PROMPT prompt (NULL);

%}

%type <nval> id v_id s_f0 s_f1 s_f2 primary_s_expr unary_s_expr
%type <nval> multiplicative_s_expr s_expr
%type <nval> primary_v_expr postfix_v_expr comma_expr unary_v_expr
%type <nval> multiplicative_v_expr d_v_expr v_expr
%type <nval> constraint_expr relational_expr id_list
%type <nval> sigma

%type <vval> list_range
%type <pval> ex_list exc_list

%left '+' '-'
%left '*' '/'
%left CROSS '.'
%left '^' POW_OP

%start translation_unit

%%

constraint_expr
        : s_expr EQ_OP s_expr         { $$ = onod2 (EQUTO, $1, $3); }
        ;

relational_expr
        : s_expr AEQ_OP s_expr sigma  { $$ = onod3 (AEQTO, $1, $3, $4); }
        | s_expr '<'    s_expr sigma  { $$ = onod3 (LTHAN, $1, $3, $4); }
        | s_expr '>'    s_expr sigma  { $$ = onod3 (GTHAN, $1, $3, $4); }
        | s_expr LE_OP  s_expr sigma  { $$ = onod3 (LTHAN, $1, $3, $4); }
        | s_expr GE_OP  s_expr sigma  { $$ = onod3 (GTHAN, $1, $3, $4); }
        ;

assignment_expr
        : id   { lex ($1->parameter.lex_state); } '=' s_expr    {
                                          assign ($1, v_value ($4));
                                          free_tree ($1); lex (0); }
        | v_id { lex ($1->parameter.lex_state); } '=' v_expr    {
                                          assign ($1, v_value ($4));
                                          free_tree ($1); lex (0); }
        |      '?' s_expr               { cout ("\n   ");
                                          fprintf_expression (stdout, $2, 0);
                                          cout (" = %.8g\n\n",
                                                               s_value ($2)); }
        |      '?' relational_expr      { cout ("\n   ");
                                          fprintf_expression (stdout, $2, 0);
                                          cout (" = %.8g\n\n",
                                                               s_value ($2)); }
        |      '?' v_expr               { vector4 v;
                                          cout ("\n   ");
                                          fprintf_expression (stdout, $2, 0);
                                          v = v_value ($2);
                                          cout (" = (%.8g, %.8g, %.8g)\n\n",
                                              v_el (v, 0), v_el (v, 1),
                                              v_el (v, 2)); }
        |      '?' STRING_LIT           { cout ($2);
                                          cout ("\n"); }
        ;

id      : IDENTIFIER
        ;

v_id    : VECTOR_ID
        ;

s_f0    : SC_F0                         { $$ = $1; }
        ;

s_f1    : SC_F1 '(' s_expr ')'          { $$ = node_op1 ($1, $3); }
        ;

s_f2    : SC_F2 '(' s_expr ',' s_expr ')' { $$ = node_op2 ($1, $3, $5); }
        ;

primary_s_expr
        : '(' s_expr ')'                { $$ = $2; }
        | '|' s_expr '|'                { $$ = onod1 (ABS, $2); }
        | '|' v_expr '|'                { $$ = onod1 (VABS, $2); }
        | CONSTANT                      { $$ = cnode (CONST, $1); }
        | id
        | s_f0
        | s_f1
        | s_f2
        | d_v_expr '.' d_v_expr         { $$ = onod2 (VDOT, $1, $3); }
        | primary_s_expr POW_OP primary_s_expr { $$ = node_op2 ($2, $1, $3); }
        ;

unary_s_expr
        : primary_s_expr
        | '+' primary_s_expr            { $$ = $2; }
        | '-' primary_s_expr            { $$ = onod1 (NEG, $2); }
        ;

multiplicative_s_expr
        : unary_s_expr
        | multiplicative_s_expr '*' unary_s_expr { $$ = onod2 (MUL, $1, $3); }
        | multiplicative_s_expr '/' unary_s_expr { $$ = onod2 (DIV, $1, $3); } 
        ;

s_expr  : multiplicative_s_expr
        | s_expr '+' multiplicative_s_expr { $$ = onod2 (ADD, $1, $3); }
        | s_expr '-' multiplicative_s_expr { $$ = onod2 (SUB, $1, $3); }
        ;

primary_v_expr
        : v_id
        | '(' v_expr ')'                { $$ = $2; }
        | '[' comma_expr ']'            { $$ = $2; }
        ;

comma_expr
        : v_expr
        | s_expr                        { $$ = onod1 (VCAST, $1); }
        | s_expr ',' s_expr             { $$ = onod2 (VCAST, $1, $3); }
        | s_expr ',' s_expr ',' s_expr  { $$ = onod3 (VCAST, $1, $3, $5); }
        ;

postfix_v_expr
        : primary_v_expr
        | primary_v_expr '^'            { $$ = onod1 (CARET, $1); }
        ;

unary_v_expr
        : '+' postfix_v_expr            { $$ = $2; }
        | '-' postfix_v_expr            { $$ = onod1 (VNEG, $2); }
        ;

d_v_expr
        : unary_v_expr
        | postfix_v_expr
        ;

multiplicative_v_expr
        : d_v_expr
        | multiplicative_v_expr CROSS d_v_expr   { $$ = onod2 (VCRO, $1, $3); }
        | multiplicative_v_expr '*' unary_s_expr { $$ = onod2 (VMUL, $1, $3); }
        | multiplicative_v_expr '/' unary_s_expr { $$ = onod2 (VDIV, $1, $3); }
        | multiplicative_s_expr '*' d_v_expr     { $$ = onod2 (VMUL, $3, $1); }
        ;

v_expr  : multiplicative_v_expr
        | v_expr '+' multiplicative_v_expr { $$ = onod2 (VADD, $1, $3); }
        | v_expr '-' multiplicative_v_expr { $$ = onod2 (VSUB, $1, $3); }
        ;

sigma   :                               { $$ = cnode (CONST, 1.0); }
        | '{' s_expr '}'                { $$ = $2;                 }
        ;

translation_unit
        : statement_list                { clean_act; }
        ;

statement_list
        :                               { PROMPT }
        | statement_list statement      { lex (0); }
        ;

statement
        : EOL                           { PROMPT }
        | constraint_expr      EOL      { PROMPT add_cons ($1); }
        | relational_expr      EOL      { PROMPT add_rest ($1); }
        | assignment_expr      EOL      { PROMPT }
        | data_statement       EOL      { PROMPT }
        | RESTORE              EOL      { PROMPT restore_act; }
        | CLEAN                EOL      { PROMPT clean_act; }
        | refine_statement     EOL      { PROMPT }
        | montecarlo_statement EOL      { PROMPT }
        | list_statement       EOL      { PROMPT }
        | save_statement       EOL      { PROMPT }
        | delete_statement     EOL      { PROMPT }
        | model_statement      EOL      { PROMPT }
        | check_d_statement    EOL      { PROMPT }
        | print_statement      EOL      { PROMPT }
        | system_statement     EOL      { PROMPT }
        | help_statement       EOL      { PROMPT }
        | log_statement        EOL      { PROMPT }
        | error EOL                     { yyerrok; }
        | INCLUDE
        ;


help_statement
        : HELP                          { help_command (""); }
        | HELP HELP_TOPIC               { help_command ($2); free ($2); }
        ;

log_statement
        :
        | LOG_FILE STRING_LIT           { log_command ($2); }
        ;

data_statement
        : DATA                          { exafs_act (""); }
        | DATA STRING_LIT               { exafs_act ($2); free ($2); }
        ;

system_statement
        : SYSTEM STRING_LIT                     { system_command ($2); free ($2); }
        ;

print_statement
        : PRINT                                 { print_act ("", 0, 1); }
        | PRINT CONSTANT                        { print_act ("", $2, 1); }
        | PRINT CONSTANT CONSTANT               { print_act ("", $2, $3); }
        | PRINT STRING_LIT                      { print_act ($2, 0, 1); }
        | PRINT STRING_LIT CONSTANT             { print_act ($2, $3, 1); }
        | PRINT STRING_LIT CONSTANT CONSTANT    { print_act ($2, $3, $4); }
        ;

check_d_statement
        : CHECK_DERIVS '=' CONSTANT     { check_derivatives = max (0, $3);
                                          if (check_derivatives)
                                            cout ("\n derivative check is ON"
                                                    " at level %d\n\n",
                                                    check_derivatives);
                                          else
                                            cout ("\n derivative check is"
                                                    " OFF\n\n"); }
        | '?' CHECK_DERIVS              { if (check_derivatives)
                                            cout ("\n derivative check is ON"
                                                    " at level %d\n\n",
                                                    check_derivatives);
                                          else
                                            cout ("\n derivative check is"
                                                    " OFF\n\n"); }
        ;

refine_statement
        : REFINE id_list                { refine_act (20, $2); free_tree ($2); }
        | REFINE CONSTANT id_list       { refine_act ($2, $3); free_tree ($3); }
        | REFINE '-' CONSTANT id_list   { refine_act (-($3), $4);
                                                               free_tree ($4); }
        ;

montecarlo_statement
        : MONTECARLO id_list ex_list            { montecarlo_act (16, 20, $2, (node **) $3);
                                                  free_tree ($2); free_check ($3); }
        | MONTECARLO CONSTANT id_list ex_list   { montecarlo_act ($2, 20, $3, (node **) $4);
                                                  free_tree ($3); free_check ($4); }
        | MONTECARLO CONSTANT CONSTANT id_list ex_list  { montecarlo_act ($2, $3, $4,(node **) $5);
                                                          free_tree ($4); free_check ($5); }
        ;

ex_list :                               { $$ = NULL; }
        | '{' exc_list '}'              { $$ = $2; }
        ;

exc_list
        : s_expr                        { $$ = (char *) node_list (NULL, (node *) $1); }
        | exc_list ',' s_expr           { $$ = (char *) node_list ((node **) $1, (node *) $3); }
        ;

model_statement
        : C_MODEL MODEL_THEORY          { change_model (&global_data, $2);
                                          cout ("\n new theory : %s\n\n",
                                          global_data.model->description); }
        | C_MODEL '?'                   { cout ("\n current theory : %s\n\n",
                                          global_data.model->description); }
        | '?' C_MODEL                   { cout ("\n current theory : %s\n\n",
                                          global_data.model->description); }
        ;

delete_statement
        : DELETE delete_list
        ;

delete_list
        :
        | delete_list delete_list_entry
        ;

delete_list_entry
        : LIST_PARAMETER delete_range_list  {/* empty */ }
        ;

delete_range_list
        : list_range                     { delete_act ($<lval>0, v_el ($1, 0),
                                                               v_el ($1, 1)); }
        | delete_range_list list_range   { delete_act ($<lval>0, v_el ($2, 0),
                                                               v_el ($2, 1)); }
        ;

save_statement
        : SAVE { save_act ("", 0, 0, 0); } save_list
        | SAVE STRING_LIT { save_act ($2, 0, 0, 0); } save_list
        ;

save_list
        :
        | save_list save_list_entry
        ;

save_list_entry
        : LIST_PARAMETER                 { save_act (NULL, $1, 0, 999); }
        | LIST_PARAMETER save_range_list { /* empty action */ }
        ;

save_range_list
        : list_range                    { save_act (NULL, $<lval>0,
                                                 v_el ($1, 0), v_el ($1, 1)); }
        | save_range_list list_range    { save_act (NULL, $<lval>0,
                                                 v_el ($2, 0), v_el ($2, 1)); }
        ;

list_statement
        : LIST list_list
        ;

list_list
        :
        | list_list list_list_entry
        ;

list_list_entry
        : LIST_PARAMETER                 { list_act ($1, 0, 999); }
        | LIST_PARAMETER list_range_list { /* empty action */ }
        ;

list_range_list
        : list_range                    { list_act ($<lval>0, v_el ($1, 0),
                                                              v_el ($1, 1)); }
        | list_range_list list_range    { list_act ($<lval>0, v_el ($2, 0),
                                                              v_el ($2, 1)); }
        ;

list_range
        : CONSTANT                      { $$ = make_vector2 ($1,  $1); }
        | CONSTANT '-'                  { $$ = make_vector2 ($1, 999); }
        | CONSTANT '-' CONSTANT         { $$ = make_vector2 ($1,  $3); }
        | '-' CONSTANT                  { $$ = make_vector2 ( 0,  $2); }
        ;

id_list :                               { $$ = NULL; }
        | id_list id                    { $$ = add_node1 (&($1), $2); }
        | id_list v_id                  { $$ = add_node1 (&($1), $2); }
        | id_list ',' id                { $$ = add_node1 (&($1), $3); }
        | id_list ',' v_id              { $$ = add_node1 (&($1), $3); }
        ;

%%

#include <stdio.h>

