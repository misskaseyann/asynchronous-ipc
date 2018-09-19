#include <stdio.h> 
#include <stdlib.h> 
#include <unistd.h> 
#include <signal.h>
#include <sys/wait.h>
#include <time.h>

void sigUsrHandler(int);
void sigChildHandler(int);

/* 
 * Communicating Processes
 * Lab 3 Programming Assignment Extra Credit
 *
 * Authors: Kasey Stowell and Kehlsey Lewis
 * Version: Fall 2018
 *
 * Experiment with asynchronous IPC with a program that
 * demonstrates the use of signals for simple communication.
 * 
 */
int main() {

	pid_t pid, pid2, ppid;
	int status;

	struct sigaction sact;
	sact.sa_handler = sigUsrHandler;
	sact.sa_flags = SA_RESTART;

	// Install signal handler(s) for the user-defined signals (SIGUSR1/SIGUSR2).
	sigaction(SIGINT, &sact, NULL);
	sigaction(SIGUSR1, &sact, NULL);
	sigaction(SIGUSR2, &sact, NULL);

	// Vector holding the signals for random choosing.
	int signals[2] = {SIGUSR1, SIGUSR2};

	// Intro cat logo.
	printf("\n  /\\___/\\\n ( o   o )\n (  =^=  ) \n (        )\n (         )\n (          )))))))))))\n\n");
	printf("~* Welcome to our experiment with asynchronous IPC by kehlsey & kasey!\n");
	printf("~* To quit please use ctrl + c or forever be stuck looping. \n\n");

	if ((pid = fork()) != 0) {
		// i am the parent
		printf("parent spawned child PID# %d\n", pid);
		if ((pid2 = fork()) != 0) {
			// i am still the parent
			printf("parent spawned child PID# %d\nwaiting... ", pid2);
			fflush(stdout);
			waitpid(-1, &status, 0); // waiting for child process
		} else {
			//child2
			ppid = getppid();
			sact.sa_handler = sigChildHandler;
			sigaction(SIGINT, &sact, NULL);
			srand(getpid()+ time(0));
			while(1) {
				int random = rand() % (5 + 1 - 1) + 1; // generating a random number to sleep
				sleep(random);	
				int chosen = signals[rand()%2];
				kill(ppid,chosen); // sends SIGUSR1 or SIGUSR2 signal to parent 
			}
		}
	} else {
		//child 1
		ppid = getppid();
		sact.sa_handler = sigChildHandler;
		sigaction(SIGINT, &sact, NULL);
		srand(getpid()+ time(0));
		while(1) {
			int random = rand() % (5 + 1 - 1) + 1; // generating a random number to sleep
			sleep(random);	
			int chosen = signals[rand()%2];
			kill(ppid,chosen); // sends SIGUSR1 or SIGUSR2 signal to parent 
		}
	}
	return 0;
}

/* Handles SIGINT, SIGUSR1, and SIGUSR2 signals. */
void sigUsrHandler(int signNum) { 
	// Terminates when control c recieved.
	if (signNum == SIGINT) {
		fflush(stdout);
		printf(" received. That's it, I'm shutting you down...\n");
		exit(0);
	}
	// Handle SIGUSR1 + SIGUSR2.
	else if (signNum == SIGUSR1) {
		printf("Recieved a SIGUSR1 signal\nwaiting... ");
	}
	else {
		printf("Recieved a SIGUSR2 signal\nwaiting... ");
	}
	fflush(stdout);
}

/* Handles SIGINT for the child only. */
void sigChildHandler(int sigNum) {
	exit(0);
}
