


#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h> 
#include <signal.h>

#define MAXLINE 80
#define MAXARGS 20

void executeBuiltInCommand( int cmd, char **argv)
{
  if (strcmp(argv[0],"cd")==0){
  	if (cmd==1){
      		char *myhome;
      		myhome=getenv("HOME");
      		chdir(myhome);
      		char cwd[1024];
        	if (getcwd(cwd, sizeof(cwd)) != NULL) {
			printf("The current working directory after changing: %s \n", cwd);
        		setenv("PWD", cwd, 1);
        	} 	   
    	}else{
      		chdir(argv[1]);
      		char cwd2[1024];
      		if (getcwd(cwd2, sizeof(cwd2)) != NULL) {
			printf("The current working directory after changing: %s \n", cwd2);
			setenv("PWD", cwd2, 1);}   
    		}
  	}	
  	else if ( cmd == 1 && strcmp(argv[0],"pwd")==0){
  		char cwd3[1024];
  		if (getcwd(cwd3, sizeof(cwd3)) != NULL) {
			printf("The current working directory : %s \n", cwd3);
        		setenv("PWD", cwd3, 1);
  		}
  	}else if ( cmd == 1 && strcmp(argv[0],"exit")==0){
    		exit(2);
  	}
}
/* ----------------------------------------------------------------- */
void process_input(int argc, char **argv) {
  /* Problem 1: perform system call execvp to execute command     */ 
  /*           No special operator(s) detected                   */
	int fileId;
	int i;
	int pos=0;
	int countg=0;
	int countl=0;
	for (i =0 ; i < argc;i++){
		if (strcmp(argv[i], ">") == 0)
	 		countg++;
		else if (strcmp(argv[i], "<") == 0)
	 		countl++;
	}
	if ( countg ==1 ||  countl ==1) {
		for (i =0 ; i < argc;i++){   
	  		if (strcmp(argv[i], ">") == 0){
  	    			pos=i;
  	    			if ((argc-1) >pos){
  	      				if (i==0){
            					printf ("ERROR - No command. Make sure file %s is not overwritten.\n", argv[pos+1]); 
	    					_exit(EXIT_FAILURE);
    						//break;
          				}else{
            					fileId = open(argv[pos+1],O_RDWR | O_CREAT | O_TRUNC,  S_IRUSR | S_IWUSR);
  		      				if( fileId < 0 ){
              						printf( "error opening or creating file \n" );
        	  					exit(1);
            					}
      		  				dup2( fileId, 1); /* copy fileID to stdout */
  		      				close( fileId ); /* close fileId - no longer need */
          				        argv[pos] = NULL;                
          				}
        			}else{
     				     printf ("ERROR - No redirection file specified. \n");
          			     _exit(EXIT_FAILURE);
				     //break;
  	    			}
    			}else if (strcmp(argv[i], "<") == 0){
  	    			pos=i;
  	    			if ((argc-1)> pos){
  	      				if (i==0){ 
						printf ("ERROR - No command. Make sure file %s is not overwritten.\n", argv[pos+1]);
						
	    					_exit(EXIT_FAILURE);
						//break; 
          				}else{
            					fileId = open(argv[pos+1],O_RDONLY);
            					if( fileId < 0 ) {
              						printf( "error opening or creating file \n" );
              						exit(1);
          					}
          					dup2( fileId, 0); /* copy fileID to stdin */
  		       				close( fileId ); /* close fileId - no longer need */
         					argv[pos] = NULL;
  		    		       }
        			}else{
          				printf ("ERROR - No redirection file specified. \n");
  	      				//break;
					_exit(EXIT_FAILURE);
        			}
    			}
	  
  		}
	}
	if  (countg > 1 ){ 
		printf ("ERROR - Can’t have %d output redirects on one line. \n", countg); 
        	
	    	_exit(EXIT_FAILURE);
	}
	else if (countl > 1){
         	printf ("ERROR - Can’t have  %d input redirects on one line. \n", countl);
        	
	    	_exit(EXIT_FAILURE);
	}else{
		if (execvp(argv[0],argv)== -1)
	 	perror("shell program");
	}
  	exit (0);
}	
  /* Hint: Please be sure to review execvp.c sample program       */
  /* if (........ == -1) {                                        */  
  /*  perror("Shell Program");                                    */
  /*  _exit(-1);                                                  */
  /* }                                                            */
  /* Problem 2: Handle redirection operators: < , or  >, or both  */ 
