all: build
mark: seas
	@ echo "\nalias seas=\"$$PWD/seas\"\n" >> ~/.bash_profile

seas:
	@ if [ -e "seas" ]; then :; else flex token.lex; bison -d parser.y; gcc -o seas parser.tab.c lex.yy.c -ll; fi

build:
	@ flex token.lex
	@ bison -d parser.y
	@ gcc -o seas parser.tab.c lex.yy.c -ll
required:
	@ brew install flex
	@ brew install bison
clean:
	@ rm -f "seas" "parser.tab.c" "parser.tab.h" "lex.yy.c" ".seas_ssh" ".seas_usr" ".seas_port"
