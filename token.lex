%option noyywrap
%{
  #include <stdio.h>
  #include <stdlib.h>
  #include <string.h>
  #include <unistd.h>
  #include <fcntl.h>
  #include <getopt.h>
  #include <ncurses.h>

  #include "parser.tab.h"
#ifndef BUF_SIZE
#define BUF_SIZE 1024
#endif
%}

%%
$u {return USR;}
$p {return PORT;}
$f {return DL;}
$d {return DLDIR;}
$e {return EXIT;}

usr {return USR;}
port {return PORT;}
dfile {return DL;}
ddir {return DLDIR;}
exit {return EXIT;}
=> {/*ignored*/}

[1-9] {
  yylval.number = atoi(yytext);
  return NUM;
}
[ \t] {/*DO NOTHING*/}
\n {return EOL;}
[a-zA-Z0-9\/\._\-~\\]+ {
  yylval.string = (char*)malloc(yyleng+1);
  strcpy(yylval.string, yytext);
  return NAME;
}

%%
