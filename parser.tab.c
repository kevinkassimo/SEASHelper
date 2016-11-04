/* A Bison parser, made by GNU Bison 3.0.4.  */

/* Bison implementation for Yacc-like parsers in C

   Copyright (C) 1984, 1989-1990, 2000-2015 Free Software Foundation, Inc.

   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <http://www.gnu.org/licenses/>.  */

/* As a special exception, you may create a larger work that contains
   part or all of the Bison parser skeleton and distribute that work
   under terms of your choice, so long as that work isn't itself a
   parser generator using the skeleton or a modified version thereof
   as a parser skeleton.  Alternatively, if you modify or redistribute
   the parser skeleton itself, you may (at your option) remove this
   special exception, which will cause the skeleton and the resulting
   Bison output files to be licensed under the GNU General Public
   License without this special exception.

   This special exception was added by the Free Software Foundation in
   version 2.2 of Bison.  */

/* C LALR(1) parser skeleton written by Richard Stallman, by
   simplifying the original so-called "semantic" parser.  */

/* All symbols defined below should begin with yy or YY, to avoid
   infringing on user name space.  This should be done even for local
   variables, as they might otherwise be expanded by user macros.
   There are some unavoidable exceptions within include files to
   define necessary library symbols; they are noted "INFRINGES ON
   USER NAME SPACE" below.  */

/* Identify Bison output.  */
#define YYBISON 1

/* Bison version.  */
#define YYBISON_VERSION "3.0.4"

/* Skeleton name.  */
#define YYSKELETON_NAME "yacc.c"

/* Pure parsers.  */
#define YYPURE 0

/* Push parsers.  */
#define YYPUSH 0

/* Pull parsers.  */
#define YYPULL 1




/* Copy the first part of user declarations.  */
#line 1 "src/parser.y" /* yacc.c:339  */

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
    fprintf(stderr, BOLDRED"ERROR: %s\n"RESET, s);
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
    //printf("%s\n", base_path);
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
    if (strcmp(user, "(null)") == 0) {
      print_warning("Please, set your username! (with command `usr <username>`)\n");
      return;
    }
    char cmd[BUF_SIZE];
    memset(cmd, 0, BUF_SIZE);
    snprintf(cmd, SAFE_SIZE, "ssh %s@lnxsrv0%d.seas.ucla.edu", user, port);
    system(cmd);
  }

  void lite_scp_to_local(int port, char* from, char* to) {
    if (strcmp(user, "(null)") == 0) {
      print_warning("Please, set your username! (with command `usr <username>`)\n");
      return;
    }
    char cmd[BUF_SIZE];
    memset(cmd, 0, BUF_SIZE);
    snprintf(cmd, SAFE_SIZE, "scp -r %s@lnxsrv0%d.seas.ucla.edu:%s %s", user, port, from, to);
    system(cmd);
  }

  void lite_scp_to_server(int port, char* from, char* to) {
    if (strcmp(user, "(null)") == 0) {
      print_warning("Please, set your username! (with command `usr <username>`)\n");
      return;
    }
    char cmd[BUF_SIZE];
    memset(cmd, 0, BUF_SIZE);
    snprintf(cmd, SAFE_SIZE, "scp -r %s %s@lnxsrv0%d.seas.ucla.edu:%s", from, user, port, to);
    system(cmd);
  }

  char pwd_path[BUF_SIZE];
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
      perror("read");
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
      perror("read");
      return SEAS_FAIL;
    }
    if (ret == 0) {
      strncpy(user, "(null)", SAFE_SIZE - strlen(user));
    }
    return SEAS_SUCC;
  }
  int init_port() {

    char working_path[BUF_SIZE];
    memset(working_path, 0, BUF_SIZE);
    snprintf(working_path, SAFE_SIZE, "%s/data", base_path);
    hasdir(working_path, TRUE, S_IRWXU);

    //now have .seas_pwd file
    strncat(working_path, "/.seas_port", SAFE_SIZE - strlen(working_path));

    port_fd = getfilefd(working_path, TRUE, O_RDWR, 0777);
    int ret;
    char temp;
    if ((ret = read(port_fd, &temp, 1)) < 0) {
      perror("read");
      return SEAS_FAIL;
    }
    if (ret == 0) {
      port = 1;
    } else {
      port = atoi(&temp);
    }
    return SEAS_SUCC;
  }


  int init() {
    int ret = init_path() | init_mcrypt() | init_pwd() | init_usr() | init_port();
    if (ret != SEAS_SUCC) {
      print_error("Initialization failed\n");
    }
    return ret;
  }

  void ending() {
    close(pwd_fd);
    close(usr_fd);
    close(port_fd);
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
    if (strlen(pwd) <= 0) return;

    memset(pwd, 0, BUF_SIZE);
    if (ftruncate(pwd_fd, 0) < 0) {
    //if (truncate(pwd_path, 0) < 0) {
      print_error("clear password file error\n");
      return;
    }
    print_normal("* Password deleted\n");
  }

  struct termios tattr;

  void auto_expect(int is_scp, char* addr_1, char* addr_2) {
    int rc = fork();
    if (rc == 0) {
      char expect_path[BUF_SIZE];
      memset(expect_path, 0, BUF_SIZE);
      strncpy(expect_path, base_path, SAFE_SIZE);
      strncat(expect_path, "/seas_expect", SAFE_SIZE - strlen(expect_path));
      //fprintf(stderr, "%s", expect_path);
      printf("%s", pwd);
      if (is_scp == TRUE) {
        execl(expect_path, expect_path, "1", pwd, addr_1, addr_2);
      } else {
        execl(expect_path, expect_path, "0", pwd, addr_1, NULL);
      }
      perror("fork");
      exit(EXIT_FAILURE);
    } else {
      wait(NULL);
      return;
    }
  }

#line 472 "parser.tab.c" /* yacc.c:339  */

# ifndef YY_NULLPTR
#  if defined __cplusplus && 201103L <= __cplusplus
#   define YY_NULLPTR nullptr
#  else
#   define YY_NULLPTR 0
#  endif
# endif

/* Enabling verbose error messages.  */
#ifdef YYERROR_VERBOSE
# undef YYERROR_VERBOSE
# define YYERROR_VERBOSE 1
#else
# define YYERROR_VERBOSE 0
#endif

/* In a future release of Bison, this section will be replaced
   by #include "parser.tab.h".  */
#ifndef YY_YY_PARSER_TAB_H_INCLUDED
# define YY_YY_PARSER_TAB_H_INCLUDED
/* Debug traces.  */
#ifndef YYDEBUG
# define YYDEBUG 0
#endif
#if YYDEBUG
extern int yydebug;
#endif

/* Token type.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
  enum yytokentype
  {
    USR = 258,
    HELP = 259,
    REPO = 260,
    PORT = 261,
    LOGIN = 262,
    STAT = 263,
    LNXSRV = 264,
    DEL = 265,
    BASH = 266,
    AUTO = 267,
    LEFT_ARROW = 268,
    RIGHT_ARROW = 269,
    EOL = 270,
    NUM = 271,
    EXIT = 272,
    KEY = 273,
    NAME = 274,
    lowest = 275,
    lower = 276,
    higher = 277,
    cursor = 278
  };
#endif

/* Value type.  */
#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED

union YYSTYPE
{
#line 407 "src/parser.y" /* yacc.c:355  */

  int number;
  char* string;

#line 541 "parser.tab.c" /* yacc.c:355  */
};

typedef union YYSTYPE YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define YYSTYPE_IS_DECLARED 1
#endif


extern YYSTYPE yylval;

int yyparse (void);

#endif /* !YY_YY_PARSER_TAB_H_INCLUDED  */

