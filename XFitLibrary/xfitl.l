
%p 2500
%a 2000
%array
%option nounput

D                       [0-9]
L                       [a-zA-Z_]
OB                      [[({]
CB                      [\])}]
ID                      [a-zA-Z_][a-zA-Z_0-9]*
NUM                     [0-9]+
INT                     "-"?[0-9]+
E                       [Ee][+-]?{D}+
WS                      [ \f\r\t\v]
WSN                     [ \f\r\t\v\n]
NWS                     [^ \f\r\t\v]
NWSN                    [^ \f\r\t\v\n]

%s INCL
%s INCLx
%s HLP

%{

int lex_state = 0;

#define lex(state) lex_state = state;

#include "input.h"

#include "list_com.h"

unsigned char yyfilename [256] = "stdin";

void skip_comment ();
int constant ();
int check_type ();

void prompt (const char * c) {

  if (command_in->file != stdin)

    return;

  if (c)

    cout (c);

  else

    cout (" > ");

  fflush (stdout);

  }

int yywrap () {

  if (end_include_file (&command_in))

    return 1;

  prompt (NULL);

  return 0;

  }

%}

%%

<INITIAL>"#"            { skip_comment (); }

^"<"                    BEGIN (INCL);

<INCL>{WS}+             { }

<INCL>\"[^\"]*\"        { yytext [yyleng - 1] = 0;
                          include_command (yytext + 1, &command_in);
                          BEGIN (INCLx); return (INCLUDE); }

<INCL>({NWSN})+         { include_command (yytext, &command_in);
                          BEGIN (INCLx); return (INCLUDE); }

<INCL>\n                { include_command ("", &command_in);
                          BEGIN (INITIAL); return (EOL); }

<INCLx>[^\n]*\n         { BEGIN (INITIAL); return (EOL); }

^"help"{WS}*            { BEGIN (HLP); return (HELP); }

<HLP>[^\n]*             { yylval.pval = strdup (yytext);  
                          return (HELP_TOPIC); }

<HLP>\n                 { BEGIN (INITIAL); return (EOL); }

"end"                   { return !yywrap (); }

^" ".*\n                { return (EOL); }

"pi"                    { yylval.nval = make_node (PI_C);  return (SC_F0); }

"random"                { yylval.nval = make_node (RAND0); return (SC_F0); }
"sin"                   { yylval.nval = make_node (SIN);   return (SC_F1); }
"cos"                   { yylval.nval = make_node (COS);   return (SC_F1); }
"tan"                   { yylval.nval = make_node (TAN);   return (SC_F1); }
"asin"                  { yylval.nval = make_node (ASIN);  return (SC_F1); }
"acos"                  { yylval.nval = make_node (ACOS);  return (SC_F1); }
"atan"                  { yylval.nval = make_node (ATAN);  return (SC_F1); }
"sinh"                  { yylval.nval = make_node (SINH);  return (SC_F1); }
"cosh"                  { yylval.nval = make_node (COSH);  return (SC_F1); }
"tanh"                  { yylval.nval = make_node (TANH);  return (SC_F1); }
"asinh"                 { yylval.nval = make_node (ASINH); return (SC_F1); }
"acosh"                 { yylval.nval = make_node (ACOSH); return (SC_F1); }
"atanh"                 { yylval.nval = make_node (ATANH); return (SC_F1); }
"sqrt"                  { yylval.nval = make_node (SQRT);  return (SC_F1); }
"exp"                   { yylval.nval = make_node (EXP);   return (SC_F1); }
"ln"                    { yylval.nval = make_node (LN);    return (SC_F1); }
"exp10"                 { yylval.nval = make_node (EXP10); return (SC_F1); }
"log"                   { yylval.nval = make_node (LOG);   return (SC_F1); }
"pow"                   { yylval.nval = make_node (POW);   return (SC_F2); }

"**"                    { yylval.nval = make_node (POW);   return (POW_OP); }

"<="|"=<"               { return (LE_OP); }
">="|"=>"               { return (GE_OP); }
"~="|"=~"               { return (AEQ_OP); }
"=="                    { return (EQ_OP); }

