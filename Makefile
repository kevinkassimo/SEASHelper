all: build 
mark: build
	@ echo "\nalias seas=\"$$PWD/seas\"\n" >> ~/.bash_profile
#	@ echo $$PWD >> ~/.bash_profile
#	@ echo "/seas\n" >> ~/.bash_profile
build: 
	@ flex token.lex
	@ bison -d parser.y
	@ gcc -o seas parser.tab.c lex.yy.c -ll
required:
	@ brew install flex
	@ brew install bison
clean:
	@ rm -f "seas" "parser.tab.c" "parser.tab.h" "lex.yy.c"