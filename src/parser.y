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
  //#include <direct.h>
  #include <mcrypt.h>
  #include <sys/stat.h>
  #include <limits.h>

#ifdef __APPLE__
  #include <libproc.h>
#endif
  //////////////////////////////////
  //WARNING:
  //This is the key to the encryption of your password:
  #define PWD_KEY 11105231919 // <== this is the key
  //Change this value, compile and delete the source file if you are worried about security
  //Warning: this program gives NO guarantee security

  //This is the seed of random
  #define SEED_RANDOM 1919
  //Changing this also ensures better security

  #define SEAS_FAIL -1
  #define SEAS_SUCC 0
  #define BUF_SIZE 1024
  #define SAFE_SIZE 1023
  #define TRUE 1
  #define FALSE 0

  typedef struct yy_buffer_state * YY_BUFFER_STATE;
  extern int yyparse();
  extern YY_BUFFER_STATE yy_scan_string(char * str);
  extern void yy_delete_buffer(YY_BUFFER_STATE buffer);

  int yylex(void);
  void yyerror(char const *s) {
    fprintf(stderr, "ERROR: %s\n", s);
  }

  //Path of the program
  char base_path[BUF_SIZE];
  //Initialize the path of the program
  int init_path() {
    char temp[PATH_MAX];
    memset(temp, 0, PATH_MAX);
    memset(base_path, 0, BUF_SIZE);

    int i;
    //Apple and linux seems to be diverting at this point
#ifdef __APPLE__
  if (proc_pidpath (getpid(), base_path, BUF_SIZE) <= 0) {
      perror("proc_pidpath");
      return SEAS_FAIL;
  }
#elif __linux__
    snprintf(base_path, SAFE_SIZE, "/proc/self/exe");
    //Use readlink to retrieve the base path
    if (readlink(temp, base_path, PATH_MAX) == -1) {
      perror("readlink");
      return SEAS_FAIL;
    }
#endif
    for (i = strlen(base_path) - 1; i > 0; i--) {
      if (base_path[i] == '/') {
        base_path[i] = '\0';
        break;
      }
    }
    printf("%s\n", base_path);
    return SEAS_SUCC;
  }

  //Create absolute path
  void create_full_path(char* dest, char* sub_path) {
    strncat(dest, base_path, SAFE_SIZE - strlen(dest));
    strncat(dest, "/", SAFE_SIZE - strlen(dest));
    strncat(dest, sub_path, SAFE_SIZE - strlen(dest));
  }

  int usr_fd;
  int port_fd;
  int run_fd;
  int runningfile;

  char user[BUF_SIZE];
  int port = 1;

  int rc;

  void lite_login(int port) {
    char cmd[BUF_SIZE];
    memset(cmd, 0, BUF_SIZE);
    snprintf(cmd, SAFE_SIZE, "ssh %s@lnxsrv0%d.seas.ucla.edu", user, port);
    system(cmd);
  }

  void lite_scp_to_local(int port, char* from, char* to) {
    char cmd[BUF_SIZE];
    memset(cmd, 0, BUF_SIZE);
    snprintf(cmd, SAFE_SIZE, "scp -r %s@lnxsrv0%d.seas.ucla.edu:%s %s", user, port, from, to);
    system(cmd);
  }

  void lite_scp_to_server(int port, char* from, char* to) {
    char cmd[BUF_SIZE];
    memset(cmd, 0, BUF_SIZE);
    snprintf(cmd, SAFE_SIZE, "scp -r %s %s@lnxsrv0%d.seas.ucla.edu:%s", from, user, port, to);
    system(cmd);
  }

  //char cwd[BUF_SIZE];
  char pwd[BUF_SIZE];
  char keystr[BUF_SIZE];
  MCRYPT td;
  /*
  int init_cwd() {
    memset(cwd, 0, BUF_SIZE);
    if (getcwd(cwd, BUF_SIZE-1) != NULL) {
       return SEAS_SUCC;
     } else {
       perror("working directory");
       return SEAS_FAIL;
     }
  }
  */
  int init_keystr() {
    memset(keystr, 0, BUF_SIZE);
    long digits = PWD_KEY;
    int one_digit;
    int count = 0;
    do {
      one_digit = (int) (digits % 10L);
      digits /= 10L;
      count++;
      if (count >= BUF_SIZE-1) {
        break;
      }
      keystr[count] = '0' + one_digit;
    } while (digits > 9);
    keystr[++count] = '0' + one_digit;
    //This will always succeed
    return SEAS_SUCC;
  }
  int init_mcrypt() {
    init_keystr();
    char *IV;
    int keysize = 16;
    td = mcrypt_module_open("twofish", NULL, "cfb", NULL);
    if (td == MCRYPT_FAILED) {
      return SEAS_FAIL;
    }
    IV = malloc(mcrypt_enc_get_iv_size(td));
    int i;
    srand(SEED_RANDOM);
    for (i = 0; i < mcrypt_enc_get_iv_size(td); i++) {
      IV[i] = rand();
    }
    i = mcrypt_generic_init(td, keystr, keysize, IV);
    if (i < 0) {
      mcrypt_perror(i);
      return SEAS_FAIL;
    }
    return SEAS_SUCC;
  }
  int init_pwd() {
    system("if [ -d data ]; then :; else mkdir data; fi");

    int pwd_fd = open("data/.seas_pwd", O_CREAT | O_RDWR);
    int ret = 0;
    if ((ret = read(pwd_fd, pwd, BUF_SIZE-1)) < 0) {
      perror("read");
      return SEAS_FAIL;
    }
    if (ret != 0) {
      mdecrypt_generic(td, pwd, ret);
    }
    return SEAS_SUCC;
  }
  int init() {
    int ret = init_path() | init_cwd() | init_mcrypt() | init_pwd();
    if (ret != SEAS_SUCC) {
      fprintf(stderr, "ERROR: initialization failed\n");
    }
    return ret;
  }
  void ending() {
    mcrypt_generic_end(td);
  }

  void save_pwd(char* new_pwd) {
    if (strlen(new_pwd) <= 0) {
      fprintf(stderr, "ERROR: Invalid password\n");
      return;
    }
    memset(pwd, 0, BUF_SIZE);
    strcat(pwd, new_pwd);
    int pwd_fd = open("data/.seas_pwd", O_TRUNC | O_RDWR);
    if (pwd_fd < 0) {
      fprintf(stderr, "ERROR: Password saving failed\n");
      return;
    }
    char* encoded = strdup(pwd);
    int len = strlen(encoded);
    mcrypt_generic(td, encoded, len);
    write(pwd_fd, encoded, len);
    fprintf(stdout, "* Password saved!\n");
  }
  void del_pwd() {
    if (strlen(pwd) <= 0) return;

    memset(pwd, 0, BUF_SIZE);
    if (truncate("data/.seas_pwd", 0) < 0) {
      fprintf(stderr, "ERROR: clear password file error\n");
      return;
    }
    fprintf(stdout, "* Password deleted\n");
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
%token <number> DEL BASH
%token <number> AUTO

%token <number> LEFT_ARROW RIGHT_ARROW
%token <number> EOL
%token <number> NUM
%token <number> EXIT
%token <number> KEY

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
| bash
| key
| del
;

scp: LNXSRV NAME RIGHT_ARROW NAME {
  lite_scp_to_local(port, $<string>2, $<string>4);
}
| LNXSRV NAME LEFT_ARROW NAME {
  lite_scp_to_server(port, $<string>4, $<string>2);
}
| NAME LEFT_ARROW LNXSRV NAME {
  lite_scp_to_local(port, $<string>4, $<string>1);
}
| NAME RIGHT_ARROW LNXSRV NAME {
  lite_scp_to_server(port, $<string>1, $<string>4);
}
;



server: LOGIN {
  lite_login(port);
}
| login num %prec lower {
  lite_login($<number>2);
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

bash: BASH {
  rc = fork();
  if (rc == 0) {
    execl("/bin/bash", "/bin/bash", NULL);
    perror("exec bash");
  } else {
    wait(NULL);
  }
}
;

key: KEY %prec lower {
  write(STDOUT_FILENO, "Enter password: ", BUF_SIZE);
  struct termios tattr;
  tcgetattr(STDIN_FILENO, &tattr);
  tattr.c_lflag &= ~ECHO;
  char buf[BUF_SIZE];
  memset(buf, 0, BUF_SIZE);
  if (read(STDIN_FILENO, buf, BUF_SIZE-1) <= 0) {
    write(STDERR_FILENO, "ERROR: Invalid password\n", BUF_SIZE);
  } else {
    save_pwd(buf);
  }
  tattr.c_lflag |= ECHO;
}
;

del: DEL KEY %prec higher {
  del_pwd();
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

  if (init() != SEAS_SUCC) {
    exit(EXIT_FAILURE);
  }

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

  system("if [ -d temp ]; then :; else mkdir temp; fi");
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
