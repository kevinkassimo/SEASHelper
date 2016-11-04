// #include <stdio.h>
// #include <stdlib.h>
// #include <string.h>
// #include <unistd.h>
// #include <fcntl.h>
// #include <getopt.h>
// #include <signal.h>
// #include <errno.h>
// #include <sys/ioctl.h>
// #include <sys/types.h>
// #include <sys/wait.h>
// #include <stdbool.h>
// //BAAAAAH! #include sequence Orz
// #include <tcl.h>
// #include <expect_tcl.h>
// #include <expect.h>
//
// #define BUF_SIZE 1024
// #define TRUE 1
// #define FALSE 0
//
// int main(int argc, char **argv) {
// 	if (argc != 4 && argc != 5) {
// 		exit(EXIT_FAILURE);
// 	}
//
// 	int is_scp = atoi(argv[1]);
// 	char* pwd = strdup(argv[2]);
// 	char* addr_1 = strdup(argv[3]);
// 	char* addr_2;
// 	if (argc == 5) {
// 		addr_2 = strdup(argv[4]);
// 	}
//
// 	//int rc = fork();
//
// 	//if (rc == 0) {
//
// 	extern int exp_timeout;
// 	exp_timeout = 100;
//
// 	/*
// 	Tcl_Interp *tcl;
// 	tcl = Tcl_CreateInterp();
// 	if (Expect_Init(tcl) != TCL_OK) {
// 		fprintf(stderr, "TCL error");
// 		return 1;
// 	}
// 	*/
//
// 	printf("hello?");
//
//
// 	int lol = FALSE;
//
// 	int fd;
// 	if (is_scp == TRUE) {
// 		printf("bad??????");
// 		fd = exp_spawnl("scp", "scp", "-r", addr_1, addr_2, (char *)0);
// 		fprintf(stderr, "haha!!!!");
// 	} else {
// 		printf("bad??????");
// 		fd = exp_spawnl("ssh", "ssh", addr_1, (char *)0);
// 	}
//
// 	if(fd < 0) {
// 		fprintf(stderr, "ERROR: connection failed\n");
// 		exit(EXIT_FAILURE);
// 	}
// 	char pas[] = "KeViN709\n";
// 	int loop = TRUE;
// 	int result;
// 	while (loop) {
// 		//predefine some expected responses
// 		result = exp_expectl(fd,
// 			exp_glob, "*assword: ", 1,
// 			exp_exact, "Permission denied, please try again.", 2,
// 			exp_glob, "Are you sure you want to continue connecting (yes/no)?", 3,
// 			exp_glob, "Last login", 4,
// 			exp_end);
// 		switch(result)
// 		{
// 			case 1:
// 				write(fd, pas, sizeof(pas)-1);
// 				write(fd, "\n", 1);
// 				fprintf(stderr, "here?");
// 				break;
// 			case 2:
// 				fprintf(stderr, "Saved password invalid. Please input manually\n");
// 				loop = FALSE;
// 				break;
// 			case 3:
// 				fprintf(stderr, "Automatically connect\n");
// 				write(fd, "yes\n", 4);
// 				break;
// 			case 4:
// 				break;
// 			case EXP_EOF:
// 				//lol = TRUE;
// 				fprintf(stderr, "Reach end of file\n");
// 				loop = FALSE;
// 				break;
// 			case EXP_TIMEOUT:
// 				fprintf(stderr, "Time out\n");
// 				loop = FALSE;
// 				break;
// 			default:
// 				loop = FALSE;
// 				break;
// 		}
// 	}
//
// 	while (! lol);
//
// 	//Tcl_DeleteInterp(tcl);
//
//
//
// 	kill(exp_pid, SIGCONT);
// }

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
//BAAAAAH! #include sequence Orz
//#include <tcl.h>
//#include <expect_tcl.h>
//#include <expect.h>
#include <tcl8.5/expect.h>

#define BUF_SIZE 1024
#define TRUE 1
#define FALSE 0

