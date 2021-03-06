#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <linux/types.h>
#include <pthread.h>
#include <time.h>
#include <unistd.h>
#include <signal.h>
#include <linux/input.h>
#include <semaphore.h>

sem_t semaphore;
pthread_t thread;

/* Signal handler function */
void handler(int signal_number){
	printf("User 1 Signal handled\n");	//printf to indicate signal handled
	exit(1);		//to terminate the program once the signal is handled
}

/* Thread function */
void* thread_function(void *ptr){
	while(1){
		/* wait for semaphore */
		sem_wait(&semaphore);
		pthread_kill(thread, SIGUSR1);            //generate the signal to the thread
		sem_post(&semaphore);

	}
	return NULL;
}

/* Main program */
int main(){
	/* Creating variable and assigning handler to SIGIO signal */
	struct sigaction sa;
	memset(&sa, 0, sizeof(sa));
	sa.sa_handler = &handler;
	sigaction(SIGUSR1, &sa, NULL);
	
	/* Initialization of semaphore */
	sem_init(&semaphore, 0, 1);
	
	/* creating a thread */
	pthread_create(&thread, NULL, thread_function, NULL);
	
	/* thread join for main program to wait till thread execution is done */
	pthread_join(thread, NULL);
	return 0;
}
