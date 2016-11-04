all: build
mark: seas
	@ echo "\nalias seas=\"$$PWD/seas\"\n" >> ~/.bash_profile

seas:
	@ if [ -e "seas" ]; then :; else flex src/token.lex; bison -d src/parser.y; gcc -o seas parser.tab.c lex.yy.c -ll -ledit -lmcrypt; fi

build:
	@ flex src/token.lex
	@ bison -d src/parser.y
#	@ gcc -o seas parser.tab.c lex.yy.c -ll -ledit -lmcrypt
	@ gcc -o seas parser.tab.c lex.yy.c -ll -ledit -I/usr/local/Cellar/mcrypt/2.6.8/include -L/usr/local/Cellar/mcrypt/2.6.8/lib -lmcrypt
	@ if [ -d "temp" ]; then :; else mkdir temp; fi
required:
	@ brew install flex
	@ brew install bison
macbuildfix:
	@ brew unlink bison
	@ brew link bison --force
	@ source ~/.bash_profile
clean:
#	@ rm -f "seas" "parser.tab.c" "parser.tab.h" "lex.yy.c" ".seas_ssh" ".seas_usr" ".seas_port"
	@ rm -fr "temp" "parser.tab.c" "parser.tab.h" "lex.yy.c"
