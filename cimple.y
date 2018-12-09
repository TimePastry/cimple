%{
#include <cstdio>
#include <iostream>
#include <string>
#include "ASTNodeDefines.h"
using namespace std;

extern int yylex();
extern int yyparse();
extern FILE * yyin;
extern int linenumber;

void yyerror(const char* s);
int num;

ASTNode* head;
%}

%code requires
{
    #include "ASTNodeDefines.h"
}

%union
{
    int ival;
    char cval;
    char* sval;
    char* idval;
    ASTNode* nval;
}

%token <ival> INT
%token <cval> CHAR
%token <sval> STRING
%token <idval> ID

%token SEMICOLON
%token OPAREN
%token CPAREN
%token OBRACE
%token CBRACE
%token COMMA
%token ASSIGNMENT
%token MULT
%token OR
%token ADD
%token AND
%token LT
%token GT
%token LTE
%token GTE
%token EQUAL
%token NEQUAL
%token SUB
%token NOT
%token WHILE
%token IF
%token ELSE
%token INTTYPE
%token CHARTYPE
%token STRINGTYPE
%token RETURN
%token GLOBAL

%type<ival> unoperator binoperator
%type<sval> type
%type<nval> globals functions declaration fundef args funbody argument arglist argprime vardeclarations statements localdeclaration statement
%type<nval> functioncall values valuelist valueprime value literal assignment term unop binop whileloop ifstatement return valuemember valueterm
%%

program:
       globals functions               { head = new ProgramNode($1, $2); }
    ;

globals:
       globals declaration
                                    {
                                        ASTNode* cur = $1;
                                        if (cur)
                                        {
                                            ((DeclarationNode*)cur)->append($2);
                                            $$ = cur;
                                        } else
                                        {
                                            $$ = $2;
                                        }
                                    }
    | /*epsilon*/                   { $$ = NULL; }
    ;

declaration:
           GLOBAL type ID SEMICOLON
                                    {
                                        string type = $2;
                                        string id = $3;
                                        $$ = new DeclarationNode(type, id);
                                        $$->setLineNum(linenumber);
                                    }
    ;

type:
    INTTYPE                         { $$ = (char *)"int"; }
    | CHARTYPE                      { $$ = (char *)"char"; }
    | STRINGTYPE                    { $$ = (char *)"string"; }
    ;

functions:
         functions fundef
                                    {
                                        ASTNode* cur = $1;
                                        if (cur)
                                        {
                                            ((FunctionNode*)cur)->append($2);
                                            $$ = cur;
                                        } else
                                        {
                                            $$ = $2;
                                        }
                                    }
    | /*epsilon*/                   { $$ = NULL; }
    ;

fundef:
      type { num = linenumber; } ID OPAREN args CPAREN OBRACE funbody CBRACE
                                    {
                                        string type = $1;
                                        string id = $3;
                                        $$ = new FunctionNode(type, id, $5, $8);
                                        $$->setLineNum(num);
                                    }
    ;

args:
    arglist                         { $$ = $1; /*Head of the argument list*/ }
    | /*epsilon*/                   { $$ = NULL; /*There are no arguments*/ }
    ;

arglist:
       argument argprime
                                    {
                                        ((ArgumentNode*)$1)->append($2);
                                        $$ = $1;
                                    }
    ;

argprime:
        COMMA arglist                   { $$ = $2; }
    | /*epsilon*/                   { $$ = NULL; }
    ;

argument:
        type ID
                                    {
                                        string type = $1;
                                        string id = $2;
                                        $$ = new ArgumentNode(type, id);
                                        $$->setLineNum(linenumber);
                                    }
    ;

funbody:
       vardeclarations statements      { 
                                        $$ = new FunctionBodyNode($1, $2);
                                        $$->setLineNum(num);
                                    }
    ;

vardeclarations:
               vardeclarations localdeclaration
                                    {
                                        ASTNode* cur = $1;
                                        if (cur)
                                        {
                                            ((DeclarationNode*)cur)->append($2);
                                            $$ = cur;
                                        } else
                                        {
                                            $$ = $2;
                                        }
                                    }
    | /*epsilon*/                   { $$ = NULL; }
    ;

localdeclaration:
                type ID SEMICOLON
                                    {
                                        string type = $1;
                                        string id = $2;
                                        $$ = new DeclarationNode(type, id);
                                        $$->setLineNum(linenumber);
                                    }

statements:
          statements statement
                                    {
                                        ASTNode* cur = $1;
                                        if ($1)
                                        {
                                            ((StatementNode*)$1)->append($2);
                                            $$ = $1;
                                        } else
                                        {
                                            $$ = $2;
                                        }
                                    }
    | /*epsilon*/                   { $$ = NULL; }
    ;

statement:
         functioncall SEMICOLON          { $$ = $1; }
    | assignment SEMICOLON          { $$ = $1; }
    | whileloop                     { $$ = $1; }
    | ifstatement                   { $$ = $1; }
    | return                        { $$ = $1; }
    ;

