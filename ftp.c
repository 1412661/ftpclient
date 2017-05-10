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

// All info about the current connection
struct FTPClient ftp;

int main(int argc,char *argv[])
{
	char* response;

    if (argc == 2)
	{
        ftp.host = argv[1];
	} else
	{
        printf("[ERROR] Please provide hostname\n");
        return 1;
	}

    printf("[INFO] Welcome to Simle FTP Client\n");
    printf("[INFO] FTP Hostname: %s\n", ftp.host);

    // Default FTP mode is active
    ftp.mode = FTP_ACTIVE;

	if (ftp_connect(&ftp) == 0)
	{
		printf("[INFO] Connected !.\n");
	}

	//struct Socket s;
	//s.port = 1234;

    //getPortListen(&s);


    /*response = establishFTP(&ftp);
    if (response != NULL)
		printf("[INFO] Connected to %s\n%s", ftp.host, response);
	else
	{
        printf("[ERROR] Could not connect to server.\n");
        return 1;
	}*/



    //printf("Please enter username: ");

    return 0;

}
