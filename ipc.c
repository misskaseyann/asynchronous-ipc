#include <stdio.h> 
#include <stdlib.h> 
#include <unistd.h> 
#include <signal.h>
#include <sys/wait.h>

void sigUsrHandler(int);

/* 
 * Communicating Processes
 * Lab 3 Programming Assignment
 *
 * By Kasey Stowell and Kehlsey Lewis
 *
 * Experiment with asynchronous IPC with a program that
 * demonstrates the use of signals for simple communication.
 * 
 */
int main() {

	pid_t pid, ppid;
	int status;

	// Install signal handler(s) for the user-defined signals (SIGUSR1/SIGUSR2).
	signal(SIGINT, sigUsrHandler);
	signal(SIGUSR1, sigUsrHandler);
	signal(SIGUSR2, sigUsrHandler); //only need one handler for all

	//vector holding the signals for random choosing
	int signals[2] = {SIGUSR1, SIGUSR2};

	//intro cat logo
	printf("\n  /\\___/\\\n ( o   o )\n (  =^=  ) \n (        )\n (         )\n (          )))))))))))\n\n");
	printf("~* Welcome to tinyshell by kehlsey & kasey!\n");
	printf("~* To quit please use ctrl + c or forever be stuck looping in tinyshell. \n\n");

	// spawn off a child process
	if ((pid = fork()) < 0) {
		perror("fork failed");
		exit(1);
	}

	// child process
	else if (!pid) {

		ppid = getppid(); //gettting parent id

		while(1) {
			int random = rand() % (5 + 1 - 1) + 1; //generating a random number to sleep
		
			sleep(random);
			printf("\ntinyshell slept for %d seconds...\n", random);	

			int chosen = signals[rand()%2];
			kill(ppid,chosen); //sends SIGUSR1 or SIGUSR2 signal to parent 
		}
	}

	printf("tinyshell spawned child PID# %d\n", pid);
	waitpid(-1, &status, 0); //waiting for child process
	
	return 0; // temp
}

void sigUsrHandler(int signNum) { 

	//terminates when control c recieved
	if (signNum == SIGINT){
		printf("Control-c registered. Exiting...\n");
		exit(0);
	}

	else if (signNum == SIGUSR1) {
		printf("Recieved a SIGUSR1 signal\n");
	}

	else {
		printf("Recieved a SIGUSR2 signal\n");
	}

}