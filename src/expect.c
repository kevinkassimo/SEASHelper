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
#include <expect_tcl.h>
#include <expect.h>

#define BUF_SIZE 1024

int main(int argc, char **argv) {
	if (argc != 4) {
		exit(EXIT_FAILURE);
	}
	
	char* username = strdup(argv[1]);
	int port = atoi(argv[2]);
	char* pwd = strdup(argv[3]);
	
	
	extern int exp_timeout;  
	exp_timeout = 100;  
	Tcl_Interp *tcl;  
	tcl = Tcl_CreateInterp();  
	if (Expect_Init(tcl) != TCL_OK) {  
		puts("failure");  
		return 1;  
	}
	//start a connection with remote ssh server 
	char addr[BUF_SIZE];
	memset(addr, 0, BUF_SIZE);
	strcat(addr, username);
	strcat(addr, )
	
	int fd = exp_spawnl("ssh", "ssh", "username@server_address", (char *)0);  
	if(fd < 0) {
		cout<<"Fail to ssh"<<endl;  
		return -1;  
	}  
	int loop = 1;  
	int result;  
	while(loop)  
	{  
		//predefine some expected responses  
		result = exp_expectl(fd, exp_glob, "*assword: ", 1, exp_exact, "Permission denied, please try again.", 2, exp_regexp, "(The authenticity of host)(.)*(Are you sure you want to continue connecting (yes/no)?)", 3, exp_end);  
		char pas[] = "your_password\n";  
		switch(result)  
		{  
			case 1:  
				write(fd, pas, sizeof(pas) - 1);  
				break;  
			case 2:  
				cout <<"wrong password"<<endl;  
				break;  
			case 3:  
				cout<<"connect security"<<endl;  
				write(fd, "yes\n", 4);  
				break;  
			case EXP_EOF:  
				cout << "EOF\n";  
				loop = 0;  
				break;  
			case EXP_TIMEOUT:  
				cout<<"Time out\n";  
				loop = 0;  
				break;  
			default:  
				cout<<"logged in "<<result<<endl;  
				loop = 0;  
				break;  
		}  
	}  
	Tcl_DeleteInterp(tcl);  
}  

