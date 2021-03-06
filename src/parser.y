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

  //VERY NICE cursor movement LOL
  #define cursorforward(x) printf("\033[%dC", (x))
  #define cursorbackward(x) printf("\033[%dD", (x))
  #define RESET   "\033[0m"
  #define BLACK   "\033[30m"      /* Black */
  #define RED     "\033[31m"      /* Red */
  #define GREEN   "\033[32m"      /* Green */
  #define YELLOW  "\033[33m"      /* Yellow */
  #define BLUE    "\033[34m"      /* Blue */
  #define MAGENTA "\033[35m"      /* Magenta */
  #define CYAN    "\033[36m"      /* Cyan */
  #define WHITE   "\033[37m"      /* White */
  #define BOLDBLACK   "\033[1m\033[30m"      /* Bold Black */
  #define BOLDRED     "\033[1m\033[31m"      /* Bold Red */
  #define BOLDGREEN   "\033[1m\033[32m"      /* Bold Green */
  #define BOLDYELLOW  "\033[1m\033[33m"      /* Bold Yellow */
  #define BOLDBLUE    "\033[1m\033[34m"      /* Bold Blue */
  #define BOLDMAGENTA "\033[1m\033[35m"      /* Bold Magenta */
  #define BOLDCYAN    "\033[1m\033[36m"      /* Bold Cyan */
  #define BOLDWHITE   "\033[1m\033[37m"      /* Bold White */
  void print_warning(char* msg) {
    fprintf(stderr, BOLDYELLOW"WARNING: %s"RESET, msg);
  }
  void print_error(char* msg) {
    fprintf(stderr, BOLDRED"ERROR: %s"RESET, msg);
  }
  void print_success(char* msg) {
    fprintf(stderr, BOLDGREEN"%s"RESET, msg);
  }
  void print_normal(char* msg) {
    fprintf(stderr, BOLDBLACK"%s"RESET, msg);
  }

  typedef struct yy_buffer_state * YY_BUFFER_STATE;
  extern int yyparse();
  extern YY_BUFFER_STATE yy_scan_string(char * str);
  extern void yy_delete_buffer(YY_BUFFER_STATE buffer);

  int yylex(void);
  void yyerror(char const *s) {
    if (strcmp(s, "syntax error") == 0) {
      fprintf(stderr, BOLDRED"ERROR: invalid command. Please check README\n"RESET);
    } else {
      fprintf(stderr, BOLDRED"ERROR: %s\n"RESET, s);
    }
  }

  void bash_str_escaping(char* str, char* out_str) {
    int i, j;
    for (i = 0, j = 0; i < strlen(str); i++, j++) {
      if (str[i] == '&' || str[i] == '|' || str[i] == '\'' || str[i] == '\''|| str[i] == '\"' || str[i] == '?' || str[i] == '*' || str[i] == '%' || str[i] == '<' || str[i] == '>' || str[i] == ' ') {
        if (i > 0) {
          if (i > 1) {
            if (str[i-1] != '\\') {
              out_str[j] = '\\';
              j++;
            } else {
              if (str[i-2] == '\\') {
                out_str[j] = '\\';
                j++;
              }
            }
          } else {
            if (str[i-1] != '\\') {
              out_str[j] = '\\';
              j++;
            }
          }
        } else {
          out_str[j] = '\\';
          j++;
        }
      }
      out_str[j] = str[i];
    }
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
    snprintf(temp, SAFE_SIZE, "/proc/%d/exe", getpid());
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
    return SEAS_SUCC;
  }

  //Create absolute path
  void create_full_path(char* dest, char* sub_path) {
    strncat(dest, base_path, SAFE_SIZE - strlen(dest));
    strncat(dest, "/", SAFE_SIZE - strlen(dest));
    strncat(dest, sub_path, SAFE_SIZE - strlen(dest));
  }

  int usr_fd;
  int srv_fd;
  int run_fd;
  int runningfile;

  char user[BUF_SIZE];
  int srv = 1;

  int rc;

  void lite_login(int srv) {
    if (strcmp(user, "(null)") == 0) {
      print_warning("Please, set your username! (with command `usr <username>`)\n");
      return;
    }
    char cmd[BUF_SIZE];
    memset(cmd, 0, BUF_SIZE);
    snprintf(cmd, SAFE_SIZE, "ssh %s@lnxsrv0%d.seas.ucla.edu", user, srv);
    system(cmd);
  }

  void lite_scp_to_local(int srv, char* from, char* to) {
    if (strcmp(user, "(null)") == 0) {
      print_warning("Please, set your username! (with command `usr <username>`)\n");
      return;
    }
    char cmd[BUF_SIZE];
    memset(cmd, 0, BUF_SIZE);
    snprintf(cmd, SAFE_SIZE, "scp -r %s@lnxsrv0%d.seas.ucla.edu:%s %s", user, srv, from, to);
    system(cmd);
  }

  void lite_scp_to_server(int srv, char* from, char* to) {
    if (strcmp(user, "(null)") == 0) {
      print_warning("Please, set your username! (with command `usr <username>`)\n");
      return;
    }
    char cmd[BUF_SIZE];
    memset(cmd, 0, BUF_SIZE);
    snprintf(cmd, SAFE_SIZE, "scp -r %s %s@lnxsrv0%d.seas.ucla.edu:%s", from, user, srv, to);
    system(cmd);
  }

  char pwd_path[BUF_SIZE];
  char* homedir = NULL;
  char pwd[BUF_SIZE];
  char keystr[BUF_SIZE];
  int pwd_fd;
  MCRYPT td;

  int hasdir(char* path, int should_create, mode_t perm) {
    struct stat st;
    //If the directory exists, return TRUE
    if (stat(path, &st) == 0 && S_ISDIR(st.st_mode)) {
      return TRUE;
    } else {
      //Otherwise, optionally create the folder
      if (should_create == TRUE) {
        mkdir(path, perm);
      }
      return FALSE;
    }
  }
  int hasfile(char* path, int should_create, mode_t s_perm) {
    if (access(path, F_OK) != -1) {
      return TRUE;
    } else {
      if (should_create == TRUE) {
        creat(path, s_perm);
      }
      return FALSE;
    }
  }
  int getfilefd(char* path, int should_create, int o_flag, mode_t s_perm) {
    if (access(path, F_OK) != -1) {
      return open(path, o_flag);
    } else {
      if (should_create == TRUE) {
        creat(path, s_perm);
        return open(path, o_flag);
      }
      return -1;
    }
  }

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
    //Clear pwd
    memset(pwd_path, 0, BUF_SIZE);
    memset(pwd, 0, BUF_SIZE);

    //try find/create base directory
    char working_path[BUF_SIZE];
    memset(working_path, 0, BUF_SIZE);
    snprintf(working_path, SAFE_SIZE, "%s/data", base_path);
    hasdir(working_path, TRUE, S_IRWXU);

    //now have .seas_pwd file
    strncat(working_path, "/.seas_pwd", SAFE_SIZE - strlen(working_path));

    pwd_fd = getfilefd(working_path, TRUE, O_RDWR, 0777);
    int ret;
    if ((ret = read(pwd_fd, pwd, SAFE_SIZE)) < 0) {
      perror("read_pwd");
      return SEAS_FAIL;
    }
    //Save path for later easy use
    strncpy(pwd_path, working_path, SAFE_SIZE);

    if (ret != 0) {
      mdecrypt_generic(td, pwd, ret);
      //fprintf(stderr, "length: %d weird: %s", ret, pwd);
    } else {
      //fprintf(stderr, "???: %s", pwd);
    }
    return SEAS_SUCC;
  }
  int init_usr() {
    memset(user, 0, BUF_SIZE);

    char working_path[BUF_SIZE];
    memset(working_path, 0, BUF_SIZE);
    snprintf(working_path, SAFE_SIZE, "%s/data", base_path);
    hasdir(working_path, TRUE, S_IRWXU);

    //now have .seas_pwd file
    strncat(working_path, "/.seas_usr", SAFE_SIZE - strlen(working_path));

    usr_fd = getfilefd(working_path, TRUE, O_RDWR, 0777);
    int ret;
    if ((ret = read(usr_fd, user, SAFE_SIZE)) < 0) {
      perror("read_usr");
      return SEAS_FAIL;
    }
    if (ret == 0) {
      strncpy(user, "(null)", SAFE_SIZE - strlen(user));
    }
    return SEAS_SUCC;
  }
  int init_srv() {
    char working_path[BUF_SIZE];
    memset(working_path, 0, BUF_SIZE);
    snprintf(working_path, SAFE_SIZE, "%s/data", base_path);
    hasdir(working_path, TRUE, S_IRWXU);

    //now have .seas_pwd file
    strncat(working_path, "/.seas_srv", SAFE_SIZE - strlen(working_path));

    srv_fd = getfilefd(working_path, TRUE, O_RDWR, 0777);
    int ret;
    char temp;
    if ((ret = read(srv_fd, &temp, 1)) < 0) {
      perror("read_srv");
      return SEAS_FAIL;
    }
    if (ret == 0) {
      srv = 1;
    } else {
      srv = atoi(&temp);
    }
    return SEAS_SUCC;
  }


  int init() {
    if ((homedir = getenv("HOME")) == NULL) {
      print_error("Something is wrong with your $HOME\n");
    }
    int ret = init_path() | init_mcrypt() | init_pwd() | init_usr() | init_srv();
    if (ret != SEAS_SUCC) {
      print_error("Initialization failed\n");
    }
    return ret;
  }

  void ending() {
    close(pwd_fd);
    close(usr_fd);
    close(srv_fd);
    mcrypt_generic_end(td);
  }

  void save_pwd(char* new_pwd) {
    if (strlen(new_pwd) <= 0) {
      print_error("Invalid password\n");
      return;
    }
    new_pwd[strlen(new_pwd)-1] = '\0';

    memset(pwd, 0, BUF_SIZE);

    if (ftruncate(pwd_fd, 0) < 0) {
    //if (truncate(pwd_path, 0) < 0) {
      print_error("clear password file error\n");
      return;
    }
    //BUG FIX: lseek reset pwd_fd's location
    lseek(pwd_fd, 0, SEEK_SET);

    init_mcrypt();

    strncat(pwd, new_pwd, SAFE_SIZE);

    char* encoded = strdup(pwd);
    int len = strlen(encoded);
    mcrypt_generic(td, encoded, len);
    if (write(pwd_fd, encoded, len) < 0) {
      perror("write");
      print_error("Password saving failed\n");
      return;
    }
    print_success("\n* Password saved!\n");
    free(encoded);
  }

  void del_pwd() {
    //if (strlen(pwd) <= 0) return;
    memset(pwd, 0, BUF_SIZE);
    if (ftruncate(pwd_fd, 0) < 0) {
      print_error("clear password file error\n");
      return;
    }
    //BUG FIX: lseek reset pwd_fd's location
    lseek(pwd_fd, 0, SEEK_SET);
    print_normal("* Password deleted\n");
  }

  struct termios tattr;

  char* bash_like_str(char* str) {
    char* new_str = (char*) malloc(BUF_SIZE);
    memset(new_str, 0, BUF_SIZE);
    int i = 0;
    int len = strlen(str);
    int index = 0;
    for (i = 0; i < len; i++) {
      switch (str[i]) {
        case '$':
        case '&':
        case ' ':
          new_str[index] = '\\';
          index++;
          //fall through
        default:
          new_str[index] = str[i];
          index++;
          break;
      }
    }
    return new_str;
  }

  char* no_escape(char* str) {
    if (strlen(str) > 0) {
      if (str[0] == '\"' || str[0] == '\'') {
        return str;
      }
    }

    char* new_str = (char*) malloc(strlen(str) + 1);
    memset(new_str, 0, strlen(str) + 1);
    int i = 0;
    int len = strlen(str);
    int index = 0;
    for (i = 0; i < len; i++) {
      switch (str[i]) {
        case '\\':
          break;
        default:
          new_str[index] = str[i];
          index++;
          break;
      }
    }
    return new_str;
  }

  char* address_fix(char* orig_str) {
    char* str = no_escape(orig_str);
    // According to testing with bash, ~ is only replaceable with $HOME if ~ is EXACTLY the first char of path
    if (str[0] == '~' && (strlen(str) == 1 || str[1] == '/')) {
      char* new_str = (char*) malloc(BUF_SIZE);
      memset(new_str, 0, BUF_SIZE);
      strncat(new_str, homedir, SAFE_SIZE);
      strncat(&new_str[strlen(new_str)], &str[1], SAFE_SIZE-1);
      return new_str;
    } else {
      return str;
    }
  }

  //Invoke external expect script
  void auto_expect(int is_scp, char* addr_1, char* addr_2) {
    if (strcmp(user, "(null)") == 0) {
      print_warning("Please, set your username! (with command `usr <username>`)\n");
      return;
    }

    int rc = fork();
    if (rc == 0) {
      char expect_path[BUF_SIZE];
      memset(expect_path, 0, BUF_SIZE);
      strncat(expect_path, base_path, SAFE_SIZE - strlen(expect_path));
      strncat(expect_path, "/seas_expect", SAFE_SIZE - strlen(expect_path));
      if (is_scp == TRUE) {
        execlp(expect_path, "---", "1", pwd, addr_1, addr_2, NULL);
      } else {
        execlp(expect_path, "---", "0", pwd, addr_1, NULL);
      }
      perror("fork");
      exit(EXIT_FAILURE);
    } else {
      wait(NULL);
      return;
    }
  }
