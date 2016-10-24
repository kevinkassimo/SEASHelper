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
  #include <stdbool.h>
  #include <histedit.h>
  #include <termios.h>

  typedef struct yy_buffer_state * YY_BUFFER_STATE;
  extern int yyparse();
  extern YY_BUFFER_STATE yy_scan_string(char * str);
  extern void yy_delete_buffer(YY_BUFFER_STATE buffer);

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

      execl("./temp/.seas_ssh", NULL);

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
        execl("./temp/.seas_ssh", NULL);

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
        execl("./temp/.seas_ssh", NULL);

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
        execl("./temp/.seas_ssh", NULL);

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
        execl("./temp/.seas_ssh", NULL);

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

%token <number> USR HELP REPO
%token <number> PORT LOGIN
%token <number> STAT
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
| prog EOL {/*printf("> ");*/}
| prog body EOL {/*printf("> ");*/}
| prog EXIT EOL {exit(EXIT_SUCCESS);}
| prog HELP EOL {
  printf("* Usage *\n\tuser *username* : set default username\n\tport *portnum* : set default server number\n\tlogin *optional_portnum* : login to server\n\t@ *server_path* => *local_path\n\t*local_path* => @ *server_path* : download and upload files/directory\n\texit : exit program\n");
}
| prog REPO EOL {
#ifdef __APPLE__
system("open https://github.com/kevinkassimo/SEASHelper");
#elif __linux__
system("xdg-open https://github.com/kevinkassimo/SEASHelper");
#endif
}
;

body: setusr
| server
| setport
| scp
| stat
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
  chr[0] = port + '0';
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

stat: STAT {
  printf("* current username: %s\n", user);
  printf("* current default port: %d\n", port);
}
;

%%

#define HIST_SIZE 800
#define BUF_SIZE 1024
#define TRUE 1
#define FALSE 0

const char* input;
char* prompt(EditLine *e) {
	return "> ";
}
int read_count;
int p_fd;

EditLine *_el;
History *_hist;

void ATEXIT_handler() {
	history_end(_hist);
	el_end(_el);
}

void print_welcome(void) {
	printf(">> SEASnet shortcut v0.2 <<\n");
	printf("* current username: %s\n", user);
	printf("* current default port: %d\n", port);
}

int main(int argc, char *argv[]) {
	atexit(ATEXIT_handler);

	// This holds all the state for our line editor
	_el = el_init(argv[0], stdin, stdout, stderr);
	el_set(_el, EL_PROMPT, &prompt);
	el_set(_el, EL_EDITOR, "emacs");
	// This holds the info for our history
	HistEvent _ev;
	_hist = history_init();
	if (_hist == 0) {
		fprintf(stderr, "History could not be initialized\n");
		exit(EXIT_FAILURE);
	}
	//Set the size of the history
	history(_hist, &_ev, H_SETSIZE, HIST_SIZE);
	//This sets up the call back functions for history functionality
	el_set(_el, EL_HIST, history, _hist);

  memset(user, 0, BUF_SIZE);

  system("if [ -d mkdir ]; then :; else mkdir temp; fi");
  usr_fd = open("temp/.seas_usr", O_RDWR | O_CREAT, S_IRWXU);
  port_fd = open("temp/.seas_port", O_RDWR | O_CREAT, S_IRWXU);
  run_fd = open("temp/.seas_ssh", O_CREAT | O_TRUNC | O_RDWR, S_IRWXU);

  if (read(usr_fd, user, BUF_SIZE-1) <= 0) {
    sprintf(user, "%s", "set_usr_name_please");
  }
  char* port_temp[BUF_SIZE];
  if (read(port_fd, port_temp, BUF_SIZE-1) <= 0) {
    port = 1;
  } else {
    port = atoi((const char*) port_temp);
  }

  //fix weird bug
  if (port == 0) {
    port = 1;
  }

  //Print version
  print_welcome();

  //looping input and parsing
	while (true) {
		input = el_gets(_el, &read_count);

		if (read_count > 0) {
			history(_hist, &_ev, H_ENTER, input);
		} else {
			exit(EXIT_SUCCESS);
		}

    YY_BUFFER_STATE buffer = yy_scan_string((char*) input);
    yyparse();
    yy_delete_buffer(buffer);
	}

	exit(EXIT_SUCCESS);
}
