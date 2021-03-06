%{
#include <stdlib.h>
#include <string.h>
void yyerror(char *);

#include "y.tab.h"
%}

%%

[a-z] {
        yylval.ival = yytext[0] - 'a';
        return VAR;
      }

[0-9]+[.]*[0-9]* {
        yylval.fval = atof(yytext);
        return FLOAT;
      }

\" {
    return ASPA;
   }

(LIN|lin) {
    return LINHA;
    }
    
(COL|col) {
    return COLUNA;
    }
   
(THR|thr) {
    return THREAD;
    }

(PRC|prc) {
    return PROCESSO;
    }

[a-zA-Z0-9\.]+ {
                for (int i = 0; i < yyleng; i++ )
                  yylval.strval[i] = yytext[i];

                yylval.strval[yyleng] = '\0';
                //printf ("Achei nome: %s\n", yylval.strval);
                return STRING;
               }

[=] {
      return IGUAL;
    }

[*] {
      return VEZES;
    }

[/] {
      return DIVIDIDO;
    }

\[  {
      return ABRE;
    }

\]  {
      return FECHA;
    }

\n {
      return EOL;
   }

[ \t]+ ;

. {
    printf("**%s**\n", yytext);
    yyerror("Token nao reconhecido!\n");
  }

%%

int  yywrap() {
  return 1;
}