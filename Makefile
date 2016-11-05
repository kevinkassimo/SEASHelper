override LOC:=`printf "%q\n" "$$PWD"`

all: build
mark: seas
	@ echo "\nalias seas=\"$(LOC)/seas\"" >> ~/.bash_profile
	@ source ~/.bash_profile
unmark:
	@ sed -ie '/^alias seas=/d' ~/.bash_profile
seas:
	@ if [ -e "seas" ]; then :; else flex src/token.lex; bison -d src/parser.y; gcc -o seas parser.tab.c lex.yy.c -ll -ledit -lmcrypt; fi

build:
	@ flex src/token.lex
	@ bison -d src/parser.y
	@ gcc -o seas parser.tab.c lex.yy.c -ll -ledit -I/usr/local/Cellar/mcrypt/2.6.8/include -L/usr/local/Cellar/mcrypt/2.6.8/lib -lmcrypt

required:
	@ brew install flex
	@ brew install bison
macbuildfix:
	@ brew unlink bison
	@ brew link bison --force
	@ source ~/.bash_profile
clean: unmark
	@ rm -fr "data" "parser.tab.c" "parser.tab.h" "lex.yy.c"