"["                     { return ('['); }
"]"                     { return (']'); }
","                     { return (','); }
"{"                     { return ('{'); }
"}"                     { return ('}'); }
"("                     { return ('('); }
")"                     { return (')'); }
"|"                     { return ('|'); }
"="                     { return ('='); }
"<"                     { return ('<'); }
">"                     { return ('>'); }
"+"                     { return ('+'); }
"-"                     { return ('-'); }
"*"                     { return ('*'); }
"/"                     { return ('/'); }
"."                     { return ('.'); }
"^"                     { return ('^'); }
"?"                     { return ('?'); }

"log-file"              { return (LOG_FILE); }

"x"                     { return (CROSS); }

{D}+({E})?              |
{D}*"."{D}+({E})?       |
{D}+"."{D}*({E})?       { return (constant ()); }

{ID}                    |
{ID}{OB}{INT}?{CB}{NUM}? { int token;
                          token = check_type ();
                          if (token != INVALID) return (token);
                          REJECT; }

\"[^\"]*\"              { yytext [yyleng - 1] = 0;
                          yylval.pval = strdup (yytext + 1);  
                          return (STRING_LIT); }

\n                      { return (EOL); }

{WS}                    { }

.                       { return (yytext [0]); /* unknown character */ }

%%

int yycolumn = 0;

void skip_comment () {

  char c;

  while ((c = xf_input()) != '#' && c != 0)

    if (c == '\n')

      prompt (NULL);

  }

int constant () {

  if (sscanf (yytext, "%lf", &yylval.dval) > 0)

    return CONSTANT;

  return INVALID;

  }

int check_type () {

  /* Attempt to match the text yytext to a command or a parameter.

     The token also depends on lex_state which is set in xfit.y
     according to the command being parsed.

     Atom names, X-ray edge types, and window edge types are translated
     into integer constants */

  int i, n, m;

  parameter_type pdata;
  command_name_type cdata;

  double key;

  switch (lex_state) {

    case 4 :

      /* Model command : curently available models are in model_list */

      for (i = 0; model_list [i].name; i++)

        if (!uniquestrlencasecmp (yytext, model_list [i].name,
                                          model_list [i].unique)) {

          yylval.pval = (char *) (model_list + i);

          return MODEL_THEORY;

          }

      break;

    case 0 :
    case 2 :
    case 3 :

      if (command_name (yytext, global_data.command_name_list, &cdata)) {

        lex_state = cdata.lex_state;

        return cdata.type;

        }

      if ((n = parameter_index (yytext, global_data.parameter_name_list,
                                        &pdata)) > 0) {

        key = parameter_vector_key_Oi (global_data.parameters, MODEL, 0);

        }
      else {

        if (n = parameter_index (yytext, EXAFS_parameter_name, &pdata)) {

          exafs_list * e;

            /* Count the data sets */

          for (n = 0, e = *global_data.exafs; e; e = e->next, n++);

          if (n > 0) {

            m = pdata.offset;

            pdata.offset = ((pdata.offset % FSIZE) + FSIZE) % FSIZE;

            m = (m - pdata.offset) / FSIZE;

            pdata.shell_offset = (((m % n) + n) % n) + 1;

            key = parameter_vector_key_Oi (global_data.parameters,
                                     DATA_SET, pdata.shell_offset - 1);

            }
          }
        }

      if (n) {

        if (pdata.command == VECTOR_ID)

          yylval.nval = make_node (V_ID);

        else

          yylval.nval = make_node (ID);

        pdata.key = key;

        yylval.nval->parameter = pdata;

        return pdata.command;

        }

      for (i = 0; lex_state == 2 && filter_edge_name [i].unique; i++)

        if (!uniquestrlencasecmp (yytext, filter_edge_name [i].name,
                                          filter_edge_name [i].unique)) {

          yylval.dval = filter_edge_name [i].edge;

          return CONSTANT;

          }

      if (lex_state == 3)

        if (X_ray_edge_code (yytext)) {

           yylval.dval = X_ray_edge_code (yytext);

           return CONSTANT;

           }

      if (atomic_number (yytext)) {

        yylval.dval = atomic_number (yytext);

        return CONSTANT;

        }

      break;

    case 1 : /* list command parameter */

      for (i = 0; __list_command_name [i].unique; i++)

        if (!uniquestrlencasecmp (yytext, __list_command_name [i].name,
                                          __list_command_name [i].unique)) {

          yylval.lval = __list_command_name [i].type;

          return LIST_PARAMETER;

          }

      break;

    }

  return INVALID;

  }
