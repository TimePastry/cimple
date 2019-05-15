CPPS = ast/ASTNodeDefines.cpp main.cpp CESKInterpreter.cpp CESKSearch.cpp PrettyPrintVisitor.cpp ListifyVisitor.cpp SymbolTableVisitor.cpp TypeCheckVisitor.cpp

all: $(CPPS)
	bison -d flexbison/cimple.y
	flex flexbison/cimple.l
	g++ -std=c++11 cimple.tab.c lex.yy.c $(CPPS) -o cim

clean:
	rm -f cim
	rm -f lex.yy.c
	rm -f cimple.tab.c
	rm -f cimple.tab.h