/* Copy the second part of user declarations.  */

#line 558 "parser.tab.c" /* yacc.c:358  */

#ifdef short
# undef short
#endif

#ifdef YYTYPE_UINT8
typedef YYTYPE_UINT8 yytype_uint8;
#else
typedef unsigned char yytype_uint8;
#endif

#ifdef YYTYPE_INT8
typedef YYTYPE_INT8 yytype_int8;
#else
typedef signed char yytype_int8;
#endif

#ifdef YYTYPE_UINT16
typedef YYTYPE_UINT16 yytype_uint16;
#else
typedef unsigned short int yytype_uint16;
#endif

#ifdef YYTYPE_INT16
typedef YYTYPE_INT16 yytype_int16;
#else
typedef short int yytype_int16;
#endif

#ifndef YYSIZE_T
# ifdef __SIZE_TYPE__
#  define YYSIZE_T __SIZE_TYPE__
# elif defined size_t
#  define YYSIZE_T size_t
# elif ! defined YYSIZE_T
#  include <stddef.h> /* INFRINGES ON USER NAME SPACE */
#  define YYSIZE_T size_t
# else
#  define YYSIZE_T unsigned int
# endif
#endif

#define YYSIZE_MAXIMUM ((YYSIZE_T) -1)

#ifndef YY_
# if defined YYENABLE_NLS && YYENABLE_NLS
#  if ENABLE_NLS
#   include <libintl.h> /* INFRINGES ON USER NAME SPACE */
#   define YY_(Msgid) dgettext ("bison-runtime", Msgid)
#  endif
# endif
# ifndef YY_
#  define YY_(Msgid) Msgid
# endif
#endif

#ifndef YY_ATTRIBUTE
# if (defined __GNUC__                                               \
      && (2 < __GNUC__ || (__GNUC__ == 2 && 96 <= __GNUC_MINOR__)))  \
     || defined __SUNPRO_C && 0x5110 <= __SUNPRO_C
#  define YY_ATTRIBUTE(Spec) __attribute__(Spec)
# else
#  define YY_ATTRIBUTE(Spec) /* empty */
# endif
#endif

#ifndef YY_ATTRIBUTE_PURE
# define YY_ATTRIBUTE_PURE   YY_ATTRIBUTE ((__pure__))
#endif

#ifndef YY_ATTRIBUTE_UNUSED
# define YY_ATTRIBUTE_UNUSED YY_ATTRIBUTE ((__unused__))
#endif

#if !defined _Noreturn \
     && (!defined __STDC_VERSION__ || __STDC_VERSION__ < 201112)
# if defined _MSC_VER && 1200 <= _MSC_VER
#  define _Noreturn __declspec (noreturn)
# else
#  define _Noreturn YY_ATTRIBUTE ((__noreturn__))
# endif
#endif

/* Suppress unused-variable warnings by "using" E.  */
#if ! defined lint || defined __GNUC__
# define YYUSE(E) ((void) (E))
#else
# define YYUSE(E) /* empty */
#endif

#if defined __GNUC__ && 407 <= __GNUC__ * 100 + __GNUC_MINOR__
/* Suppress an incorrect diagnostic about yylval being uninitialized.  */
# define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN \
    _Pragma ("GCC diagnostic push") \
    _Pragma ("GCC diagnostic ignored \"-Wuninitialized\"")\
    _Pragma ("GCC diagnostic ignored \"-Wmaybe-uninitialized\"")
# define YY_IGNORE_MAYBE_UNINITIALIZED_END \
    _Pragma ("GCC diagnostic pop")
#else
# define YY_INITIAL_VALUE(Value) Value
#endif
#ifndef YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
# define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
# define YY_IGNORE_MAYBE_UNINITIALIZED_END
#endif
#ifndef YY_INITIAL_VALUE
# define YY_INITIAL_VALUE(Value) /* Nothing. */
#endif


#if ! defined yyoverflow || YYERROR_VERBOSE

/* The parser invokes alloca or malloc; define the necessary symbols.  */

# ifdef YYSTACK_USE_ALLOCA
#  if YYSTACK_USE_ALLOCA
#   ifdef __GNUC__
#    define YYSTACK_ALLOC __builtin_alloca
#   elif defined __BUILTIN_VA_ARG_INCR
#    include <alloca.h> /* INFRINGES ON USER NAME SPACE */
#   elif defined _AIX
#    define YYSTACK_ALLOC __alloca
#   elif defined _MSC_VER
#    include <malloc.h> /* INFRINGES ON USER NAME SPACE */
#    define alloca _alloca
#   else
#    define YYSTACK_ALLOC alloca
#    if ! defined _ALLOCA_H && ! defined EXIT_SUCCESS
#     include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
      /* Use EXIT_SUCCESS as a witness for stdlib.h.  */
#     ifndef EXIT_SUCCESS
#      define EXIT_SUCCESS 0
#     endif
#    endif
#   endif
#  endif
# endif

# ifdef YYSTACK_ALLOC
   /* Pacify GCC's 'empty if-body' warning.  */
#  define YYSTACK_FREE(Ptr) do { /* empty */; } while (0)
#  ifndef YYSTACK_ALLOC_MAXIMUM
    /* The OS might guarantee only one guard page at the bottom of the stack,
       and a page size can be as small as 4096 bytes.  So we cannot safely
       invoke alloca (N) if N exceeds 4096.  Use a slightly smaller number
       to allow for a few compiler-allocated temporary stack slots.  */
#   define YYSTACK_ALLOC_MAXIMUM 4032 /* reasonable circa 2006 */
#  endif
# else
#  define YYSTACK_ALLOC YYMALLOC
#  define YYSTACK_FREE YYFREE
#  ifndef YYSTACK_ALLOC_MAXIMUM
#   define YYSTACK_ALLOC_MAXIMUM YYSIZE_MAXIMUM
#  endif
#  if (defined __cplusplus && ! defined EXIT_SUCCESS \
       && ! ((defined YYMALLOC || defined malloc) \
             && (defined YYFREE || defined free)))
#   include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
#   ifndef EXIT_SUCCESS
#    define EXIT_SUCCESS 0
#   endif
#  endif
#  ifndef YYMALLOC
#   define YYMALLOC malloc
#   if ! defined malloc && ! defined EXIT_SUCCESS
void *malloc (YYSIZE_T); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
#  ifndef YYFREE
#   define YYFREE free
#   if ! defined free && ! defined EXIT_SUCCESS
void free (void *); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
# endif
#endif /* ! defined yyoverflow || YYERROR_VERBOSE */


#if (! defined yyoverflow \
     && (! defined __cplusplus \
         || (defined YYSTYPE_IS_TRIVIAL && YYSTYPE_IS_TRIVIAL)))

/* A type that is properly aligned for any stack member.  */
union yyalloc
{
  yytype_int16 yyss_alloc;
  YYSTYPE yyvs_alloc;
};

/* The size of the maximum gap between one aligned stack and the next.  */
# define YYSTACK_GAP_MAXIMUM (sizeof (union yyalloc) - 1)

/* The size of an array large to enough to hold all stacks, each with
   N elements.  */
# define YYSTACK_BYTES(N) \
     ((N) * (sizeof (yytype_int16) + sizeof (YYSTYPE)) \
      + YYSTACK_GAP_MAXIMUM)

# define YYCOPY_NEEDED 1

/* Relocate STACK from its old location to the new one.  The
   local variables YYSIZE and YYSTACKSIZE give the old and new number of
   elements in the stack, and YYPTR gives the new location of the
   stack.  Advance YYPTR to a properly aligned location for the next
   stack.  */
