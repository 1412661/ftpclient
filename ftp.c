#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/sendfile.h>
#include <fcntl.h>


#include "const.h"
#include "type.h"
#include "function.h"


char* FTP_HOST;
char* FTP_USER[BUFFSIZE_VAR];
char* FTP_PASS[BUFFSIZE_VAR];

struct Computer Client;
struct Computer Server;

// Default trasmission mode is active
int FTP_MODE = FTP_ACTIVE;



int main(int argc,char *argv[])
{
    if (argc == 2)
	{
        FTP_HOST = argv[1];
	} else
	{
        printf("[ERROR] Please provide FTP_HOST\n");
        return 1;
	}

    printf("[INFO] Welcome to Simle FTP Client\n");
    printf("[INFO] FTP Hostname: %s\n", FTP_HOST);

    int byte;
    char* welcome = ask(FTP_HOST, SERVER_CMD_PORT, "", &byte);

    if (welcome != NULL)
		printf("[INFO] Connected to %s\n%s", FTP_HOST, welcome);
	else
	{
        printf("[ERROR] Could not connect to server.\n");
        return 1;
	}



    //printf("Please enter username: ");

    return 0;

}
