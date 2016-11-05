%option noyywrap
%{
  #include <stdio.h>
  #include <stdlib.h>
  #include <string.h>
  #include <unistd.h>
  #include <fcntl.h>
  #include <getopt.h>

  #include "parser.tab.h"
#ifndef BUF_SIZE
#define BUF_SIZE 1024
#endif
%}

%%
`u {return USR;}
`l {return LOGIN;}
`s {return SRV;}
`e {return EXIT;}
`h {return HELP;}
`r {return REPO;}
`i {return INFO;}
`b {return BASH;}
`k {return KEY;}
`d {return DEL;}

user {return USR;}
usr {return USR;}
server {return SRV;}
srv {return SRV;}
log {return LOGIN;}
login {return LOGIN;}
exit {return EXIT;}

"@" {return LNXSRV;}
"<=" {return LEFT_ARROW;}
"<-" {return LEFT_ARROW;}
"=>" {return RIGHT_ARROW;}
"->" {return RIGHT_ARROW;}

key {return KEY;}
setkey {return KEY;}
password {return KEY;}
del {return DEL;}
delete {return DEL;}

"!" {return AUTO;}
auto {return AUTO;}
help {return HELP;}
repo {return REPO;}
info {return INFO;}
bash {return BASH;}

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
