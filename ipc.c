#include <stdio.h> 
#include <stdlib.h> 
#include <unistd.h> 
#include <signal.h>

void sigIntHandler(int);
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
	pid_t pid;

	// TODO: install signal handler(s) for the user-defined signals (SIGUSR1/SIGUSR2)
	signal(SIGINT, sigIntHandler);

	// spawn off a child process
	if ((pid = fork()) < 0) {
		perror("fork failed");
		exit(1);
	}
	// child process
	else if (!pid) {
		while(1) {
			int random = 3; // TODO make random
			sleep(random);
			printf("heyyy");
			// TODO: randomly send one of the two user-defined signals to its parent
		}
	}
	printf("spawned child PID# %d\n", pid);
	pause();
	// TODO: when a user-defined signal is raised, it reports the type of signal sent
	// note: may be necessary to reinstall your signal handler after a signal is received
	
	return 0; // temp
}

// TODO: terminates gracefully upon receiving a control-c
void sigIntHandler(int signNum) {
	printf("Control-c registered. Exiting...");
	sleep(1);
	printf("Bye!");
	exit(0);
}
// TODO: finish handler for siguser calls.
void sigUsrHandler(int signNum) {
	printf("User signal registered....");
	sleep(1);
	printf("Keep doing your thing...");
}