functioncall:
            ID OPAREN values CPAREN
                                    {
                                        string id = $1;
                                        ASTNode* vlist = $3;
                                        $$ = new FunctionCallNode(id, vlist);
                                        $$->setLineNum(linenumber);
                                    }
    ;

values:
      valuelist                       { $$ = $1; }
    | /**/                          { $$ = NULL; }
    ;

valuelist:
         valuemember valueprime
                                    {
                                        ((ValueListNode*)$1)->append($2);
                                        $$ = $1;
                                    }
    ;

valuemember:
           ID                              { 
                                        $$ = new ValueListNode("ID", $1);
                                        $$->setLineNum(linenumber);
                                    }
    | CHAR                          { 
                                        $$ = new ValueListNode($1);
                                        $$->setLineNum(linenumber); 
                                    }
    | STRING                        { 
$$ = new ValueListNode("String", $1); 
$$->setLineNum(linenumber);
}
    | INT                           { 
$$ = new ValueListNode($1); 
$$->setLineNum(linenumber);
}
    ;

valueprime:
          COMMA valuelist                 { $$ = $2; }
    | /**/                          { $$ = NULL; }
    ;

assignment:
          ID ASSIGNMENT term
                                    {
                                        string id = $1;
                                        $$ = new AssignmentNode(id, $3);
$$->setLineNum(linenumber);
                                    }
    ;

term:
    functioncall                    { $$ = $1; }
    | valueterm                     { $$ = $1; }
    | binop                         { $$ = $1; }
    | unop                          { $$ = $1; }
    ;

valueterm:
         ID                              { 
$$ = new ValueTermNode("ID", $1); 
$$->setLineNum(linenumber);
}
    | CHAR                          { 
$$ = new ValueTermNode($1); 
$$->setLineNum(linenumber);
}
    | STRING                        { 
$$ = new ValueTermNode("String", $1); 
$$->setLineNum(linenumber);
}
    | INT                           { 
$$ = new ValueTermNode($1); 
$$->setLineNum(linenumber);
}
    ;

binop:
     value binoperator value         { 
$$ = new BinopNode($1, $3, $2); 
$$->setLineNum(linenumber);
}
    | literal MULT value            { 
$$ = new BinopNode($1, $3, 10); 
$$->setLineNum(linenumber);
}
    ;

value:
     ID                              { 
$$ = new ValueNode("ID", $1); 
$$->setLineNum(linenumber);
}
    | literal                       { $$ = $1; }
    ;

literal:
       CHAR                            { 
$$ = new ValueNode($1); 
$$->setLineNum(linenumber);
}
    | STRING                        { 
$$ = new ValueNode("String", $1); 
$$->setLineNum(linenumber);
}
    | INT                           { 
$$ = new ValueNode($1); 
$$->setLineNum(linenumber);
}
    ;

binoperator:
           ADD                             { $$ = 0; }
    | SUB                           { $$ = 1; }
    | OR                            { $$ = 2; }
    | AND                           { $$ = 3; }
    | LT                            { $$ = 4; }
    | GT                            { $$ = 5; }
    | LTE                           { $$ = 6; }
    | GTE                           { $$ = 7; }
    | EQUAL                         { $$ = 8; }
    | NEQUAL                        { $$ = 9; }
    ;

unop:
	unoperator value			{ 
										$$ = new UnopNode($2, $1); 
										$$->setLineNum(linenumber);
									}
    ;

unoperator:
          SUB                             { $$ = 1; }
    | NOT                           { $$ = 11; }
    ;

whileloop:
         WHILE OPAREN value CPAREN OBRACE statements CBRACE
                                    {
                                        $$ = new WhileNode($3, $6);
$$->setLineNum($3->getLineNum());
                                    }
    ;

ifstatement:
           IF OPAREN value CPAREN OBRACE statements CBRACE
                                    { 
$$ = new IfNode($3, $6); 
$$->setLineNum($3->getLineNum());
}
    | IF OPAREN value CPAREN OBRACE statements CBRACE ELSE OBRACE statements CBRACE
                                    { 
$$ = new IfNode($3, $6, new ElseNode($10)); 
$$->setLineNum($3->getLineNum());
}
    ;

return:
      RETURN value SEMICOLON           { 
$$ = new ReturnNode($2); 
$$->setLineNum(linenumber);
}
    ;

%%

ASTNode* parse(char* filename){
    head = NULL;
    FILE *myfile = fopen(filename, "r");
    if (!myfile)
    {
        cout << "file " << filename << " was unable to open" << endl;
        exit(-1);
        return NULL;
    }
    yyin = myfile;
    yyparse();

if (head)
    {
        cout << head->toString() << endl;
    } else
    {
        cout << "head was still null" << endl;
    }

return head;
}

void yyerror(const char* s)
{
    cout << "Parse error on line " << linenumber << endl << s << endl;
    exit(-1);
}
