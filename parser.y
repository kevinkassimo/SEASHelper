%{
  #include <stdio.h>
  #include <stdlib.h>
  #include <string.h>
  #include <unistd.h>
  #include <fcntl.h>
  #include <getopt.h>
  #include <signal.h>
  #include <errno.h>
  #include <sys/ioctl.h>
  #include <sys/types.h>
  #include <sys/wait.h>

  int yylex(void);
  void yyerror(char const *s) {
    fprintf(stderr, "ERROR: %s\n", s);
  }

  #define BUF_SIZE 1024
  #define TRUE 1
  #define FALSE 0

  int fd;
  int run_fd;
  int runningfile;
  char user[BUF_SIZE];

  int rc;

  void ssh_login(int port) {
    /*
    ftruncate(run_fd, 0);
    char cmd[BUF_SIZE];
    sprintf(cmd, "ssh %s@lnxsrv0%d.seas.ucla.edu", user, port);
    char* run_type = strdup("#!/bin/bash\n");
    lseek(run_fd, 0, SEEK_SET);
    write(run_fd, run_type, strlen(run_type));
    write(run_fd, cmd, strlen(cmd));
    execl("./.seas_ssh", NULL);
    */
    rc = fork();
    if (rc == 0) {
      char cmd[BUF_SIZE];
      memset(cmd, 0, BUF_SIZE);
      sprintf(cmd, "ssh %s@lnxsrv0%d.seas.ucla.edu", user, port);
      char* run_type = strdup("#!/bin/bash\n");

      ftruncate(run_fd, 0);
      //Fix wrong fd position bug
      lseek(run_fd, 0, SEEK_SET);

      write(run_fd, run_type, strlen(run_type));
      write(run_fd, cmd, strlen(cmd));

      execl("./.seas_ssh", NULL);

      perror("Failed");
      exit(EXIT_FAILURE);
    } else {
      int status;
      waitpid(rc, &status, 0);
    }
  }

  void ssh_scp_to_local(int is_folder, int port, char* from, char* to) {
    rc = fork();
    if (rc == 0) {
      char _from[BUF_SIZE];
      memset(_from, 0, BUF_SIZE);
      sprintf(_from, "%s@lnxsrv0%d.seas.ucla.edu:%s", user, port, from);

      if (is_folder) {
        ftruncate(run_fd, 0);
        //Fix wrong fd position bug
        lseek(run_fd, 0, SEEK_SET);
        char cmd[BUF_SIZE];
        sprintf(cmd, "scp -r %s %s\n", _from, to);
        char* run_type = strdup("#!/bin/bash\n");
        /*lseek(run_fd, 0, SEEK_SET);*/
        write(run_fd, run_type, strlen(run_type));
        write(run_fd, cmd, strlen(cmd));
        execl("./.seas_ssh", NULL);

        perror("Failed");
        exit(EXIT_FAILURE);
      } else {
        ftruncate(run_fd, 0);
        //Fix wrong fd position bug
        lseek(run_fd, 0, SEEK_SET);
        char cmd[BUF_SIZE];
        sprintf(cmd, "scp %s %s", _from, to);
        char* run_type = strdup("#!/bin/bash\n");
        /*lseek(run_fd, 0, SEEK_SET);*/
        write(run_fd, run_type, strlen(run_type));
        write(run_fd, cmd, strlen(cmd));
        execl("./.seas_ssh", NULL);

        perror("Failed");
        exit(EXIT_FAILURE);
      }
    } else {
      int status;
      waitpid(rc, &status, 0);
    }
  }

  void ssh_scp_to_server(int is_folder, int port, char* from, char* to) {
    rc = fork();
    if (rc == 0) {
      char _to[BUF_SIZE];
      memset(_to, 0, BUF_SIZE);
      sprintf(_to, "%s@lnxsrv0%d.seas.ucla.edu:%s", user, port, to);

      if (is_folder) {
        ftruncate(run_fd, 0);
        //Fix wrong fd position bug
        lseek(run_fd, 0, SEEK_SET);

        char cmd[BUF_SIZE];
        sprintf(cmd, "scp -r %s %s", from, _to);
        char* run_type = strdup("#!/bin/bash\n");
        /*lseek(run_fd, 0, SEEK_SET);*/
        write(run_fd, run_type, strlen(run_type));
        write(run_fd, cmd, strlen(cmd));
        execl("./.seas_ssh", NULL);

        perror("Failed");
        exit(EXIT_FAILURE);
      } else {
        ftruncate(run_fd, 0);
        //Fix wrong fd position bug
        lseek(run_fd, 0, SEEK_SET);

        char cmd[BUF_SIZE];
        sprintf(cmd, "scp %s %s", from, _to);
        char* run_type = strdup("#!/bin/bash\n");
        /*lseek(run_fd, 0, SEEK_SET);*/
        write(run_fd, run_type, strlen(run_type));
        write(run_fd, cmd, strlen(cmd));
        execl("./.seas_ssh", NULL);

        perror("Failed");
        exit(EXIT_FAILURE);
      }
    } else {
      int status;
      waitpid(rc, &status, 0);
    }
  }
%}

%union {
  int number;
  char* string;
}

%token <number> CURSOR_LEFT CURSOR_RIGHT
%token <number> USR
%token <number> PORT
%token <number> DL DLDIR
%token <number> EOL
%token <number> NUM
%token <number> EXIT

%token <string> NAME

%precedence lowest
%precedence lower
%precedence higher
%precedence cursor

%%

prog: %empty {}
| prog EOL {printf("> ");}
| prog body EOL {printf("> ");}
| prog EXIT EOL {exit(EXIT_SUCCESS);}
;

body: setusr
| server
| scp
;

scp: DL port num NAME NAME %prec higher {
  int portnum = $<number>3;
  ssh_scp_to_local(FALSE, portnum, $<string>4, $<string>5);
}
| DL NAME port num NAME %prec higher {
  int portnum = $<number>4;
  ssh_scp_to_server(FALSE, portnum, $<string>2, $<string>5);
}
| DLDIR port num NAME NAME %prec higher {
  int portnum = $<number>3;
  ssh_scp_to_local(TRUE, portnum, $<string>4, $<string>5);
}
| DLDIR NAME port num NAME %prec higher {
  int portnum = $<number>4;
  ssh_scp_to_server(TRUE, portnum, $<string>2, $<string>5);
}
;


server: port num %prec lower {
  ssh_login($<number>2);
}
;
port: PORT
;
num: NUM
;



setusr: usr name {
  /*Truncate only when we reset the new name*/
  ftruncate(fd, 0);
  /*Set fd to the start of file*/
  lseek(fd, 0, SEEK_SET);
  memset(user, 0, BUF_SIZE);
  strcat(user, $<string>2);
  write(fd, user, strlen(user));
}
;
usr: USR
;
name: NAME
;

/*
csrright: CURSOR_RIGHT %prec higher {printf("rec");}
;
csrleft: CURSOR_LEFT %prec higher
;
*/

%%

int main() {
  memset(user, 0, BUF_SIZE);

  fd = open(".seas_settings", O_RDWR | O_CREAT, S_IRWXU);
  run_fd = open(".seas_ssh", O_CREAT | O_TRUNC | O_RDWR, S_IRWXU);

  read(fd, user, BUF_SIZE-1);

  printf(">> SEASnet shortcut v0.1 <<\n> ");

  yyparse();
}
