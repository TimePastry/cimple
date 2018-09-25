CPPS = ASTNodeDefines.cpp main.cpp PrettyPrintVisitor.cpp ListifyVisitor.cpp SymbolTableVisitor.cpp TypeCheckVisitor.cpp

all: cimple.l
	bison -d cimple.y
	flex cimple.l
	g++ -std=c++11 cimple.tab.c lex.yy.c $(CPPS) -o exec
