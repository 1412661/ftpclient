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
#include "net.h"

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
    //ftp.mode = FTP_ACTIVE;
	ftp.mode = FTP_PASSIVE;
	if (ftp_connect(&ftp))
		return 1;

	if (ftp_auth(&ftp))
		return 1;

    ftp_mode(&ftp, ftp.mode);

    ftp_loop(&ftp);

	//ftp_close(&ftp);

	return 0;
}