/* ----------------------------------------------------------------- */
/*                  parse input line into argc/argv format           */
/* ----------------------------------------------------------------- */
int parseline(char *cmdline, char **argv)
{
  int count = 0;
  char *separator = " \n\t";
  argv[count] = strtok(cmdline, separator);
  while ((argv[count] != NULL) && (count+1 < MAXARGS)) {
    argv[++count] = strtok((char *) 0, separator);
  }
  return count;
}

void handle_SIGINT()
{
	printf("Ouch!\n"); /* UNSAFE (see Section 21.1.2) */
}
/* ----------------------------------------------------------------- */
/*                  The main program starts here                     */
/* ----------------------------------------------------------------- */
int main(void)
{
  char cmdline[MAXLINE];
  char *argv[MAXARGS];
  int argc;
  int status;
  pid_t pid;
  
  /* Loop forever to wait and process commands */
  while (1) {
    	/* Step 1: Name your shell: csc60mshell - m for mini shell */ 
    	printf("csc60mshell> ");
    	fgets(cmdline, MAXLINE, stdin); /* call fgets to read in command, result to store in cmdline */
    	/* Step 1: Call parseline to build argc/argv: argc/argv parameters declared above */ 
    	/* call parseline to parse the command. Result is stored in argc (argurment count), and argv (argurment vectors - individual parsed tokens) */
    	argc=parseline(cmdline, argv);

    	/* Step 1: If user hits enter key without a command, continue to loop again at the beginning */
    
   	if (argc ==0)  
    		continue;
    	/*         Hint: look up for they keyword "continue" in C */
    	/* Step 1: Handle build-in command: exit, pwd, or cd - if detect one              */
    	if (strcmp(argv[0],"cd")==0 || strcmp(argv[0],"exit")==0 || strcmp(argv[0],"pwd")==0){
    		executeBuiltInCommand(argc, argv);
    	}else{ /* Step 1: Else, fork off a process */ 
    		pid = fork();
      		if (pid == -1) 
        		perror("Shell Program fork error");
      		else if (pid == 0){ 
      			struct sigaction handler;
			handler.sa_handler = SIG_IGN;
			sigemptyset(&handler.sa_mask);
			handler.sa_flags = 0;
			sigaction(SIGINT, &handler, NULL);
			/* I am child process. I will execute the command, call: execvp */
        		int i;
			int pipeNum = 0 ;
			for(i=0; i < argc; i++){
				if(strcmp(argv[i], "|" ) ==0){
					pipeNum = i;
				}
			}
			if(pipeNum == 0){
				process_input(argc, argv);
			}else{
				pid_t childpid;
				int fd[2];
				if((pipe(fd) == -1) || ((childpid = fork()) == -1)){
					perror("Failed to set up Pipeline ");
					return(1);
				} 
				if(childpid == 0){
					dup2(fd[1], 1);
					if(close(fd[1])== -1 || close(fd[0])== -1){ 
						perror("Failed to close extra files");	
					}
					argv[pipeNum] = NULL;
					execvp(argv[0], argv);
					perror("Failed to execute");
				}else{
					dup2(fd[0], 0);
					if(close(fd[0])== -1 || close(fd[1]) == -1){
						perror("Failed to close extra files");
					}
					execvp(argv[pipeNum+1], argv+pipeNum+1);
					perror("Failed to execute");	
				}
			}
      		}else 
      		/* I am parent process */
        	if (wait(&status) == -1)
        		perror("Shell Program error");
       		else{
         		if(WIFEXITED(status))
				printf("Child exited with value: %d\n",WEXITSTATUS(status));
			else if(WIFSIGNALED(status))
				printf("Child exited due to signal: %d\n",WTERMSIG(status));
			else if(WIFSTOPPED(status))
    				printf("Child was stopped by signal: %d\n",WIFSTOPPED(status)); 
		}
	}		
  }
}
