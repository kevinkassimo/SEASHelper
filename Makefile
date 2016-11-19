override LOC:=`printf "%q\n" $$PWD`
override LINUXLOC:=`/bin/bash -c 'printf %q "$(CURDIR)"'`

OS:=$(shell uname)

all: build
mark:
ifeq ($(OS), Darwin)
	@ echo "\nalias seas=\"$(LOC)/seas_mac\"" >> ~/.bash_profile
	@ source ~/.bash_profile
else
	@ echo "\nalias seas=\"$(LINUXLOC)/seas_lnx\"" >> ~/.bashrc
endif

unmark:
ifeq ($(OS), Darwin)
	@ sed -ie '/^alias seas=/d' ~/.bash_profile
else
	@ sed -ie '/^alias seas=/d' ~/.bashrc
endif

build:
ifeq ($(OS), Darwin)
	@ flex src/token.lex
	@ bison -d src/parser.y
	@ gcc -o seas_mac parser.tab.c lex.yy.c -ll -ledit -I/usr/local/Cellar/mcrypt/2.6.8/include -L/usr/local/Cellar/mcrypt/2.6.8/lib -lmcrypt
else
	@ flex src/token.lex
	@ bison -d src/parser.y
	@ gcc -o seas_lnx parser.tab.c lex.yy.c -ll -ledit -lmcrypt
endif

required:
ifeq ($(OS), Darwin)
	@ brew install flex
	@ brew install bison
else
	@ sudo apt-get install mcrypt
	@ sudo apt-get install ledit
	@ sudo apt-get install flex
	@ sudo apt-get install bison
endif

macbuildfix:
	@ brew unlink bison
	@ brew link bison --force
	@ source ~/.bash_profile
clean: unmark
	@ rm -fr "data" "parser.tab.c" "parser.tab.h" "lex.yy.c"
