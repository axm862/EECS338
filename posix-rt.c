//gcc posix-rt.c -o posix-rt -lpthread
//Aditya Malik
//Steve Ruan
#include <pthread.h>
#include <stdio.h>
#include <sys/time.h>
#include <sys/wait.h>
#include <unistd.h>
#define NUM_THREADS 5

int array[] = {5,4,3,2,1};
int arrSize = 5;
struct timeval start_time, stop_time, elapse_time;

/* the thread runs in this function */
void *largeProcess();
void *smallProcess();

int main(int argc, char *argv[])
{
	int i, policy;
	pthread_t tid[NUM_THREADS]; 	/* the thread identifier */
	pthread_attr_t attr; 		/* set of attributes for the thread */

	/* get the default attributes */
	pthread_attr_init(&attr);

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
	
	/* set the scheduling policy - FIFO, RT, or OTHER */
	if (pthread_attr_setschedpolicy(&attr, SCHED_OTHER) != 0)
		printf("unable to set scheduling policy to SCHED_OTHER \n");

	/* create the threads */
    pthread_create(&tid[0],&attr,largeProcess,NULL);

    for (i = 1; i < NUM_THREADS; i++)
        pthread_create(&tid[i],&attr,smallProcess,NULL);

	/**
	 * Now join on each thread
	 */
	for (i = 0; i < NUM_THREADS; i++) 
		pthread_join(tid[i], NULL);

    printf("All processes have finished.\n");
}

/**
 * The thread will begin control in this function. This is our longer process for bubblesort
 */
void *largeProcess()
{
    //struct timeval start_time, stop_time, elapsd_time;  // timers
    gettimeofday(&start_time, NULL); // Unix timer
    srand(time[NULL]);
    int array[1000], c, d, swap;
    int i;
    int size = sizeof(array) / sizeof(array[0]);
    for(i = 0; i < size; i++) {
        array[i] = rand() % 1000 + 1; //range is from 1 to 1000
    }

    for (c = 0; c < 1000; c++) { 
        for (d = 0; d < 1000 - c - 1; d++) {
            if (array[d] > array[d+1]) {
                swap = array[d];
                array[d] = array[d+1];
                array[d+1] = swap;
            }
        }
    }

    gettimeofday(&stop_time,NULL);
    timersub(&stop_time, &start_time, &elapse_time);
    printf("Total time was %f seconds.\n", elapse_time.tv_sec+elapse_time.tv_usec/1000000.0);

	pthread_exit(0);
}

// Implement heap sort, but need to merge it into just one function so additional processes don't 
// get added to the ready queue 
void *smallProcess()
{
    struct timeval start_time, stop_time, elapsd_time;  // timers
    gettimeofday(&start_time, NULL); // Unix timer
    heapSort(array,arrSize);
    gettimeofday(&stop_time,NULL);
    timersub(&stop_time, &start_time, &elapse_time);
    printf("Total time was %f seconds.\n", elapse_time.tv_sec+elapse_time.tv_usec/1000000.0);
    
    pthread_exit(0);

}

void heapify(int arr[], int n, int i)
{
    int largest = i;  // Initialize largest as root
    int l = 2*i + 1;  // left = 2*i + 1
    int r = 2*i + 2;  // right = 2*i + 2
    
    // If left child is larger than root
    if (l < n && arr[l] > arr[largest])
        largest = l;
    
    // If right child is larger than largest so far
    if (r < n && arr[r] > arr[largest])
        largest = r;
    
    // If largest is not root
    if (largest != i)
    {
        swap(arr[i], arr[largest]);
        
        // Recursively heapify the affected sub-tree
        heapify(arr, n, largest);
    }
}

// main function to do heap sort
void heapSort(int arr[], int size)
{
    // Build heap (rearrange array)
    for (int i = size / 2 - 1; i >= 0; i--)
        heapify(arr, size, i);
    
    // One by one extract an element from heap
    for (int i=size-1; i>=0; i--)
    {
        // Move current root to end
        swap(arr[0], arr[i]);
        
        // call max heapify on the reduced heap
        heapify(arr, i, 0);
    }
}