# define YYSTACK_RELOCATE(Stack_alloc, Stack)                           \
    do                                                                  \
      {                                                                 \
        YYSIZE_T yynewbytes;                                            \
        YYCOPY (&yyptr->Stack_alloc, Stack, yysize);                    \
        Stack = &yyptr->Stack_alloc;                                    \
        yynewbytes = yystacksize * sizeof (*Stack) + YYSTACK_GAP_MAXIMUM; \
        yyptr += yynewbytes / sizeof (*yyptr);                          \
      }                                                                 \
    while (0)

#endif

#if defined YYCOPY_NEEDED && YYCOPY_NEEDED
/* Copy COUNT objects from SRC to DST.  The source and destination do
   not overlap.  */
# ifndef YYCOPY
#  if defined __GNUC__ && 1 < __GNUC__
#   define YYCOPY(Dst, Src, Count) \
      __builtin_memcpy (Dst, Src, (Count) * sizeof (*(Src)))
#  else
#   define YYCOPY(Dst, Src, Count)              \
      do                                        \
        {                                       \
          YYSIZE_T yyi;                         \
          for (yyi = 0; yyi < (Count); yyi++)   \
            (Dst)[yyi] = (Src)[yyi];            \
        }                                       \
      while (0)
#  endif
# endif
#endif /* !YYCOPY_NEEDED */

/* YYFINAL -- State number of the termination state.  */
#define YYFINAL  2
/* YYLAST -- Last index in YYTABLE.  */
#define YYLAST   55

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  24
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  16
/* YYNRULES -- Number of rules.  */
#define YYNRULES  38
/* YYNSTATES -- Number of states.  */
#define YYNSTATES  64

/* YYTRANSLATE[YYX] -- Symbol number corresponding to YYX as returned
   by yylex, with out-of-bounds checking.  */
#define YYUNDEFTOK  2
#define YYMAXUTOK   278

#define YYTRANSLATE(YYX)                                                \
  ((unsigned int) (YYX) <= YYMAXUTOK ? yytranslate[YYX] : YYUNDEFTOK)

/* YYTRANSLATE[TOKEN-NUM] -- Symbol number corresponding to TOKEN-NUM
   as returned by yylex, without out-of-bounds checking.  */
static const yytype_uint8 yytranslate[] =
{
       0,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     1,     2,     3,     4,
       5,     6,     7,     8,     9,    10,    11,    12,    13,    14,
      15,    16,    17,    18,    19,    20,    21,    22,    23
};

#if YYDEBUG
  /* YYRLINE[YYN] -- Source line where rule number YYN was defined.  */
static const yytype_uint16 yyrline[] =
{
       0,   434,   434,   435,   436,   437,   440,   443,   452,   453,
     454,   455,   456,   457,   458,   459,   462,   465,   468,   474,
     480,   483,   486,   492,   502,   505,   508,   514,   522,   525,
     539,   541,   544,   557,   559,   562,   572,   583,   603
};
#endif

#if YYDEBUG || YYERROR_VERBOSE || 0
/* YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
   First, the terminals, then, starting at YYNTOKENS, nonterminals.  */
static const char *const yytname[] =
{
  "$end", "error", "$undefined", "USR", "HELP", "REPO", "PORT", "LOGIN",
  "STAT", "LNXSRV", "DEL", "BASH", "AUTO", "LEFT_ARROW", "RIGHT_ARROW",
  "EOL", "NUM", "EXIT", "KEY", "NAME", "lowest", "lower", "higher",
  "cursor", "$accept", "prog", "body", "scp", "server", "login", "setport",
  "port", "num", "setusr", "usr", "name", "stat", "bash", "key", "del", YY_NULLPTR
};
#endif

# ifdef YYPRINT
/* YYTOKNUM[NUM] -- (External) token number corresponding to the
   (internal) symbol number NUM (which must be that of a token).  */
static const yytype_uint16 yytoknum[] =
{
       0,   256,   257,   258,   259,   260,   261,   262,   263,   264,
     265,   266,   267,   268,   269,   270,   271,   272,   273,   274,
     275,   276,   277,   278
};
# endif

#define YYPACT_NINF -13

#define yypact_value_is_default(Yystate) \
  (!!((Yystate) == (-13)))

#define YYTABLE_NINF -1

#define yytable_value_is_error(Yytable_value) \
  0

  /* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
     STATE-NUM.  */
static const yytype_int8 yypact[] =
{
     -13,     0,   -13,   -13,    -2,     5,   -13,   -13,   -13,     6,
      11,   -13,     7,   -13,    15,   -13,   -12,    16,   -13,   -13,
      17,   -13,    17,   -13,    13,   -13,   -13,   -13,   -13,   -13,
     -13,     8,   -13,    18,    10,   -13,   -13,    25,    26,   -13,
     -13,   -13,   -13,   -13,   -13,    19,    20,    14,    27,    31,
      22,    23,   -13,   -13,    24,    28,    29,    30,   -13,   -13,
     -13,   -13,   -13,   -13
};

  /* YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
     Performed when YYTABLE does not specify something else to do.  Zero
     means the default is an error.  */
static const yytype_uint8 yydefact[] =
{
       2,     0,     1,    33,     0,     0,    30,    28,    35,     0,
       0,    36,    27,     3,     0,    37,     0,     0,    11,     9,
      24,    10,     0,     8,     0,    12,    13,    14,    15,     6,
       7,     0,    38,     0,     0,    26,     5,     0,     0,     4,
      31,    25,    29,    34,    32,     0,     0,     0,     0,     0,
       0,     0,    17,    16,     0,     0,     0,     0,    20,    21,
      19,    18,    22,    23
};

  /* YYPGOTO[NTERM-NUM].  */
static const yytype_int8 yypgoto[] =
{
     -13,   -13,   -13,   -13,   -13,    32,   -13,   -13,    33,   -13,
     -13,   -13,   -13,   -13,   -13,   -13
};

  /* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int8 yydefgoto[] =
{
      -1,     1,    17,    18,    19,    20,    21,    22,    41,    23,
      24,    44,    25,    26,    27,    28
};

  /* YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
     positive, shift that token.  If negative, reduce the rule whose
     number is the opposite.  If YYTABLE_NINF, syntax error.  */
static const yytype_uint8 yytable[] =
{
       2,    37,    38,     3,     4,     5,     6,     7,     8,     9,
      10,    11,    12,    29,     7,    13,    33,    14,    15,    16,
      30,    45,    46,    48,    49,    31,    34,    54,    55,    32,
      36,    39,    43,    40,    50,    51,    56,    47,    52,    53,
      57,    58,    59,    60,    35,     0,     0,    61,    62,    63,
       0,     0,     0,     0,     0,    42
};

static const yytype_int8 yycheck[] =
{
       0,    13,    14,     3,     4,     5,     6,     7,     8,     9,
      10,    11,    12,    15,     7,    15,     9,    17,    18,    19,
      15,    13,    14,    13,    14,    19,    19,    13,    14,    18,
      15,    15,    19,    16,     9,     9,     9,    19,    19,    19,
       9,    19,    19,    19,    12,    -1,    -1,    19,    19,    19,
      -1,    -1,    -1,    -1,    -1,    22
};

  /* YYSTOS[STATE-NUM] -- The (internal number of the) accessing
     symbol of state STATE-NUM.  */
static const yytype_uint8 yystos[] =
{
       0,    25,     0,     3,     4,     5,     6,     7,     8,     9,
      10,    11,    12,    15,    17,    18,    19,    26,    27,    28,
      29,    30,    31,    33,    34,    36,    37,    38,    39,    15,
      15,    19,    18,     9,    19,    29,    15,    13,    14,    15,
      16,    32,    32,    19,    35,    13,    14,    19,    13,    14,
       9,     9,    19,    19,    13,    14,     9,     9,    19,    19,
      19,    19,    19,    19
};

  /* YYR1[YYN] -- Symbol number of symbol that rule YYN derives.  */