%}

%union {
  int number;
  char* string;
}

%token <number> USR HELP REPO
%token <number> SRV LOGIN
%token <number> INFO
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
| prog EXIT EOL {print_success("Goodbye!\n");
exit(EXIT_SUCCESS);
}
| prog HELP EOL {
  printf(BOLDBLACK"* Usage *\n\tuser <username>"RESET" : set default username\n\n\t"BOLDBLACK"server <srvnum>"RESET" : set default server number\n\n\t"BOLDBLACK"key"RESET" : save password\n\n\t"BOLDBLACK"auto <optional_srvnum>\n\t! <optional_srvnum>"RESET" : auto login with saved password\n\n\t"BOLDBLACK"login <optional_srvnum>"RESET" : login to server\n\n\t"BOLDBLACK"@ <server_path> => <local_path>\n\t<local_path> => @ <server_path>"RESET" : download and upload files/directory\n\n\t"BOLDBLACK"! @ <server_path> => <local_path>\n\t! <local_path> => @ <server_path>"RESET" : auto (using saved password) download and upload files/directory\n\n\t"BOLDBLACK"info"RESET" : check current settings\n\n\t"BOLDBLACK"exit"RESET" : exit program\n");
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
| login
| setsrv
| scp
| info
| bash
| key
| del
;

scp: LNXSRV NAME RIGHT_ARROW NAME {
  lite_scp_to_local(srv, $<string>2, address_fix($<string>4));
}
| LNXSRV NAME LEFT_ARROW NAME {
  lite_scp_to_server(srv, address_fix($<string>4), $<string>2);
}
| AUTO LNXSRV NAME RIGHT_ARROW NAME {
  char addr[BUF_SIZE];
  memset(addr, 0, BUF_SIZE);
  snprintf(addr, SAFE_SIZE, "%s@lnxsrv0%d.seas.ucla.edu:%s", user, srv, $<string>3);
  auto_expect(TRUE, addr, address_fix($<string>5));
}
| AUTO LNXSRV NAME LEFT_ARROW NAME {
  char addr[BUF_SIZE];
  memset(addr, 0, BUF_SIZE);
  snprintf(addr, SAFE_SIZE, "%s@lnxsrv0%d.seas.ucla.edu:%s", user, srv, $<string>3);
  auto_expect(TRUE, $<string>5, address_fix(addr));
}
| NAME LEFT_ARROW LNXSRV NAME {
  lite_scp_to_local(srv, $<string>4, address_fix($<string>1));
}
| NAME RIGHT_ARROW LNXSRV NAME {
  lite_scp_to_server(srv, address_fix($<string>1), $<string>4);
}
| AUTO NAME LEFT_ARROW LNXSRV NAME {
  char addr[BUF_SIZE];
  memset(addr, 0, BUF_SIZE);
  snprintf(addr, SAFE_SIZE, "%s@lnxsrv0%d.seas.ucla.edu:%s", user, srv, $<string>5);
  auto_expect(TRUE, addr, address_fix($<string>2));
}
| AUTO NAME RIGHT_ARROW LNXSRV NAME {
  char addr[BUF_SIZE];
  memset(addr, 0, BUF_SIZE);
  snprintf(addr, SAFE_SIZE, "%s@lnxsrv0%d.seas.ucla.edu:%s", user, srv, $<string>5);
  auto_expect(TRUE, address_fix($<string>2), addr);
}

;


login: LOGIN {
  lite_login(srv);
}
| LOGIN num %prec lower {
  lite_login($<number>2);
}
| AUTO LOGIN {
  char addr[BUF_SIZE];
  memset(addr, 0, BUF_SIZE);
  snprintf(addr, SAFE_SIZE, "%s@lnxsrv0%d.seas.ucla.edu", user, srv);
  auto_expect(FALSE, addr, NULL);
}
| AUTO {
  char addr[BUF_SIZE];
  memset(addr, 0, BUF_SIZE);
  snprintf(addr, SAFE_SIZE, "%s@lnxsrv0%d.seas.ucla.edu", user, srv);
  auto_expect(FALSE, addr, NULL);
}
| AUTO LOGIN num %prec lower {
  char addr[BUF_SIZE];
  memset(addr, 0, BUF_SIZE);
  snprintf(addr, SAFE_SIZE, "%s@lnxsrv0%d.seas.ucla.edu", user, $<number>3);
  auto_expect(FALSE, addr, NULL);
}
| AUTO num %prec lower {
  char addr[BUF_SIZE];
  memset(addr, 0, BUF_SIZE);
  snprintf(addr, SAFE_SIZE, "%s@lnxsrv0%d.seas.ucla.edu", user, $<number>2);
  auto_expect(FALSE, addr, NULL);
}
;

setsrv: srv num %prec lower {
  /*Truncate only when we reset the new name*/
  ftruncate(srv_fd, 0);
  /*Set fd to the start of file*/
  lseek(srv_fd, 0, SEEK_SET);
  srv = $<number>2;
  char* chr = (char*) malloc(sizeof(char) * 2);
  chr[0] = srv + '0';
  chr[1] = '\0';
  write(srv_fd, chr, strlen(chr));
  free(chr);
  printf("* Default login server has been changed to "BOLDGREEN"# %d\n"RESET, srv);
}
;
srv: SRV
;
num: NUM
;

setusr: usr name {
  /*Truncate only when we reset the new name*/
  ftruncate(usr_fd, 0);
  /*Set fd to the start of file*/
  lseek(usr_fd, 0, SEEK_SET);
  memset(user, 0, BUF_SIZE);
  strncpy(user, $<string>2, SAFE_SIZE);
  if (write(usr_fd, user, strlen(user)) < 0) {
    print_error("Save username failed\n");
  }
  printf("* Username has been changed to: "BOLDGREEN"%s\n"RESET, user);
}
;
usr: USR
;
name: NAME
;

info: INFO {
  if (strcmp(user, "(null)") == 0) {
    printf("* current username: "BOLDRED"%s"RESET"\n", user);
  } else {
	  printf("* current username: "BOLDGREEN"%s"RESET"\n", user);
  }
  printf("* current default server (1~9): "BOLDGREEN"%d"RESET"\n", srv);
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
  fprintf(stderr, "Enter password: ");
  tcgetattr(STDIN_FILENO, &tattr);
  tattr.c_lflag &= ~ECHO;
  tcsetattr(STDIN_FILENO, TCSANOW, &tattr);

  char buf[BUF_SIZE];
  memset(buf, 0, BUF_SIZE);
  if (read(STDIN_FILENO, buf, BUF_SIZE-1) <= 0) {
    write(STDERR_FILENO, "ERROR: Invalid password\n", BUF_SIZE);
  } else {
    //Automatically clears old pwd
    save_pwd(buf);
  }

  tattr.c_lflag |= ECHO;
  tcsetattr(STDIN_FILENO, TCSANOW, &tattr);
}
;

del: DEL KEY %prec higher {
  del_pwd();
}
| DEL USR %prec higher {
  if (ftruncate(usr_fd, 0) < 0) {
  //if (truncate(pwd_path, 0) < 0) {
    print_error("clear user file error\n");
  } else {
    memset(user, 0, BUF_SIZE);
    strncpy(user, "(null)", SAFE_SIZE);
    print_normal("* Saved username deleted\n");
  }
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
  cursorforward(18);
	printf(BOLDBLACK">> SEASHelper V1.0 (Release) <<"RESET"\n");
  cursorforward(17);
	printf("<< By kevinkassimo (github ID) >>\n");
  printf("===================================================================\n");
  cursorforward(5);
  if (strcmp(user, "(null)") == 0) {
    printf("* current username: "BOLDRED"%s"RESET"\n", user);
  } else {
	  printf("* current username: "BOLDGREEN"%s"RESET"\n", user);
  }
  cursorforward(5);
	printf("* current default server (1~9): "BOLDGREEN"%d"RESET"\n", srv);
  printf("===================================================================\n");
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

  //fix weird bug
  if (srv == 0) {
    srv = 1;
  }

  //Print version
  print_welcome();
  fprintf(stderr, "(Program Directory: %s)\n", base_path);

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
