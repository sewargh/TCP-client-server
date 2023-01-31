#include	<sys/socket.h>
#include	<netinet/in.h>
#include   	<arpa/inet.h>
#include 	<netinet/in.h>
#include 	<stdio.h>
#include	<string.h>
#include	<stdlib.h>
#include	<errno.h>
#include 	<unistd.h>
#include        <time.h>
#include	<sys/wait.h>
#define	LISTENQ	1024
#define	MAXLINE	8192


void operations(int connfd, const char * addr, FILE * log) {

  int retVal;
  ssize_t n;
  char buf[MAXLINE];

  memset(buf, 0, sizeof(buf));
  while ( (n = read(connfd, buf, MAXLINE)) > 0 ) {

    char ch, buffer[32];
    int  arr[100], i = 0 ,j = 0;
    int choice;
    int onedigitcount =0;
    int twodigitcount =0;
    int maxnumber =0;
    int minnumber=100;
    int diffrence;
    char result[20];

    buf[n] = 0;

    for (int g = 0; g < sizeof(buf); g++) {
	//// Reads the character where the seeker is currently
	ch = buf[g];
	//// If EOF is encountered then break out of the while loop
	if(ch == '\0' ){ break; }
	//// If the delimiter is encounterd then skip the character and store the last read array of characters in an integer array 
	else if(ch == ' ' ){
		//// Converting the content of the buffer into an array position
		arr[j] = atoi(buffer);
		/// Increamenting the array position
		j++;
		//// Clearing the buffer, this function takes two arguments, one is a character pointer and the other one is the size of the character array 
		bzero(buffer, 32);
		//// clearing the counter which counts the number of character in each number used for reading into the buffer. 
		i = 0;
		/// then continue
		continue; }
	else {  //// reads the current character in the buffer
		buffer[i] = ch;
		//// increamenting the counter so that the next character is read in the next position in the array of buffer 
		i++; }}

    printf(" Received text is: ");
    for(int i = 0; i < j-1; i++){
	printf("%d ",arr[i]);  }

    choice = arr[6];
    printf("\n Received operation is: %d\n", choice);


    memset(buf, 0, sizeof(buf));
    switch(choice){
	      case 1 : {
	           for (int a =0 ;a <j-1;a++) {
	              if(arr[a]<10){
	                onedigitcount ++; }}

		      sprintf(buf,"%d",onedigitcount);
		      retVal = write(connfd, buf, sizeof(buf));
	              if (retVal < 0) perror("write error");
		      strcpy(result, "Successful");
		      break; }

	      case 2 :{
	           for (int a =0 ;a <j-1;a++){
	              if(arr[a]>9){
	                twodigitcount ++;} }

		      sprintf(buf,"%d",twodigitcount);
		      retVal = write(connfd, buf, sizeof(buf));
	              if (retVal < 0) perror("write error");
		      strcpy(result, "Successful");
	              break; }

              case 3 : {
	           for (int a =0 ;a <j-1;a++){
	              if(arr[a]>maxnumber) {
                        maxnumber =arr[a]; } }

	              sprintf(buf,"%d",maxnumber);
		      retVal = write(connfd, buf, sizeof(buf));
	              if (retVal < 0) perror("write error");
		      strcpy(result, "Successful");
	              break; }

	      case 4 :{
	           for (int a =0 ;a <j-1;a++){
	              if(arr[a]<minnumber){
                        minnumber =arr[a];}}

		      sprintf(buf,"%d",minnumber);
		      retVal = write(connfd, buf, sizeof(buf));
	              if (retVal < 0) perror("write error");
	              strcpy(result, "Successful");
		      break;}

	      case 5 :{
	           for (int a =0 ;a <j-1;a++){
	              if(arr[a]>maxnumber){
                        maxnumber =arr[a];}
	              if(arr[a]<minnumber){
                        minnumber =arr[a];}
                      }

	              diffrence = maxnumber - minnumber;

		      sprintf(buf,"%d",diffrence);
		      retVal = write(connfd, buf, sizeof(buf));
	              if (retVal < 0) perror("write error");
	              strcpy(result, "Successful");
		      break; }

	      default: {
		   strcpy(buf, "Unsupported Operation");
		   retVal = write(connfd, buf, sizeof(buf));
	           if (retVal < 0) perror("write error");
		   strcpy(result, "Unsuccessful"); }
		   }



  char *pts; /* pointer to time string */
  time_t now; /* current time */
  char *ctime();
  (void) time(&now);
  fprintf(log ,"LOCAL TIME :            %s", ctime(&now));
  fprintf(log ,"CLIENT IP ADDRESS :	%s \n", addr);
  fprintf(log ,"OPERATION NUMBER :	%d \n", choice);
  fprintf(log ,"STATUS :                %s \n---------------------------------------------------------- \n", result);

} if (n < 0) perror("read error"); }

int main(int argc, char ** argv) {
    FILE *log;
    log  = fopen ("log-file.txt", "w");

    ssize_t n;
    char buf[MAXLINE];
    pid_t pid;
    int listenfd, connfd, l_retVal, b_retVal, retVal, status ,clicount = 0;
    socklen_t len;
    struct sockaddr_in cliaddr, servaddr;
    char addr[MAXLINE];

	listenfd = socket(AF_INET, SOCK_STREAM, 0);
	if (listenfd < 0) perror("main: socket error");

	memset(&servaddr, 0, sizeof(servaddr));
	servaddr.sin_family = AF_INET;
	servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
	servaddr.sin_port = htons(atoi(argv[1]));
	b_retVal = bind(listenfd, (struct sockaddr *) &servaddr, sizeof(servaddr));
	if (b_retVal < 0) perror("main: bind error");

	l_retVal = listen(listenfd, LISTENQ);
	if (l_retVal < 0) perror("main: listen error");

	for ( ; ; ) {
			len = sizeof(cliaddr);
			connfd = accept(listenfd, (struct sockaddr *) &cliaddr, &len);
			if (connfd < 0) {perror("main: accept error"); printf("not accepted\n");}
			clicount++;

			if (clicount > 5)
                                  if((pid = waitpid(-1 ,&status,0)) == -1)
                                     perror("main: wait error");
                                  else clicount--;

			if( (pid = fork()) == 0 ) {
				retVal = close(listenfd);
				if (retVal < 0) perror("main: child close error");
				operations(connfd, inet_ntop(AF_INET , &cliaddr.sin_addr, addr , sizeof(addr) ), log);
				exit(0);
			}
			else {
			retVal = close(connfd);
			if (retVal < 0) perror("main: parent close error");
		}}
    fclose(log);
    return 0; }
