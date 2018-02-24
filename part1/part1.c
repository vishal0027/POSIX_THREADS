//  part1.c
//  part1
//  Created by Vishal Malik on 2017-11-08.
//  Copyright © 2017 Vishal Malik. All rights reserved.
#include <stdio.h>
#include <pthread.h>
#include <sys/syscall.h>
#include <sys/types.h>
//Funtion declarations 
void* write_to(void *pthread);	//Threads call this function 
//--------------Global Variables----------------------//
FILE *fp;			// Common file where each thread will write ("SHARED MEMORY")
int default_run = 1000;		// Default Run by each threads
int temp_threads = 100;	
int mutex = 0;	
pthread_mutex_t filemut = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t start_line = PTHREAD_COND_INITIALIZER;
//------------------------------------------------------//
int main(int argc, char **argv[]){
		//printf("Enter File \n");					
		if((argc > 1 && argc < 3) || argc >3){		// Checks the correct number of arguments
			printf("Usage:-  main (Number_of_threads <INTEGER>) (Number_of_runs <INTEGER>) \" Default Execution will take place \" %n");
		}else if(argc == 3){
			temp_threads = atoll(argv[1]);		// Assign N to temp_threads
			default_run = atoll(argv[2]);		// Assign M to default_run
		}
		pthread_t thread[temp_threads];			// initalize an Array of threads
		fp = fopen ("pthread_stats.txt","a");			 							                
		int i;
		for(i = 0; i<temp_threads; i++){					
			if((pthread_create(&thread[i], NULL, write_to, &thread[i] )) !=0){// Create threads and report if not created.
			printf("Error where thread is not created %d", i);					
			}
		}
		mutex = 1;
		pthread_cond_signal(&start_line);
		int j;
		for(j= 0; j<temp_threads; j++){			//join the threads and report any error occurs
			if((pthread_join(thread[j], NULL)) != 0){
				printf("Error where thread is not joined %d", i);			
			}
		}
		pthread_mutex_destroy(&filemut);
}				
//------------------------------------------------------//
void* write_to(void *pthread){	
	while((mutex == 0)){
		 pthread_cond_wait(&start_line,&filemut);
		 pthread_mutex_unlock(&filemut);	// makes a mutex lock and make the block atomic
	}							
	 	 pid_t tid;					// initialize pid_t variable
		 int i;
		 for(i = 0; i< default_run; i++){ 
			//syscall(SYS_gettid);//This gets the process ID and pthread_self();will get the TID 
			pthread_mutex_lock(&filemut);
	 		tid = pthread_self();				 	//(thread ID).
	 		fprintf (fp,"%d\n",tid);
			pthread_mutex_unlock(&filemut);		// Write on the file its TID.
		 }				
	pthread_cond_signal(&start_line);
}

