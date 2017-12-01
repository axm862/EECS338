#include <pthread.h>
#include <string.h>
#include <stdio.h>
#include <unistd.h>     
#include <sys/types.h>  
#include <stdlib.h>  

void *roundRobin();
void *firstComeFirstServe();
void *shortestJob();
void *priorityScheduling();

int main(int argc, char *argv[]) {
	int choice;
	printf("Please choose which algorithm you would like:\n");
	printf("Round Robin (1), First Come First Serve (2), Shortest Job (3), Priority Scheduling (4): ");
	scanf("%d", &choice);

	if (choice == 1) {
		roundRobin();
	}
	else if (choice == 2) {
		firstComeFirstServe();
	}
	else if (choice == 3) {
		shortestJob();
	}
	else if (choice == 4) {
		priorityScheduling();
	}
	else {
		printf("Not a correct option :( Please try again\n"); 
	}
	return 0;
}

void *roundRobin() {
	int i, x, processes;
	int total = 0;
	int counter = 0;
	int time_quantum;
	int wait_time = 0, turnaround_time = 0;
	int arrival_time[20], burst_time[20], temp[20];
	float avg_wait_time, avg_turnaround_time;

	printf("\nEnter Total Number of Processes: ");
	scanf("%d", &processes);
	
	for (i = 0; i < processes; i++) {
		printf("Enter input details for process [%d]\n", i + 1);
		printf("Arrival time: ");
		scanf("%d", &arrival_time[i]);
		printf("Burst time: ");
		scanf("%d", &burst_time[i]);
		temp[i] = burst_time[i];
	}
	printf("\nEnter Time Quantum: ");
	scanf("%d", &time_quantum);
	printf("\nProcess [ID]\t\tBurst Time\tWaiting Time\t\tTurnaround Time\n");

	i = 0;
	x = processes; //how many times to loop
	while(x != 0) {
		if(temp[i] <= time_quantum && temp[i] > 0) {
			total = total + temp[i];
			temp[i] = 0;
			counter = 1;
		}
		else if (temp[i] > 0) {
			temp[i] = temp[i] - time_quantum;
			total = total + time_quantum;
		}
		if (temp[i] == 0 && counter == 1) {
			x--;
			printf("\nProcess %d\t\t%d\t\t%d\t\t\t%d\n", i + 1, burst_time[i], total - arrival_time[i] - burst_time[i], 
				total - arrival_time[i]);
			wait_time = wait_time + total - arrival_time[i] - burst_time[i];
			turnaround_time = turnaround_time + total - arrival_time[i];
			counter = 0;
		}
		if (i == processes - 1) {
			i = 0;
		}
		else if (arrival_time[i+1] <= total) {
			i++;
		}
		else {
			i = 0;
		}
	}

	avg_wait_time = wait_time / processes;
	avg_turnaround_time = turnaround_time / processes;
	printf("\n\nAverage Waiting Time: %f milliseconds.\n", avg_wait_time);
	printf("Average Turnaround Time: %f milliseconds.\n", avg_turnaround_time);
}

void *firstComeFirstServe() {
	int i, j, 
	int processes; //for number of processes 
	int burst_time[20], wait_time[20], turnaround_time[20];
	float avg_turnaround_time = 0, avg_wait_time = 0;

	printf("\nEnter Total Number of Processes: ");
	scanf("%d", &processes);
	printf("\n");
	for(i = 0; i < processes; i++) {
		printf("Enter Process Burst Time for Process [%d]: ", i + 1);
		// printf("Process [%d]: ", i + 1);
		scanf("%d", &burst_time[i]);
	}

	wait_time[0] = 0; //first process has wait time 0 
	for(i = 1; i < processes; i++) {
		wait_time[i] = 0;
		for(j = 0; j < i; j++) {
			wait_time[i] = wait_time[i] + burst_time[j]; 
		}
	}
	printf("\nProcess [ID]\t\tBurst Time\tWaiting Time\tTurnaround Time\n");

	for(i = 0; i < processes; i++) {
		turnaround_time[i] = burst_time[i] + wait_time[i];
		avg_wait_time = avg_wait_time + wait_time[i]; 
		avg_turnaround_time = avg_turnaround_time + turnaround_time[i];
		printf("\nProcess %d\t\t%d\t\t%d\t\t\t%d\n", i + 1, burst_time[i], wait_time[i], turnaround_time[i]);
	}
	avg_wait_time = avg_wait_time / processes;
	avg_turnaround_time = avg_turnaround_time / processes;
	printf("\n\nAverage Waiting Time: %f milliseconds.\n", avg_wait_time);
	printf("Average Turnaround Time: %f milliseconds.\n", avg_turnaround_time);
}