int main(int argc, char **argv) {
	if (argc != 4 && argc != 5) {
		exit(EXIT_FAILURE);
	}

	int is_scp = atoi(argv[1]);
	char* pwd = strdup(argv[2]);
	char* addr_1 = strdup(argv[3]);
	char* addr_2;
	if (argc == 5) {
		addr_2 = strdup(argv[4]);
	}

	//int rc = fork();

	//if (rc == 0) {

	extern int exp_timeout;
	exp_timeout = 100;

	/*
	Tcl_Interp *tcl;
	tcl = Tcl_CreateInterp();
	if (Expect_Init(tcl) != TCL_OK) {
		fprintf(stderr, "TCL error");
		return 1;
	}
	*/

	FILE *expect = exp_popen("ssh kun@lnxsrv09.seas.ucla.edu");
	if (expect == 0) {
			return 1;
	}

	enum { denied, invalid, command_not_found,
					command_failed, prompt };

	// exp_fexpectl takes variable parameters terminated by
	// exp_end.  A parameter takes the form
	// {type, pattern, and return value}  corresponding to
	// exp_glob, "password", prompt,  in exp_fexpectl call below.
	// If a pattern matches, exp_fexpectl returns with the value
	// corresponding to the pattern.

	switch (exp_fexpectl(expect,
							exp_glob, "password: ", prompt,
							exp_end)) {
			case prompt:
							// continue
							break;
			case EXP_TIMEOUT:
							fprintf(stderr, "...");
							return 1;
	}
}

	//bool success = true


	/*
	if (is_scp == TRUE) {
		printf("bad??????");
		fd = exp_spawnl("scp", "scp", "-r", addr_1, addr_2, (char *)0);
		fprintf(stderr, "haha!!!!");
	} else {
		printf("bad??????");
		fd = exp_spawnl("ssh", "ssh", addr_1, (char *)0);
	}

	if(fd < 0) {
		fprintf(stderr, "ERROR: connection failed\n");
		exit(EXIT_FAILURE);
	}
	char pas[] = "KeViN709\n";
	int loop = TRUE;
	int result;
	while (loop) {
		//predefine some expected responses
		result = exp_expectl(fd,
			exp_glob, "*assword: ", 1,
			exp_exact, "Permission denied, please try again.", 2,
			exp_glob, "Are you sure you want to continue connecting (yes/no)?", 3,
			exp_glob, "Last login", 4,
			exp_end);
		switch(result)
		{
			case 1:
				write(fd, pas, sizeof(pas)-1);
				write(fd, "\n", 1);
				fprintf(stderr, "here?");
				break;
			case 2:
				fprintf(stderr, "Saved password invalid. Please input manually\n");
				loop = FALSE;
				break;
			case 3:
				fprintf(stderr, "Automatically connect\n");
				write(fd, "yes\n", 4);
				break;
			case 4:
				break;
			case EXP_EOF:
				//lol = TRUE;
				fprintf(stderr, "Reach end of file\n");
				loop = FALSE;
				break;
			case EXP_TIMEOUT:
				fprintf(stderr, "Time out\n");
				loop = FALSE;
				break;
			default:
				loop = FALSE;
				break;
		}
	}

	while (! lol);

	//Tcl_DeleteInterp(tcl);



	kill(exp_pid, SIGCONT);
}




#include <sys/wait.h>
#include <tcl8.5/expect.h>
#include <glog/logging.h>

#include <iostream>
#include <fstream>
#include <string>
#include <iterator>
#include <sstream>
#include <vector>

using namespace std;

int task(const string& host, const string& password,
        const string& command) {

    exp_is_debugging = 0;
    exp_timeout = 20;

    // exp_open spawn our ssh/program and returns a stream.
    FILE *expect = exp_popen((char *) host.c_str());
    if (expect == 0) {
        return 1;
    }

    enum { denied, invalid, command_not_found,
            command_failed, prompt };

    // exp_fexpectl takes variable parameters terminated by
    // exp_end.  A parameter takes the form
    // {type, pattern, and return value}  corresponding to
    // exp_glob, "password", prompt,  in exp_fexpectl call below.
    // If a pattern matches, exp_fexpectl returns with the value
    // corresponding to the pattern.

    switch (exp_fexpectl(expect,
                exp_glob, "password: ", prompt,
                exp_end)) {
        case prompt:
                // continue
                break;
        case EXP_TIMEOUT:
                LOG(ERROR) << "Timeout,  may be invalid host" << endl;
                return 1;
    }

    bool success = true

    // In sending command the return character \r is required.
    fprintf(expect, "%s\r", password.c_str());

    switch (exp_fexpectl(expect,
                exp_glob, "denied", denied, // 1 case
                exp_glob, "invalid password", invalid, // another case
                exp_glob, "$ ", prompt, // third case
                exp_end)) {
        case denied:
                LOG(ERROR) << "Access denied" << endl;
                success = false;
                break;
        case invalid:
                success = false;
                LOG(ERROR) << "Invalid password" << endl;
                break;
        case EXP_TIMEOUT:
                LOG(ERROR) << "Login timeout" << endl;
                break;
        case prompt:
                // continue;
                break;
        default:
                break;
    }

    if (!success) {
        fclose(expect);
        waitpid(exp_pid, 0, 0);
        return 1;
    }

    // Note that the pattern used below are dependent
    // on the command.  Adding more cases to cover
    // various command will help.

    fprintf(expect, "%s\r", command.c_str());

    switch (exp_fexpectl(expect,
        exp_glob, "cannot create", command_failed,
        exp_glob, "command not found", command_not_found,
        exp_glob, "$ ", prompt,
        exp_end)) {
    case command_failed:
        LOG(ERROR) << "Could not create directory" << endl;
        success = false;;
        break;
    case command_not_found:
        LOG(ERROR) << "Command not found" << endl;
        success = false;
        break;
    case prompt:
        fprintf(expect, "%s\r", "exit");
        LOG(INFO) << "Task completed." << endl;
        break;
    }

    // You should always do this.
    // exp_popen spawns a process with pid exp_pid and
    // return a FILE object.

    fclose(expect);
    waitpid(exp_pid, 0, 0);

    if (success)
        return 0;
    else
        return 1;
}
*/
