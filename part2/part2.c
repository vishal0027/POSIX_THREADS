//
//  part2.c
//  part2
//  Created by Vishal Malik on 2017-11-08.
//  Copyright © 2017 Vishal Malik. All rights reserved.
//
#include <stdio.h>
#include <pthread.h>
#include <sys/syscall.h>
#include <sys/types.h>
//Funtion declarations 
void* write_Red(void *pthread);				//Threads call these function 
void* write_Green(void *pthread);
//------------------------------------------------------//
//Global declarations 
FILE *fp;									// Common file where each thread will write ("SHARED MEMORY")
int default_run = 1000;					// Default Run by each threads
int temp_threads = 100;					// Default Number of threads
int number_of_run = 10;					// Default Number of run
int redcnt;						// Red Thread counter
int greencnt;						// Green Thread counter
pthread_mutex_t filemut = PTHREAD_MUTEX_INITIALIZER;	// pthreads mutex
pthread_mutex_t mut = PTHREAD_MUTEX_INITIALIZER;	// pthreads mutex
pthread_mutex_t rmut = PTHREAD_MUTEX_INITIALIZER;	// pthreads mutex
pthread_cond_t signal_green = PTHREAD_COND_INITIALIZER; // pthread cond variale where all green thread waits for next itration.
pthread_cond_t start_line = PTHREAD_COND_INITIALIZER;	// pthread cond variale where thread waits till all threads created
//-----------------------------------------------------//
int main(int argc, char **argv[]){
		if((argc > 1 && argc < 4) || argc >4){
			printf("Usage:-  main (Number_of_threads <INTEGER>) (Number_of_runs <INTEGER>) (Number_of_overall run <INTEGER>)\" Default Execution will take place \" \n");
		}else if(argc == 4){
			temp_threads = atoll(argv[1]);	// Upadation of default variable want to provide argument
			default_run = atoll(argv[2]);
			number_of_run = atoll(argv[3]);	
		}
		fp = fopen ("pthread_stats.txt","a");	// Opening of file (Shared Memory by each thread)

		pthread_t thread_red[temp_threads];	// Array of red threads
		pthread_t thread_green[temp_threads];	// Array of green threads
			
		redcnt = temp_threads;			// redcnt variable initilized with number of required thread
		greencnt = temp_threads;		// greencnt variable initilized with number of required thread
		// Creation of red threads
		int i;
		for(i = 0; i<temp_threads; i++){
			pthread_create(&thread_red[i], NULL, write_Red, &thread_red[i] );
			
		}
		// Creation of green threads
		int k;
		for(k=0; k<temp_threads; k++){
			pthread_create(&thread_green[k], NULL, write_Green, &thread_green[k] );
		}
		pthread_cond_broadcast(&start_line);	// All Thread signalled to start execution
	
		// Joining of red threads
		int j;
		for(j= 0; j<temp_threads; j++){
			pthread_join(thread_red[j], NULL);
			
		}
		// Joining of Green threads
		int z;
		for(z=0; z< temp_threads; z++){
			pthread_join(thread_green[z], NULL);
		}
		pthread_mutex_destroy(&filemut);  	// Destroying the mutex after usage
		return(0);
}
//------------------------------------------------------//
void* write_Red(void *pthread){
	int k = 0;								// Variable counter for "L".
	for(k =0; k< number_of_run; k++){
										//Condition variable allow all threads to wait at condition
				pthread_cond_wait(&start_line,&rmut);		// All Red Thread wait here till all threads are created.
				pthread_mutex_unlock(&rmut);			// Since we know "pthread_cond_wait()" returns the parameter mutex locked, so we unlock it.
								
				int i;
				 for(i = 0; i< default_run; i++){		// Thread Aquires mutex
					 pthread_mutex_lock(&filemut);		 // Runs M times for each thread
	 				 fprintf (fp,"%d\tRed\n", pthread_self()); //syscall(SYS_gettid); This gets the process ID and pthread_self(); will get the TID (thread ID).
					 pthread_mutex_unlock(&filemut);	
			 	}
				redcnt--;						// decrement the redcnt to keep track of red threads
				if(redcnt == 0 ){					// When No more Red Threads are left then it signals the green thread.
				 	greencnt = temp_threads;			// Set green count to total number of threads. 	
					pthread_cond_broadcast(&signal_green);		// When No more Red Threads are left then it signals the green thread.
			 	}								
	}
	pthread_exit(NULL);
}
//--------------------------------------------------------------------//
void* write_Green(void *pthread){
	int k = 0;							// Variable counter for "L".
	for(k =0; k< number_of_run; k++){
				pthread_cond_wait(&signal_green,&mut);	// Condition where all green threads waits for red thread to signal.
									// waits for all Red threads to write
				pthread_mutex_unlock(&mut);		// Since we know "pthread_cond_wait()" returns the parameter mutex locked, so we unlock it.
		
			// green thread  acquire mutex and write on file M times
			int i;
			for(i = 0; i< default_run; i++){
				 pthread_mutex_lock(&filemut);	
		 		fprintf (fp,"%d\tGreen\n", pthread_self());//syscall(SYS_gettid);// This gets the process ID and pthread_self(); will get the TID (thread ID).		
				pthread_mutex_unlock(&filemut);
			}
			greencnt--;					// Decrement green cnt to keep track of how many green are done
		if(greencnt == 0){					// Once every green thread is finished printing it signal again to red to print.
				redcnt = temp_threads;			// It set the red count to again number of threads as it will be needed to keep track of threads.
				pthread_cond_broadcast(&start_line); 
				
		}
		
	}
	pthread_exit(NULL);
}

