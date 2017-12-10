//gcc posix-rt.c -o posix-rt -lpthread
//Aditya Malik
//Steve Ruan
#include <pthread.h>
#include <stdio.h>
#include <sys/time.h>
#include <sys/wait.h>
#include <unistd.h>
#include <sys/resource.h>
#include <time.h>
#define NUM_THREADS 10

//int arrayTime[5];
//int arrSize = 5;
//struct timeval start_time, stop_time, elapse_time;

/* the thread runs in this function */
void *largeProcess();
void *smallProcess();

int main(int argc, char *argv[])
{
	int i, policy;
	pthread_t tid[NUM_THREADS]; 	/* the thread identifier */
	pthread_attr_t attr; 		/* set of attributes for the thread */

    struct timeval start_time, stop_time, elapse_time;  // timers
    gettimeofday(&start_time, NULL); // Unix timer

    struct timespec qtm;

	/* get the default attributes */
	pthread_attr_init(&attr);

	/* set the scheduling policy - FIFO, RT, or OTHER */
	//pthread_attr_setinheritsched(&attr, PTHREAD_EXPLICIT_SCHED);
	pthread_attr_setschedpolicy(&attr, SCHED_RR);

	if (pthread_attr_setschedpolicy(&attr, SCHED_RR) != 0)
		printf("unable to set scheduling policy to SCHED_RR \n");

	/* get the current scheduling policy */
	if (pthread_attr_getschedpolicy(&attr,&policy) != 0)
		fprintf(stderr, "Unable to get policy.\n");
	else {
		if (policy == SCHED_OTHER)
			printf("SCHED_OTHER\n");
		else if (policy == SCHED_RR)
			printf("SCHED_RR\n");
		else if (policy == SCHED_FIFO)
			printf("SCHED_FIFO\n");
	}

	/* create the threads */
	for (i = 0; i < 5; i++) {
		pthread_create(&tid[i],&attr,largeProcess,NULL);
		sched_rr_get_interval(getpid(), &qtm);
		fprintf(stderr, "Time quantum(thread %lu): %lds %ldns.\n", &tid[i], qtm.tv_sec, qtm.tv_nsec);
	}
    for (i = 5; i < NUM_THREADS; i++) {
        pthread_create(&tid[i],&attr,smallProcess,NULL);
		sched_rr_get_interval(getpid(), &qtm);
		fprintf(stderr, "Time quantum(thread %lu): %lds %ldns.\n", &tid[i], qtm.tv_sec, qtm.tv_nsec);
    }

	/**
	 * Now join on each thread
	 */
	for (i = 0; i < NUM_THREADS; i++) 
		pthread_join(tid[i], NULL);

    gettimeofday(&stop_time,NULL);
    timersub(&stop_time, &start_time, &elapse_time);
    printf("\nTotal time for entire program was %f seconds.\n", elapse_time.tv_sec+elapse_time.tv_usec/1000000.0);
    printf("All processes have finished.\n");
}

/**
 * The thread will begin control in this function. This is our longer process for bubblesort. Tested on printing, it works!!
 */
void *largeProcess()
{
    struct timeval start_time, stop_time, elapse_time;  // timers
    gettimeofday(&start_time, NULL); // Unix timer

    int array[100000];
    int c, d, i, swap;
    //populate an array from 100000 to 1 in reverse order for sorting. This way each time we run it, its running the same exact process.
    //The commented out code was if we wanted random integers in the array, but I realized this would create different run times.
    for(i = 0; i < 100000; i++) {
    	array[i] = 100000 - i;
    }
    //srand(time(NULL));
    // int size = sizeof(array) / sizeof(array[0]);
    // for(i = 0; i < size; i++) {
    //     array[i] = rand() % 100000 + 1; //range is from 1 to 1000
    // }
    // printf("unsorted:\n");
    // for(i = 0; i < size; i++) {
    // 	printf("%d, ", array[i]);
    // }
    //Do bubble sort
    for (c = 0; c < 100000; c++) { 
        for (d = 0; d < 100000 - c - 1; d++) {
            if (array[d] > array[d+1]) {
                swap = array[d];
                array[d] = array[d+1];
                array[d+1] = swap;
            }
        }
    }
    //Print result
    // printf("sorted:\n");
    // for(i = 0; i < size; i++) {
    // 	printf("%d, ", array[i]);
    // }
    //Print times
    gettimeofday(&stop_time,NULL);
    timersub(&stop_time, &start_time, &elapse_time);
    printf("Total time for large process thread was %f seconds.\n", elapse_time.tv_sec+elapse_time.tv_usec/1000000.0);
    //arrayTime[0] = (elapse_time.tv_sec+elapse_time.tv_usec/1000000.0);
	pthread_exit(0);
}

//Changed small process to just adding sums of random intergers in an array of size 1000000. The sum 
//will be from 0 to 999999. This way it will be the same each time the small process runs. Testing on total works!
void *smallProcess() 
{
	struct timeval start_time, stop_time, elapse_time;  // timers
    gettimeofday(&start_time, NULL); // Unix timer

    int array[1000000];
    //srand(time(NULL));
    int sum = 0, i;
    int size = sizeof(array) / sizeof(array[0]);
    //populate array from 0 to 999999
    for(i = 0; i < size; i++) {
        //array[i] = rand() % 1000000 + 1; //range is from 1 to 1000000
        array[i] = i;
    }
    //print elements from 0 to 999999
    // printf("elements:\n");
    // for(i = 0; i < size; i++) {
    // 	printf("%d, ", array[i]);
    // }

    //sum elements from 0 to 999999
    for(i = 0; i < size; i++) {
    	sum = sum + array[i];
    }

    printf("\nTotal: %d\n", sum);

    gettimeofday(&stop_time,NULL);
    timersub(&stop_time, &start_time, &elapse_time);
    printf("Total time for small process thread was %f seconds.\n", elapse_time.tv_sec+elapse_time.tv_usec/1000000.0);
   
    pthread_exit(0);
}

