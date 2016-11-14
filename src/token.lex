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
  char comment[BUF_SIZE];
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
`c {
  system("clear");
}

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
"$" {return BASH;}
auto {return AUTO;}
help {return HELP;}
repo {return REPO;}
info {return INFO;}
bash {return BASH;}
clear {
  system("clear");
}

[1-9] {
  yylval.number = atoi(yytext);
  return NUM;
}
[ \t] {}
\n {return EOL;}


(\"((?:\\.|[^!@$ \t\n\"]){1}|(?:\\.|[^\t\n\"]){2,})\") {
  yylval.string = (char*) malloc(yyleng+1);
  strncpy(yylval.string, &yytext[1], strlen(yytext)-2);
  return NAME;
}
(\'((?:\\.|[^!@$ \t\n\']){1}|(?:\\.|[^\t\n\']){2,})\') {
  yylval.string = (char*) malloc(yyleng+1);
  strncpy(yylval.string, &yytext[1], strlen(yytext)-2);
  return NAME;
}
((?:\\.|[^!@$ \t\n\'\"]){1}|(?:\\.|[^ \t\n\'\"]){2,}) {
  yylval.string = (char*) malloc(yyleng+1);
  if ((yytext[0] == '\'' && yytext[strlen(yytext)-1] == '\'') || (yytext[0] == '\"' && yytext[strlen(yytext)-1] == '\"')) {
    strncpy(yylval.string, &yytext[1], strlen(yytext)-2);
  } else {
    strcpy(yylval.string, yytext);
  }
  return NAME;
}

^\/\/.*$ {
  memset(comment, 0, BUF_SIZE);
  int i;
  for (i = 2; i < strlen(yytext); i++) {
    if (yytext[i] != ' ') {
      break;
    }
  }
  strncpy(comment, &yytext[i], BUF_SIZE-1);
  printf("COMMENT: %s\n", comment);
}

%%