void *shortestJob() {

	int temp, i, j, sum = 0;
	int position; //for selection sorting
	int processes; //for number of processes 
	float avg_wait_time, avg_turnaround_time;
	int burst_time[20], wait_time[20], turnaround_time[20], process[20];
	
	printf("\nEnter Total Number of Processes: ");
	scanf("%d", &processes);	
	printf("\n");
	for(i = 0; i < processes; i++) {
		printf("Enter Process Burst Time for Process [%d]: ", i + 1);
		// printf("Process [%d]: ", i + 1);
		scanf("%d", &burst_time[i]);
		process[i] = i + 1; //contains process number
	}
	
	//TODO: sort burst time in ascending order using selection sort: see if we can find selection sort code online -> DONE
	for(i = 0; i < processes; i++) {
		position = i;
		for (j = i + 1; j < processes; j++) {
			//printf("madeithere\n");
			if(burst_time[j] < burst_time[position]) {
				position = j;
				//printf("madeither2\n");
			}
		}
		//printf("madeithere2\n");
		//do actual sorting below
		//sort burst
		temp = burst_time[i];
		burst_time[i] = burst_time[position];
		burst_time[position] = temp;
		//sort processes to the respecive bursts
		temp = process[i]; 
		process[i] = process[position]; 
		process[position] = temp;
		//printf("madeithere3\n");
	}
	
	wait_time[0] = 0; //first one doesn't wait 
	for(i = 1; i < processes; i++) {
		wait_time[i] = 0;
		for(j = 0; j < i; j++) {
			wait_time[i] = wait_time[i] + burst_time[j]; 
		}
		sum = sum + wait_time[i];
	}
	avg_wait_time = ((float)sum) / processes; 
	sum = 0;

	printf("\nProcess [ID]\t\tBurst Time\tWaiting Time\t\tTurnaround Time\n");

	for(i = 0; i < processes; i++) {
		turnaround_time[i] = burst_time[i] + wait_time[i];
		sum = sum + turnaround_time[i]; 
		printf("\nProcess %d\t\t%d\t\t%d\t\t\t%d\n", process[i], burst_time[i], wait_time[i], 
			turnaround_time[i]);
	}
	avg_turnaround_time = ((float)sum) / processes;

	printf("\n\nAverage Waiting Time: %f milliseconds.\n", avg_wait_time);
	printf("Average Turnaround Time: %f milliseconds.\n", avg_turnaround_time);

}

void *priorityScheduling() {

	int temp, i, j, sum = 0;
	int position; //for selection sort tracking 
	int processes; //for number of processes used
	float avg_wait_time, avg_turnaround_time;
	int burst_time[20], wait_time[20], turnaround_time[20], process[20], priority[20];

	printf("\nEnter Total Number of Processes: ");
	scanf("%d", &processes);	

	for(i = 0; i < processes; i++) {
		printf("\nEnter Process Burst Time for Process [%d]: ", i + 1);
		scanf("%d", &burst_time[i]);
		printf("Enter Process Priority for Process [%d]: ", i + 1);
		scanf("%d", &priority[i]);
		process[i] = i + 1; //contains process number
	}

    //TODO: sorting burst time, priority and process number in ascending order using selection sort -> DONE
	for(i = 0; i < processes; i++) {
		position = i;
		for (j = i + 1; j < processes; j++) {
			if(priority[j] < priority[position]) {
				position = j;
			}
		}
		//do actual sorting and swapping below
		//sort priorirty
		temp = priority[i];
		priority[i] = priority[position];
		priority[position] = temp;
		//sort burst times to respective processes and priority
		temp = burst_time[i];
		burst_time[i] = burst_time[position];
		burst_time[position] = temp;
		//sort processes to respective burst and priority
		temp = process[i]; 
		process[i] = process[position]; 
		process[position] = temp;
	}
	
	wait_time[0] = 0;
	for(i = 1; i < processes; i++) {
		wait_time[i] = 0;
		for(j = 0; j < i; j++) {
			wait_time[i] = wait_time[i] + burst_time[j]; 
		}
		sum = sum + wait_time[i];
	}
	avg_wait_time = ((float)sum) / processes; 
	sum = 0;
	
	printf("\nProcess [ID]\t\tBurst Time\tWaiting Time\tTurnaround Time\n");

	for(i = 0; i < processes; i++) {
		turnaround_time[i] = burst_time[i] + wait_time[i];
		sum = sum + turnaround_time[i]; 
		printf("\nProcess %d\t\t%d\t\t%d\t\t\t%d\n", process[i], burst_time[i], wait_time[i], 
			turnaround_time[i]);
	}
	avg_turnaround_time = ((float)sum) / processes;

	printf("\n\nAverage Waiting Time: %f milliseconds.\n", avg_wait_time);
	printf("Average Turnaround Time: %fmilliseconds.\n", avg_turnaround_time);
}