static const yytype_uint8 yyr1[] =
{
       0,    24,    25,    25,    25,    25,    25,    25,    26,    26,
      26,    26,    26,    26,    26,    26,    27,    27,    27,    27,
      27,    27,    27,    27,    28,    28,    28,    28,    29,    30,
      31,    32,    33,    34,    35,    36,    37,    38,    39
};

  /* YYR2[YYN] -- Number of symbols on the right hand side of rule YYN.  */
static const yytype_uint8 yyr2[] =
{
       0,     2,     0,     2,     3,     3,     3,     3,     1,     1,
       1,     1,     1,     1,     1,     1,     4,     4,     5,     5,
       4,     4,     5,     5,     1,     2,     2,     1,     1,     2,
       1,     1,     2,     1,     1,     1,     1,     1,     2
};


#define yyerrok         (yyerrstatus = 0)
#define yyclearin       (yychar = YYEMPTY)
#define YYEMPTY         (-2)
#define YYEOF           0

#define YYACCEPT        goto yyacceptlab
#define YYABORT         goto yyabortlab
#define YYERROR         goto yyerrorlab


#define YYRECOVERING()  (!!yyerrstatus)

#define YYBACKUP(Token, Value)                                  \
do                                                              \
  if (yychar == YYEMPTY)                                        \
    {                                                           \
      yychar = (Token);                                         \
      yylval = (Value);                                         \
      YYPOPSTACK (yylen);                                       \
      yystate = *yyssp;                                         \
      goto yybackup;                                            \
    }                                                           \
  else                                                          \
    {                                                           \
      yyerror (YY_("syntax error: cannot back up")); \
      YYERROR;                                                  \
    }                                                           \
while (0)

/* Error token number */
#define YYTERROR        1
#define YYERRCODE       256



/* Enable debugging if requested.  */
#if YYDEBUG

# ifndef YYFPRINTF
#  include <stdio.h> /* INFRINGES ON USER NAME SPACE */
#  define YYFPRINTF fprintf
# endif

# define YYDPRINTF(Args)                        \
do {                                            \
  if (yydebug)                                  \
    YYFPRINTF Args;                             \
} while (0)

/* This macro is provided for backward compatibility. */
#ifndef YY_LOCATION_PRINT
# define YY_LOCATION_PRINT(File, Loc) ((void) 0)
#endif


# define YY_SYMBOL_PRINT(Title, Type, Value, Location)                    \
do {                                                                      \
  if (yydebug)                                                            \
    {                                                                     \
      YYFPRINTF (stderr, "%s ", Title);                                   \
      yy_symbol_print (stderr,                                            \
                  Type, Value); \
      YYFPRINTF (stderr, "\n");                                           \
    }                                                                     \
} while (0)


/*----------------------------------------.
| Print this symbol's value on YYOUTPUT.  |
`----------------------------------------*/

static void
yy_symbol_value_print (FILE *yyoutput, int yytype, YYSTYPE const * const yyvaluep)
{
  FILE *yyo = yyoutput;
  YYUSE (yyo);
  if (!yyvaluep)
    return;
# ifdef YYPRINT
  if (yytype < YYNTOKENS)
    YYPRINT (yyoutput, yytoknum[yytype], *yyvaluep);
# endif
  YYUSE (yytype);
}


/*--------------------------------.
| Print this symbol on YYOUTPUT.  |
`--------------------------------*/

static void
yy_symbol_print (FILE *yyoutput, int yytype, YYSTYPE const * const yyvaluep)
{
  YYFPRINTF (yyoutput, "%s %s (",
             yytype < YYNTOKENS ? "token" : "nterm", yytname[yytype]);

  yy_symbol_value_print (yyoutput, yytype, yyvaluep);
  YYFPRINTF (yyoutput, ")");
}

/*------------------------------------------------------------------.
| yy_stack_print -- Print the state stack from its BOTTOM up to its |
| TOP (included).                                                   |
`------------------------------------------------------------------*/

static void
yy_stack_print (yytype_int16 *yybottom, yytype_int16 *yytop)
{
  YYFPRINTF (stderr, "Stack now");
  for (; yybottom <= yytop; yybottom++)
    {
      int yybot = *yybottom;
      YYFPRINTF (stderr, " %d", yybot);
    }
  YYFPRINTF (stderr, "\n");
}

# define YY_STACK_PRINT(Bottom, Top)                            \
do {                                                            \
  if (yydebug)                                                  \
    yy_stack_print ((Bottom), (Top));                           \
} while (0)


/*------------------------------------------------.
| Report that the YYRULE is going to be reduced.  |
`------------------------------------------------*/

static void
yy_reduce_print (yytype_int16 *yyssp, YYSTYPE *yyvsp, int yyrule)
{
  unsigned long int yylno = yyrline[yyrule];
  int yynrhs = yyr2[yyrule];
  int yyi;
  YYFPRINTF (stderr, "Reducing stack by rule %d (line %lu):\n",
             yyrule - 1, yylno);
  /* The symbols being reduced.  */
  for (yyi = 0; yyi < yynrhs; yyi++)
    {
      YYFPRINTF (stderr, "   $%d = ", yyi + 1);
      yy_symbol_print (stderr,
                       yystos[yyssp[yyi + 1 - yynrhs]],
                       &(yyvsp[(yyi + 1) - (yynrhs)])
                                              );
      YYFPRINTF (stderr, "\n");
    }
}

# define YY_REDUCE_PRINT(Rule)          \
do {                                    \
  if (yydebug)                          \
    yy_reduce_print (yyssp, yyvsp, Rule); \
} while (0)

/* Nonzero means print parse trace.  It is left uninitialized so that
   multiple parsers can coexist.  */
int yydebug;
#else /* !YYDEBUG */
# define YYDPRINTF(Args)
# define YY_SYMBOL_PRINT(Title, Type, Value, Location)
# define YY_STACK_PRINT(Bottom, Top)
# define YY_REDUCE_PRINT(Rule)
#endif /* !YYDEBUG */


/* YYINITDEPTH -- initial size of the parser's stacks.  */
#ifndef YYINITDEPTH
# define YYINITDEPTH 200
#endif

/* YYMAXDEPTH -- maximum size the stacks can grow to (effective only
   if the built-in stack extension method is used).

   Do not make this value too large; the results are undefined if
   YYSTACK_ALLOC_MAXIMUM < YYSTACK_BYTES (YYMAXDEPTH)
   evaluated with infinite-precision integer arithmetic.  */

#ifndef YYMAXDEPTH
# define YYMAXDEPTH 10000
#endif


#if YYERROR_VERBOSE

# ifndef yystrlen
#  if defined __GLIBC__ && defined _STRING_H
#   define yystrlen strlen
#  else
/* Return the length of YYSTR.  */
static YYSIZE_T
yystrlen (const char *yystr)
{
  YYSIZE_T yylen;
  for (yylen = 0; yystr[yylen]; yylen++)
    continue;
  return yylen;
}
#  endif
# endif

# ifndef yystpcpy
#  if defined __GLIBC__ && defined _STRING_H && defined _GNU_SOURCE
#   define yystpcpy stpcpy
#  else
/* Copy YYSRC to YYDEST, returning the address of the terminating '\0' in
   YYDEST.  */
static char *
yystpcpy (char *yydest, const char *yysrc)
{
  char *yyd = yydest;
  const char *yys = yysrc;

  while ((*yyd++ = *yys++) != '\0')
    continue;

  return yyd - 1;
}
#  endif
# endif

