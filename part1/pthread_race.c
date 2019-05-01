#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <time.h>
#include <sys/time.h>
/**
 * THESE DEFINE VALUES CANNOT BE CHANGED.
 * DOING SO WILL CAUSE POINTS TO BE DEDUCTED
 * FROM YOUR GRADE
 */
 /** BEGIN VALUES THAT CANNOT BE CHANGED */
#define MAX_THREADS 16
#define MAX_ITERATIONS 40
/** END VALUES THAT CANNOT BE CHANGED */

/**
 * use this struct as a parameter for the function
 * nanosleep.
 * For exmaple : nanosleep(&ts, NULL);
 */
struct timespec ts = {0, 123456 };
int counter;
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

void* thread_func_add( void* arg ){
    for(int i = 0; i < MAX_ITERATIONS; i++){
        int idx = *((int *)arg);
        pthread_mutex_lock(&mutex);
        int temp = counter;
        temp += 10;
        counter = temp;
        pthread_mutex_unlock(&mutex);
        printf("Current Value written to Global Variables by ADDER thread id: %d is %d\n",idx,temp);
        nanosleep(&ts, NULL);
    }
}
void* thread_func_sub( void* arg ){
    for(int i = 0; i < MAX_ITERATIONS; i++){
        int idx = *((int *)arg);
        pthread_mutex_lock(&mutex);
        int temp = counter;
        temp -= 10;
        counter = temp;
        pthread_mutex_unlock(&mutex);
        printf("Current Value written to Global Variables by SUBTRACTOR thread id: %d is %d\n",idx,temp);
        nanosleep(&ts, NULL);
    }
}

int
main(int argc, char** argv)
{
    counter = 0;

    pthread_t ids[MAX_THREADS];
    int i;

    for(i = 0 ; i < MAX_THREADS ; i++)
    {
        int idx = i;
        if(i%2 == 0){
            //even
            pthread_create(&ids[i],NULL,thread_func_add,(void *)&idx);
        }else{
            //odd
            pthread_create(&ids[i],NULL,thread_func_sub,(void *)&idx);
        }

    }
    for(i = 0; i < MAX_THREADS; i++)
    {
        pthread_join(ids[i],NULL);
    }

    printf("Final Value of Shared Variable : %d\n",counter);

    return 0;
}
