#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/wait.h>

int main(void) {
    setbuf(stdout, NULL); // disable I/O buffering - all prints are dumped to terminal immediately
	
////////////////////////////////

  char * argument[6];
  FILE *read_file = fopen ("cmds", "r");
  int k = 0;
  for (int k =0; k < 6; k++) {  
	argument[k] = malloc (128); 
    	fscanf (read_file, "%127s", argument[k]);
	k++;
  }

////////////////////////////////


  int status;
  int i;
  int child_pid[3]={0,0,0};

  // create 3 commands - 
  char *cat[] = {argument[0], argument[1], NULL};
  char *head[] = {argument[2], argument[3], NULL};
  char *wc[] = {argument[4], argument[5], NULL};

////////////////////////////////

  int pipes[4];
  
  // Create Pipes
  pipe(pipes);
  pipe(pipes + 2);


  //first child
  if ((child_pid[0] = fork()) == 0)
    {
	printf("In CHILD-1 (PID = %d): executing command cat ... \n", (int)getpid() );	
	
	// replace write with read end of 1st pipe        
	dup2(pipes[1], 1);

	//close the pipes
	close(pipes[0]);
	close(pipes[1]);
	close(pipes[2]);
	close(pipes[3]);

	//executes first command
	execvp(*cat, cat);
    }
  else
    {
	//Second Child
      	if ((child_pid[1] = fork()) == 0)
	{
		printf("In CHILD-2 (PID = %d): executing command head ... \n", (int)getpid() );

		// replace head write with read end of 1st pipe
		dup2(pipes[0], 0);

		// replace head read with write of 2nd pipe
		dup2(pipes[3], 1);

		// close all pipes
		close(pipes[0]);
		close(pipes[1]);
		close(pipes[2]);
		close(pipes[3]);

		//executes first command
		execvp(*head, head);
	}
      else
	{
	  // fork third child
	  if ((child_pid[2] = fork()) == 0)
	    {
		printf("In CHILD-3 (PID = %d): executing command wc ... \n", (int)getpid());	
		
		// replace wc's write with input read of 2nd pipe
		dup2(pipes[2], 0);

		// close all pipes
		close(pipes[0]);
		close(pipes[1]);
		close(pipes[2]);
		close(pipes[3]);

		//execute third command
		execvp(*wc, wc);
	    }
	}
    }
      
  //close the pipes in parent 
  close(pipes[0]);
  close(pipes[1]);
  close(pipes[2]);
  close(pipes[3]);

  for (i = 0; i < 3; i++)
  {
	wait(&status);
	printf("In PARENT (PID = %d): successfully reaped child (PID = %d) \n", getpid(), child_pid[i]);
  }
 
    return 0;
}