# ifndef yytnamerr
/* Copy to YYRES the contents of YYSTR after stripping away unnecessary
   quotes and backslashes, so that it's suitable for yyerror.  The
   heuristic is that double-quoting is unnecessary unless the string
   contains an apostrophe, a comma, or backslash (other than
   backslash-backslash).  YYSTR is taken from yytname.  If YYRES is
   null, do not copy; instead, return the length of what the result
   would have been.  */
static YYSIZE_T
yytnamerr (char *yyres, const char *yystr)
{
  if (*yystr == '"')
    {
      YYSIZE_T yyn = 0;
      char const *yyp = yystr;

      for (;;)
        switch (*++yyp)
          {
          case '\'':
          case ',':
            goto do_not_strip_quotes;

          case '\\':
            if (*++yyp != '\\')
              goto do_not_strip_quotes;
            /* Fall through.  */
          default:
            if (yyres)
              yyres[yyn] = *yyp;
            yyn++;
            break;

          case '"':
            if (yyres)
              yyres[yyn] = '\0';
            return yyn;
          }
    do_not_strip_quotes: ;
    }

  if (! yyres)
    return yystrlen (yystr);

  return yystpcpy (yyres, yystr) - yyres;
}
# endif

/* Copy into *YYMSG, which is of size *YYMSG_ALLOC, an error message
   about the unexpected token YYTOKEN for the state stack whose top is
   YYSSP.

   Return 0 if *YYMSG was successfully written.  Return 1 if *YYMSG is
   not large enough to hold the message.  In that case, also set
   *YYMSG_ALLOC to the required number of bytes.  Return 2 if the
   required number of bytes is too large to store.  */
static int
yysyntax_error (YYSIZE_T *yymsg_alloc, char **yymsg,
                yytype_int16 *yyssp, int yytoken)
{
  YYSIZE_T yysize0 = yytnamerr (YY_NULLPTR, yytname[yytoken]);
  YYSIZE_T yysize = yysize0;
  enum { YYERROR_VERBOSE_ARGS_MAXIMUM = 5 };
  /* Internationalized format string. */
  const char *yyformat = YY_NULLPTR;
  /* Arguments of yyformat. */
  char const *yyarg[YYERROR_VERBOSE_ARGS_MAXIMUM];
  /* Number of reported tokens (one for the "unexpected", one per
     "expected"). */
  int yycount = 0;

  /* There are many possibilities here to consider:
     - If this state is a consistent state with a default action, then
       the only way this function was invoked is if the default action
       is an error action.  In that case, don't check for expected
       tokens because there are none.
     - The only way there can be no lookahead present (in yychar) is if
       this state is a consistent state with a default action.  Thus,
       detecting the absence of a lookahead is sufficient to determine
       that there is no unexpected or expected token to report.  In that
       case, just report a simple "syntax error".
     - Don't assume there isn't a lookahead just because this state is a
       consistent state with a default action.  There might have been a
       previous inconsistent state, consistent state with a non-default
       action, or user semantic action that manipulated yychar.
     - Of course, the expected token list depends on states to have
       correct lookahead information, and it depends on the parser not
       to perform extra reductions after fetching a lookahead from the
       scanner and before detecting a syntax error.  Thus, state merging
       (from LALR or IELR) and default reductions corrupt the expected
       token list.  However, the list is correct for canonical LR with
       one exception: it will still contain any token that will not be
       accepted due to an error action in a later state.
  */
  if (yytoken != YYEMPTY)
    {
      int yyn = yypact[*yyssp];
      yyarg[yycount++] = yytname[yytoken];
      if (!yypact_value_is_default (yyn))
        {
          /* Start YYX at -YYN if negative to avoid negative indexes in
             YYCHECK.  In other words, skip the first -YYN actions for
             this state because they are default actions.  */
          int yyxbegin = yyn < 0 ? -yyn : 0;
          /* Stay within bounds of both yycheck and yytname.  */
          int yychecklim = YYLAST - yyn + 1;
          int yyxend = yychecklim < YYNTOKENS ? yychecklim : YYNTOKENS;
          int yyx;

          for (yyx = yyxbegin; yyx < yyxend; ++yyx)
            if (yycheck[yyx + yyn] == yyx && yyx != YYTERROR
                && !yytable_value_is_error (yytable[yyx + yyn]))
              {
                if (yycount == YYERROR_VERBOSE_ARGS_MAXIMUM)
                  {
                    yycount = 1;
                    yysize = yysize0;
                    break;
                  }
                yyarg[yycount++] = yytname[yyx];
                {
                  YYSIZE_T yysize1 = yysize + yytnamerr (YY_NULLPTR, yytname[yyx]);
                  if (! (yysize <= yysize1
                         && yysize1 <= YYSTACK_ALLOC_MAXIMUM))
                    return 2;
                  yysize = yysize1;
                }
              }
        }
    }

  switch (yycount)
    {
# define YYCASE_(N, S)                      \
      case N:                               \
        yyformat = S;                       \
      break
      YYCASE_(0, YY_("syntax error"));
      YYCASE_(1, YY_("syntax error, unexpected %s"));
      YYCASE_(2, YY_("syntax error, unexpected %s, expecting %s"));
      YYCASE_(3, YY_("syntax error, unexpected %s, expecting %s or %s"));
      YYCASE_(4, YY_("syntax error, unexpected %s, expecting %s or %s or %s"));
      YYCASE_(5, YY_("syntax error, unexpected %s, expecting %s or %s or %s or %s"));
# undef YYCASE_
    }

  {
    YYSIZE_T yysize1 = yysize + yystrlen (yyformat);
    if (! (yysize <= yysize1 && yysize1 <= YYSTACK_ALLOC_MAXIMUM))
      return 2;
    yysize = yysize1;
  }

  if (*yymsg_alloc < yysize)
    {
      *yymsg_alloc = 2 * yysize;
      if (! (yysize <= *yymsg_alloc
             && *yymsg_alloc <= YYSTACK_ALLOC_MAXIMUM))
        *yymsg_alloc = YYSTACK_ALLOC_MAXIMUM;
      return 1;
    }

  /* Avoid sprintf, as that infringes on the user's name space.
     Don't have undefined behavior even if the translation
     produced a string with the wrong number of "%s"s.  */
  {
    char *yyp = *yymsg;
    int yyi = 0;
    while ((*yyp = *yyformat) != '\0')
      if (*yyp == '%' && yyformat[1] == 's' && yyi < yycount)
        {
          yyp += yytnamerr (yyp, yyarg[yyi++]);
          yyformat += 2;
        }
      else
        {
          yyp++;
          yyformat++;
        }
  }
  return 0;
}
#endif /* YYERROR_VERBOSE */

/*-----------------------------------------------.
| Release the memory associated to this symbol.  |
`-----------------------------------------------*/

static void
yydestruct (const char *yymsg, int yytype, YYSTYPE *yyvaluep)
{
  YYUSE (yyvaluep);
  if (!yymsg)
    yymsg = "Deleting";
  YY_SYMBOL_PRINT (yymsg, yytype, yyvaluep, yylocationp);

  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  YYUSE (yytype);
  YY_IGNORE_MAYBE_UNINITIALIZED_END
}




/* The lookahead symbol.  */
int yychar;

/* The semantic value of the lookahead symbol.  */
YYSTYPE yylval;
/* Number of syntax errors so far.  */
int yynerrs;


/*----------.
| yyparse.  |
`----------*/

