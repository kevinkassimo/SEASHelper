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
`p {return PORT;}
`f {return DL;}
`d {return DLDIR;}
`e {return EXIT;}
`h {return HELP;}
`r {return REPO;}
`s {return STAT;}

user {return USR;}
"user:" {return USR;}
port {return PORT;}
"port:" {return PORT;}
log {return LOGIN;}
login {return LOGIN;}
fcp {return DL;}
dcp {return DLDIR;}
exit {return EXIT;}

"@" {return LNXSRV;}
srv {return LNXSRV;}
server {return LNXSRV;}
"<=" {return LEFT_ARROW;}
"=>" {return RIGHT_ARROW;}

help {return HELP;}
repo {return REPO;}
stat {return STAT;}

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
