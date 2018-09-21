%{
#include <iostream>
#include <string>
#include "cimple.tab.h"
using namespace std;
extern int yylex();
int linenumber = 1;
%}
%option noyywrap
%%
\/\/.*                          { /*this is a comment*/}
\n                              { linenumber++; }
[ \t]                           ;
;                               { return SEMICOLON; }
while                           { return WHILE; }
\(                              { return OPAREN; }
\)                              { return CPAREN; }
\{                              { return OBRACE; }
\}                              { return CBRACE; } 
,                               { return COMMA; }
=                               { return ASSIGNMENT; }
\*                              { return MULT; }
\+                              { return ADD; }
\|\|                            { return OR; }
&&                              { return AND; }
\<                              { return LT; }
>                               { return GT; }
\<=                             { return LTE; }
>=                              { return GTE; }
==                              { return EQUAL; }
!=                              { return NEQUAL; }
-                               { return SUB; }
!                               { return NOT; }
if                              { return IF; }
else                            { return ELSE; }
int                             { return INTTYPE; }
char                            { return CHARTYPE; }
string                          { return STRINGTYPE; }
return                          { return RETURN; }
global                          { return GLOBAL; }
[0-9]+                          { yylval.ival = atoi(yytext); return INT; }
\"[a-zA-Z0-9 ]*\"               {   
                                    yylval.sval = (char*) malloc(yyleng - 1);
                                    memcpy(yylval.sval, yytext + 1, yyleng - 2);
                                    yylval.sval[yyleng - 2] = 0;
                                    return STRING; }
\'[a-zA-Z0-9]\'                 { yylval.cval = yytext[1]; return CHAR; }
[a-zA-Z][a-zA-Z0-9_]*           { yylval.idval = strdup(yytext); return ID; }
%%
