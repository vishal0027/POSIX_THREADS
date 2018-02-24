#include <stdio.h>

FILE *fp;
int main(int argc, char **argv[]){
	int count_of_repetation;
	int input_number;
	char prev_line;
	char current_line;
	float mean;
	fp = fopen ("pthread_stats.txt","r");	
	 if (fp == NULL){
       	 	 perror("opening ");
       	 	 return (-1);
  	  }
   	 while ((fscanf(fp, "%d%*[^\n]", &current_line) != -1)){
         	if(prev_line != current_line){
			input_number++;
				printf("The input is %d\n", input_number);
		}
		count_of_repetation++;
		prev_line = current_line;
   	 }
	printf("The input is %d\n", input_number);
	printf("The count is %d\n", count_of_repetation++);
	mean = count_of_repetation/input_number;
	printf("The mean is %d\n", mean);
   	return (0);
}
