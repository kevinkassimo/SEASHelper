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

  int usr_fd;
  int port_fd;
  int run_fd;
  int runningfile;

  char user[BUF_SIZE];
  int port = 1;

  int rc;

  void ssh_login(int port) {
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
%token <number> USR HELP REPO
%token <number> PORT
%token <number> LOGIN
%token <number> LNXSRV
%token <number> LEFT_ARROW RIGHT_ARROW
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
| prog HELP EOL {
  printf("* Usage *\n\tuser *username* : set default username\n\tport *portnum* : set default server number\n\tlogin *optional_portnum* : login to server\n\t@ *server_path* => *local_path\n\t*local_path* => @ *server_path* : download and upload files/directory\n\texit : exit program\n> ");
}
| prog REPO EOL {
#ifdef __APPLE__
system("open https://github.com/kevinkassimo/SEASHelper");
#elif __linux__
system("xdg-open https://github.com/kevinkassimo/SEASHelper");
#endif
printf("> ");
}
;

body: setusr
| server
| setport
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
| LNXSRV NAME RIGHT_ARROW NAME {
  ssh_scp_to_local(TRUE, port, $<string>2, $<string>4);
}
| LNXSRV NAME LEFT_ARROW NAME {
  ssh_scp_to_server(TRUE, port, $<string>4, $<string>2);
}
| NAME LEFT_ARROW LNXSRV NAME {
  ssh_scp_to_local(TRUE, port, $<string>4, $<string>1);
}
| NAME RIGHT_ARROW LNXSRV NAME {
  ssh_scp_to_server(TRUE, port, $<string>1, $<string>4);
}
;



server: LOGIN {
  ssh_login(port);
}
| login num %prec lower {
  ssh_login($<number>2);
}
;
login: LOGIN
;

setport: port num %prec lower {
  /*Truncate only when we reset the new name*/
  ftruncate(port_fd, 0);
  /*Set fd to the start of file*/
  lseek(port_fd, 0, SEEK_SET);
  port = $<number>2;
  char* chr = (char*) malloc(sizeof(char) * 2);
  chr[0] = port + '\0';
  chr[1] = '\0';
  write(port_fd, chr, strlen(chr));
  free(chr);
  printf("* Default login server has been changed into # %d\n", port);
}
;
port: PORT
;
num: NUM
;



setusr: usr name {
  /*Truncate only when we reset the new name*/
  ftruncate(usr_fd, 0);
  /*Set fd to the start of file*/
  lseek(usr_fd, 0, SEEK_SET);
  memset(user, 0, BUF_SIZE);
  strcat(user, $<string>2);
  write(usr_fd, user, strlen(user));
  printf("* Username has been changed into: %s\n", user);
}
;
usr: USR
;
name: NAME
;

%%

int main() {
  memset(user, 0, BUF_SIZE);

  usr_fd = open(".seas_usr", O_RDWR | O_CREAT, S_IRWXU);
  port_fd = open(".seas_port", O_RDWR | O_CREAT, S_IRWXU);
  run_fd = open(".seas_ssh", O_CREAT | O_TRUNC | O_RDWR, S_IRWXU);


  if (read(usr_fd, user, BUF_SIZE-1) <= 0) {
    sprintf(user, "%s", "set_usr_name_please");
  }
  char* port_temp[BUF_SIZE];
  if (read(port_fd, port_temp, BUF_SIZE-1) <= 0) {
    port = 1;
  } else {
    port = atoi((const char*) port_temp);
  }

  printf(">> SEASnet shortcut v0.1 <<\n");
  printf("* current username: %s\n", user);
  printf("* current default port: %d\n", port);
  printf("> ");
  yyparse();
}