int
yyparse (void)
{
    int yystate;
    /* Number of tokens to shift before error messages enabled.  */
    int yyerrstatus;

    /* The stacks and their tools:
       'yyss': related to states.
       'yyvs': related to semantic values.

       Refer to the stacks through separate pointers, to allow yyoverflow
       to reallocate them elsewhere.  */

    /* The state stack.  */
    yytype_int16 yyssa[YYINITDEPTH];
    yytype_int16 *yyss;
    yytype_int16 *yyssp;

    /* The semantic value stack.  */
    YYSTYPE yyvsa[YYINITDEPTH];
    YYSTYPE *yyvs;
    YYSTYPE *yyvsp;

    YYSIZE_T yystacksize;

  int yyn;
  int yyresult;
  /* Lookahead token as an internal (translated) token number.  */
  int yytoken = 0;
  /* The variables used to return semantic value and location from the
     action routines.  */
  YYSTYPE yyval;

#if YYERROR_VERBOSE
  /* Buffer for error messages, and its allocated size.  */
  char yymsgbuf[128];
  char *yymsg = yymsgbuf;
  YYSIZE_T yymsg_alloc = sizeof yymsgbuf;
#endif

#define YYPOPSTACK(N)   (yyvsp -= (N), yyssp -= (N))

  /* The number of symbols on the RHS of the reduced rule.
     Keep to zero when no symbol should be popped.  */
  int yylen = 0;

  yyssp = yyss = yyssa;
  yyvsp = yyvs = yyvsa;
  yystacksize = YYINITDEPTH;

  YYDPRINTF ((stderr, "Starting parse\n"));

  yystate = 0;
  yyerrstatus = 0;
  yynerrs = 0;
  yychar = YYEMPTY; /* Cause a token to be read.  */
  goto yysetstate;

/*------------------------------------------------------------.
| yynewstate -- Push a new state, which is found in yystate.  |
`------------------------------------------------------------*/
 yynewstate:
  /* In all cases, when you get here, the value and location stacks
     have just been pushed.  So pushing a state here evens the stacks.  */
  yyssp++;

 yysetstate:
  *yyssp = yystate;

  if (yyss + yystacksize - 1 <= yyssp)
    {
      /* Get the current used size of the three stacks, in elements.  */
      YYSIZE_T yysize = yyssp - yyss + 1;

#ifdef yyoverflow
      {
        /* Give user a chance to reallocate the stack.  Use copies of
           these so that the &'s don't force the real ones into
           memory.  */
        YYSTYPE *yyvs1 = yyvs;
        yytype_int16 *yyss1 = yyss;

        /* Each stack pointer address is followed by the size of the
           data in use in that stack, in bytes.  This used to be a
           conditional around just the two extra args, but that might
           be undefined if yyoverflow is a macro.  */
        yyoverflow (YY_("memory exhausted"),
                    &yyss1, yysize * sizeof (*yyssp),
                    &yyvs1, yysize * sizeof (*yyvsp),
                    &yystacksize);

        yyss = yyss1;
        yyvs = yyvs1;
      }
#else /* no yyoverflow */
# ifndef YYSTACK_RELOCATE
      goto yyexhaustedlab;
# else
      /* Extend the stack our own way.  */
      if (YYMAXDEPTH <= yystacksize)
        goto yyexhaustedlab;
      yystacksize *= 2;
      if (YYMAXDEPTH < yystacksize)
        yystacksize = YYMAXDEPTH;

      {
        yytype_int16 *yyss1 = yyss;
        union yyalloc *yyptr =
          (union yyalloc *) YYSTACK_ALLOC (YYSTACK_BYTES (yystacksize));
        if (! yyptr)
          goto yyexhaustedlab;
        YYSTACK_RELOCATE (yyss_alloc, yyss);
        YYSTACK_RELOCATE (yyvs_alloc, yyvs);
#  undef YYSTACK_RELOCATE
        if (yyss1 != yyssa)
          YYSTACK_FREE (yyss1);
      }
# endif
#endif /* no yyoverflow */

      yyssp = yyss + yysize - 1;
      yyvsp = yyvs + yysize - 1;

      YYDPRINTF ((stderr, "Stack size increased to %lu\n",
                  (unsigned long int) yystacksize));

      if (yyss + yystacksize - 1 <= yyssp)
        YYABORT;
    }

  YYDPRINTF ((stderr, "Entering state %d\n", yystate));

  if (yystate == YYFINAL)
    YYACCEPT;

  goto yybackup;

/*-----------.
| yybackup.  |
`-----------*/
yybackup:

  /* Do appropriate processing given the current state.  Read a
     lookahead token if we need one and don't already have one.  */

  /* First try to decide what to do without reference to lookahead token.  */
  yyn = yypact[yystate];
  if (yypact_value_is_default (yyn))
    goto yydefault;

  /* Not known => get a lookahead token if don't already have one.  */

  /* YYCHAR is either YYEMPTY or YYEOF or a valid lookahead symbol.  */
  if (yychar == YYEMPTY)
    {
      YYDPRINTF ((stderr, "Reading a token: "));
      yychar = yylex ();
    }

  if (yychar <= YYEOF)
    {
      yychar = yytoken = YYEOF;
      YYDPRINTF ((stderr, "Now at end of input.\n"));
    }
  else
    {
      yytoken = YYTRANSLATE (yychar);
      YY_SYMBOL_PRINT ("Next token is", yytoken, &yylval, &yylloc);
    }

  /* If the proper action on seeing token YYTOKEN is to reduce or to
     detect an error, take that action.  */
  yyn += yytoken;
  if (yyn < 0 || YYLAST < yyn || yycheck[yyn] != yytoken)
    goto yydefault;
  yyn = yytable[yyn];
  if (yyn <= 0)
    {
      if (yytable_value_is_error (yyn))
        goto yyerrlab;
      yyn = -yyn;
      goto yyreduce;
    }

  /* Count tokens shifted since error; after three, turn off error
     status.  */
  if (yyerrstatus)
    yyerrstatus--;

  /* Shift the lookahead token.  */
  YY_SYMBOL_PRINT ("Shifting", yytoken, &yylval, &yylloc);

  /* Discard the shifted token.  */
  yychar = YYEMPTY;

  yystate = yyn;
  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  *++yyvsp = yylval;
  YY_IGNORE_MAYBE_UNINITIALIZED_END

  goto yynewstate;


/*-----------------------------------------------------------.
| yydefault -- do the default action for the current state.  |
`-----------------------------------------------------------*/
yydefault:
  yyn = yydefact[yystate];
  if (yyn == 0)
    goto yyerrlab;
  goto yyreduce;


/*-----------------------------.
| yyreduce -- Do a reduction.  |
`-----------------------------*/
yyreduce:
  /* yyn is the number of a rule to reduce with.  */
  yylen = yyr2[yyn];

  /* If YYLEN is nonzero, implement the default value of the action:
     '$$ = $1'.

     Otherwise, the following line sets YYVAL to garbage.
     This behavior is undocumented and Bison
     users should not rely upon it.  Assigning to YYVAL
     unconditionally makes the parser a bit smaller, and it avoids a
     GCC warning that YYVAL may be used uninitialized.  */
  yyval = yyvsp[1-yylen];


  YY_REDUCE_PRINT (yyn);
  switch (yyn)
    {
        case 2:
#line 434 "src/parser.y" /* yacc.c:1661  */
    {}
#line 1669 "parser.tab.c" /* yacc.c:1661  */
    break;

  case 3:
#line 435 "src/parser.y" /* yacc.c:1661  */
    {/*printf("> ");*/}
#line 1675 "parser.tab.c" /* yacc.c:1661  */
    break;

  case 4:
#line 436 "src/parser.y" /* yacc.c:1661  */
    {/*printf("> ");*/}
#line 1681 "parser.tab.c" /* yacc.c:1661  */
    break;

  case 5:
#line 437 "src/parser.y" /* yacc.c:1661  */
    {print_success("Goodbye!\n");
exit(EXIT_SUCCESS);
}
#line 1689 "parser.tab.c" /* yacc.c:1661  */
    break;

  case 6:
#line 440 "src/parser.y" /* yacc.c:1661  */
    {
  printf("* Usage *\n\tuser *username* : set default username\n\tport *portnum* : set default server number\n\tlogin *optional_portnum* : login to server\n\t@ *server_path* => *local_path\n\t*local_path* => @ *server_path* : download and upload files/directory\n\texit : exit program\n");
}
#line 1697 "parser.tab.c" /* yacc.c:1661  */
    break;

  case 7:
#line 443 "src/parser.y" /* yacc.c:1661  */
    {
#ifdef __APPLE__
system("open https://github.com/kevinkassimo/SEASHelper");
#elif __linux__
system("xdg-open https://github.com/kevinkassimo/SEASHelper");
#endif
}
#line 1709 "parser.tab.c" /* yacc.c:1661  */
    break;

  case 16:
#line 462 "src/parser.y" /* yacc.c:1661  */
    {
  lite_scp_to_local(port, (yyvsp[-2].string), (yyvsp[0].string));
}
#line 1717 "parser.tab.c" /* yacc.c:1661  */
    break;

  case 17:
#line 465 "src/parser.y" /* yacc.c:1661  */
    {
  lite_scp_to_server(port, (yyvsp[0].string), (yyvsp[-2].string));
}
#line 1725 "parser.tab.c" /* yacc.c:1661  */
    break;

  case 18:
#line 468 "src/parser.y" /* yacc.c:1661  */
    {
  char addr[BUF_SIZE];
  memset(addr, 0, BUF_SIZE);
  snprintf(addr, SAFE_SIZE, "%s@lnxsrv0%d.seas.ucla.edu:%s", user, port, (yyvsp[-2].string));
  auto_expect(TRUE, addr, (yyvsp[0].string));
}
#line 1736 "parser.tab.c" /* yacc.c:1661  */
    break;

  case 19:
#line 474 "src/parser.y" /* yacc.c:1661  */
    {
  char addr[BUF_SIZE];
  memset(addr, 0, BUF_SIZE);
  snprintf(addr, SAFE_SIZE, "%s@lnxsrv0%d.seas.ucla.edu:%s", user, port, (yyvsp[-2].string));
  auto_expect(TRUE, (yyvsp[0].string), addr);
}
#line 1747 "parser.tab.c" /* yacc.c:1661  */
    break;

  case 20:
#line 480 "src/parser.y" /* yacc.c:1661  */
    {
  lite_scp_to_local(port, (yyvsp[0].string), (yyvsp[-3].string));
}
#line 1755 "parser.tab.c" /* yacc.c:1661  */
    break;

  case 21:
#line 483 "src/parser.y" /* yacc.c:1661  */
    {
  lite_scp_to_server(port, (yyvsp[-3].string), (yyvsp[0].string));
}
#line 1763 "parser.tab.c" /* yacc.c:1661  */
    break;

  case 22:
#line 486 "src/parser.y" /* yacc.c:1661  */
    {
  char addr[BUF_SIZE];
  memset(addr, 0, BUF_SIZE);
  snprintf(addr, SAFE_SIZE, "%s@lnxsrv0%d.seas.ucla.edu:%s", user, port, (yyvsp[0].string));
  auto_expect(TRUE, addr, (yyvsp[-3].string));
}
#line 1774 "parser.tab.c" /* yacc.c:1661  */
    break;

  case 23:
#line 492 "src/parser.y" /* yacc.c:1661  */
    {
  char addr[BUF_SIZE];
  memset(addr, 0, BUF_SIZE);
  snprintf(addr, SAFE_SIZE, "%s@lnxsrv0%d.seas.ucla.edu:%s", user, port, (yyvsp[0].string));
  auto_expect(TRUE, (yyvsp[-3].string), addr);
}
#line 1785 "parser.tab.c" /* yacc.c:1661  */
    break;

  case 24:
#line 502 "src/parser.y" /* yacc.c:1661  */
    {
  lite_login(port);
}
#line 1793 "parser.tab.c" /* yacc.c:1661  */
    break;

  case 25:
#line 505 "src/parser.y" /* yacc.c:1661  */
    {
  lite_login((yyvsp[0].number));
}
#line 1801 "parser.tab.c" /* yacc.c:1661  */
    break;

  case 26:
#line 508 "src/parser.y" /* yacc.c:1661  */
    {
  char addr[BUF_SIZE];
  memset(addr, 0, BUF_SIZE);
  snprintf(addr, SAFE_SIZE, "%s@lnxsrv0%d.seas.ucla.edu", user, port);
  auto_expect(FALSE, addr, NULL);
}
#line 1812 "parser.tab.c" /* yacc.c:1661  */
    break;

  case 27:
#line 514 "src/parser.y" /* yacc.c:1661  */
    {
  char addr[BUF_SIZE];
  memset(addr, 0, BUF_SIZE);
  snprintf(addr, SAFE_SIZE, "%s@lnxsrv0%d.seas.ucla.edu", user, port);
  auto_expect(FALSE, addr, NULL);
}
#line 1823 "parser.tab.c" /* yacc.c:1661  */
    break;

  case 29:
#line 525 "src/parser.y" /* yacc.c:1661  */
    {
  /*Truncate only when we reset the new name*/
  ftruncate(port_fd, 0);
  /*Set fd to the start of file*/
  lseek(port_fd, 0, SEEK_SET);
  port = (yyvsp[0].number);
  char* chr = (char*) malloc(sizeof(char) * 2);
  chr[0] = port + '0';
  chr[1] = '\0';
  write(port_fd, chr, strlen(chr));
  free(chr);
  printf("* Default login server has been changed into "BOLDGREEN"# %d\n"RESET, port);
}
#line 1841 "parser.tab.c" /* yacc.c:1661  */
    break;

  case 32:
#line 544 "src/parser.y" /* yacc.c:1661  */
    {
  /*Truncate only when we reset the new name*/
  ftruncate(usr_fd, 0);
  /*Set fd to the start of file*/
  lseek(usr_fd, 0, SEEK_SET);
  memset(user, 0, BUF_SIZE);
  strncpy(user, (yyvsp[0].string), SAFE_SIZE);
  if (write(usr_fd, user, strlen(user)) < 0) {
    print_error("Save username failed\n");
  }
  printf("* Username has been changed into: "BOLDGREEN"%s\n"RESET, user);
}
#line 1858 "parser.tab.c" /* yacc.c:1661  */
    break;

  case 35:
#line 562 "src/parser.y" /* yacc.c:1661  */
    {
  if (strcmp(user, "(null)") == 0) {
    printf("* current username: "BOLDRED"%s"RESET"\n", user);
  } else {
	  printf("* current username: "BOLDGREEN"%s"RESET"\n", user);
  }
  printf("* current default port (1~9): "BOLDGREEN"%d"RESET"\n", port);
}
#line 1871 "parser.tab.c" /* yacc.c:1661  */
    break;

  case 36:
#line 572 "src/parser.y" /* yacc.c:1661  */
    {
  rc = fork();
  if (rc == 0) {
    execl("/bin/bash", "/bin/bash", NULL);
    perror("exec bash");
  } else {
    wait(NULL);
  }
}
#line 1885 "parser.tab.c" /* yacc.c:1661  */
    break;

  case 37:
#line 583 "src/parser.y" /* yacc.c:1661  */
    {
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
#line 1908 "parser.tab.c" /* yacc.c:1661  */
    break;

  case 38:
#line 603 "src/parser.y" /* yacc.c:1661  */
    {
  del_pwd();
}
#line 1916 "parser.tab.c" /* yacc.c:1661  */
    break;


#line 1920 "parser.tab.c" /* yacc.c:1661  */
      default: break;
    }
  /* User semantic actions sometimes alter yychar, and that requires
     that yytoken be updated with the new translation.  We take the
     approach of translating immediately before every use of yytoken.
     One alternative is translating here after every semantic action,
     but that translation would be missed if the semantic action invokes
     YYABORT, YYACCEPT, or YYERROR immediately after altering yychar or
     if it invokes YYBACKUP.  In the case of YYABORT or YYACCEPT, an
     incorrect destructor might then be invoked immediately.  In the
     case of YYERROR or YYBACKUP, subsequent parser actions might lead
     to an incorrect destructor call or verbose syntax error message
     before the lookahead is translated.  */
  YY_SYMBOL_PRINT ("-> $$ =", yyr1[yyn], &yyval, &yyloc);

  YYPOPSTACK (yylen);
  yylen = 0;
  YY_STACK_PRINT (yyss, yyssp);

  *++yyvsp = yyval;

  /* Now 'shift' the result of the reduction.  Determine what state
     that goes to, based on the state we popped back to and the rule
     number reduced by.  */

  yyn = yyr1[yyn];

  yystate = yypgoto[yyn - YYNTOKENS] + *yyssp;
  if (0 <= yystate && yystate <= YYLAST && yycheck[yystate] == *yyssp)
    yystate = yytable[yystate];
  else
    yystate = yydefgoto[yyn - YYNTOKENS];

  goto yynewstate;


/*--------------------------------------.
| yyerrlab -- here on detecting error.  |
`--------------------------------------*/
yyerrlab:
  /* Make sure we have latest lookahead translation.  See comments at
     user semantic actions for why this is necessary.  */
  yytoken = yychar == YYEMPTY ? YYEMPTY : YYTRANSLATE (yychar);

  /* If not already recovering from an error, report this error.  */
  if (!yyerrstatus)
    {
      ++yynerrs;
#if ! YYERROR_VERBOSE
      yyerror (YY_("syntax error"));
#else
# define YYSYNTAX_ERROR yysyntax_error (&yymsg_alloc, &yymsg, \
                                        yyssp, yytoken)
      {
        char const *yymsgp = YY_("syntax error");
        int yysyntax_error_status;
        yysyntax_error_status = YYSYNTAX_ERROR;
        if (yysyntax_error_status == 0)
          yymsgp = yymsg;
        else if (yysyntax_error_status == 1)
          {
            if (yymsg != yymsgbuf)
              YYSTACK_FREE (yymsg);
            yymsg = (char *) YYSTACK_ALLOC (yymsg_alloc);
            if (!yymsg)
              {
                yymsg = yymsgbuf;
                yymsg_alloc = sizeof yymsgbuf;
                yysyntax_error_status = 2;
              }
            else
              {
                yysyntax_error_status = YYSYNTAX_ERROR;
                yymsgp = yymsg;
              }
          }
        yyerror (yymsgp);
        if (yysyntax_error_status == 2)
          goto yyexhaustedlab;
      }
# undef YYSYNTAX_ERROR
#endif
    }



  if (yyerrstatus == 3)
    {
      /* If just tried and failed to reuse lookahead token after an
         error, discard it.  */

      if (yychar <= YYEOF)
        {
          /* Return failure if at end of input.  */
          if (yychar == YYEOF)
            YYABORT;
        }
      else
        {
          yydestruct ("Error: discarding",
                      yytoken, &yylval);
          yychar = YYEMPTY;
        }
    }

  /* Else will try to reuse lookahead token after shifting the error
     token.  */
  goto yyerrlab1;


/*---------------------------------------------------.
| yyerrorlab -- error raised explicitly by YYERROR.  |
`---------------------------------------------------*/
yyerrorlab:

  /* Pacify compilers like GCC when the user code never invokes
     YYERROR and the label yyerrorlab therefore never appears in user
     code.  */
  if (/*CONSTCOND*/ 0)
     goto yyerrorlab;

  /* Do not reclaim the symbols of the rule whose action triggered
     this YYERROR.  */
  YYPOPSTACK (yylen);
  yylen = 0;
  YY_STACK_PRINT (yyss, yyssp);
  yystate = *yyssp;
  goto yyerrlab1;


/*-------------------------------------------------------------.
| yyerrlab1 -- common code for both syntax error and YYERROR.  |
`-------------------------------------------------------------*/
yyerrlab1:
  yyerrstatus = 3;      /* Each real token shifted decrements this.  */

  for (;;)
    {
      yyn = yypact[yystate];
      if (!yypact_value_is_default (yyn))
        {
          yyn += YYTERROR;
          if (0 <= yyn && yyn <= YYLAST && yycheck[yyn] == YYTERROR)
            {
              yyn = yytable[yyn];
              if (0 < yyn)
                break;
            }
        }

      /* Pop the current state because it cannot handle the error token.  */
      if (yyssp == yyss)
        YYABORT;


      yydestruct ("Error: popping",
                  yystos[yystate], yyvsp);
      YYPOPSTACK (1);
      yystate = *yyssp;
      YY_STACK_PRINT (yyss, yyssp);
    }

  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  *++yyvsp = yylval;
  YY_IGNORE_MAYBE_UNINITIALIZED_END


  /* Shift the error token.  */
  YY_SYMBOL_PRINT ("Shifting", yystos[yyn], yyvsp, yylsp);

  yystate = yyn;
  goto yynewstate;


/*-------------------------------------.
| yyacceptlab -- YYACCEPT comes here.  |
`-------------------------------------*/
yyacceptlab:
  yyresult = 0;
  goto yyreturn;

/*-----------------------------------.
| yyabortlab -- YYABORT comes here.  |
`-----------------------------------*/
yyabortlab:
  yyresult = 1;
  goto yyreturn;

#if !defined yyoverflow || YYERROR_VERBOSE
/*-------------------------------------------------.
| yyexhaustedlab -- memory exhaustion comes here.  |
`-------------------------------------------------*/
yyexhaustedlab:
  yyerror (YY_("memory exhausted"));
  yyresult = 2;
  /* Fall through.  */
#endif

yyreturn:
  if (yychar != YYEMPTY)
    {
      /* Make sure we have latest lookahead translation.  See comments at
         user semantic actions for why this is necessary.  */
      yytoken = YYTRANSLATE (yychar);
      yydestruct ("Cleanup: discarding lookahead",
                  yytoken, &yylval);
    }
  /* Do not reclaim the symbols of the rule whose action triggered
     this YYABORT or YYACCEPT.  */
  YYPOPSTACK (yylen);
  YY_STACK_PRINT (yyss, yyssp);
  while (yyssp != yyss)
    {
      yydestruct ("Cleanup: popping",
                  yystos[*yyssp], yyvsp);
      YYPOPSTACK (1);
    }
#ifndef yyoverflow
  if (yyss != yyssa)
    YYSTACK_FREE (yyss);
#endif
#if YYERROR_VERBOSE
  if (yymsg != yymsgbuf)
    YYSTACK_FREE (yymsg);
#endif
  return yyresult;
}
#line 607 "src/parser.y" /* yacc.c:1906  */


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
  cursorforward(20);
	printf(BOLDBLACK">> SEASnet Shortcut v0.8 <<"RESET"\n");
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
	printf("* current default port (1~9): "BOLDGREEN"%d"RESET"\n", port);
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
