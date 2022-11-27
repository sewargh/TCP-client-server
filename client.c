// Some required header files
#include	<sys/socket.h>
#include	<netinet/in.h>	
#include   	<arpa/inet.h>
#include 	<netinet/in.h>
#include 	<stdio.h>
#include	<string.h>
#include	<stdlib.h>
#include	<errno.h>
#include 	<unistd.h>

#define MAX_LINE_LENGTH 100
#define	MAXLINE	8192

char * removeCommasFromStr(char *string)
{
    // non_comma_count to keep the frequency of non space characters
    int non_comma_count = 0;
 
    //Traverse a string and if it is non comma character then, place it at index non_space_count
    for (int i = 0; string[i] != '\0'; i++)
    {
        if (string[i] != ',')
        {
            string[non_comma_count] = string[i];
            non_comma_count++;//non_comma_count incremented
        }    
    }
    
    //Finally placing final character at the string end
    string[non_comma_count] = '\0';
    return string;
}

void str_cli(FILE * fp,int sockfd, char time) {
	int operation=0;
    char line[MAX_LINE_LENGTH] = {0},lineOrig[MAX_LINE_LENGTH] = {0};
    unsigned int line_count = 0;
	int n,number;	
	char sendline[MAXLINE], rcvline[MAXLINE];

    while (fgets(line, MAX_LINE_LENGTH, fp))
    {
		if(line_count != 0) sleep(time);
        /* Print each line */
        printf("Reading line number %02d: %s", ++line_count, line);
        strcpy(lineOrig,line); 
		lineOrig[strlen(lineOrig)-1] = 0;// to remove \n; for the output format.
		removeCommasFromStr(line);
		
        /* Add a trailing newline to lines that don't already have one */
        if (line[strlen(line) - 1] != '\n')
            printf("\n");
		
		n = write(sockfd, line, strlen(line));
		if (n < 0) perror("str_cli: write error"); 
		printf("What would you like to do ?\n");
		printf("1.Computing the count of numbers that consist of 1 digit in a list of integers.\n");
		printf("2.Computing the count of numbers that consist of 2 digit in a list of integers.\n");
		printf("3.Finding the maximum number in a list of integers.\n");
		printf("4.Finding the maximum number in a list of integers.\n");
		printf("5.Finding the difference between the maximum and the minimum numbers in a list of integers.\n");
		scanf("%d", &operation);
		if(operation < 1 || operation > 5)
			printf("Unsupported Operation\n");
		else{
			n = write(sockfd, line, strlen(line));
			if (n < 0) perror("str_cli: write error");
			n = write(sockfd, &operation, sizeof(operation));
			if (n < 0) perror("str_cli: write error");

			n = read(sockfd, rcvline, MAXLINE);
			if (n<=0) perror("str_cli: read error");
			rcvline[n] = 0;

			printf("OP%d(%s)=",operation,lineOrig);
			fputs(rcvline, stdout);
		}

    }
    /* Close file */
    if (fclose(fp))
    {
        printf("Error! Could not close file\n"); 
    }
}

int main(int argc, char ** argv) {

	int sockfd, c_retVal;
	struct sockaddr_in servaddr;

	//create socket
	sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if (sockfd < 0) perror("main: socket error");

	//intialize with zero
	memset(&servaddr, 0, sizeof(servaddr));
	servaddr.sin_family = AF_INET;

	//ip address
	inet_pton(AF_INET, argv[1], &servaddr.sin_addr);
	//port number
	servaddr.sin_port = htons(atoi(argv[2])); 

	//connect
	c_retVal = connect(sockfd, (struct sockaddr *) &servaddr, sizeof(servaddr));
	if (c_retVal < 0) perror("main: connect error");

	//stdin is the input file 
	FILE *in_file  = fopen(argv[3], "r"); // read only 
	if (! in_file ) 
            {   
              printf("Error! Could not open file\n"); 
              exit(-1); // must include stdlib.h 
            } 
    printf(argv[4]);     
	//this function is the service that the client will request from the server.
	str_cli(in_file, sockfd,1);
	
	return 0;
}
