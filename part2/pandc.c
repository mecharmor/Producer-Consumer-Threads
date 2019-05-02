#include <stdio.h>
#include <string.h>
#include <pthread.h>
#include <ctype.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <semaphore.h>

int N; //Number of Buffers to maintain
int P; //Number of Producer threads
int C; //Number of Consumer threads
int X; //Number of items each producer will produce
int Ptime; //(in seconds) sleep time of producer after producing an item
int Ctime; //(in seconds) sleep time of each consumer after consuming an item

//Mutex and Semaphores
sem_t Full;
sem_t Empty;
pthread_mutex_t Mutex = PTHREAD_MUTEX_INITIALIZER;
//flag
int OverConsume;
int ConsumeItemCount;

//Buffer array
int * BufferArr;
int BufferRear;

//Producer Consumer Test
int * TrackConsumersArray;
int * TrackProducersArray;
int TrackConsumersIndex;
int TrackProducersIndex;

void addProducerTrack(int item){
  TrackProducersIndex++;
  *(TrackProducersArray + TrackProducersIndex) = item;
}
void addConsumerTrack(int item){
  TrackConsumersIndex++;
  *(TrackConsumersArray + TrackConsumersIndex) = item;
}
void printTrackedArrays(){
  for(int i = 0; i <= TrackProducersIndex; i++){
    printf(" %d ", *(TrackProducersArray+i));

  }
  printf("\n");
  for(int i = 0; i <= TrackConsumersIndex; i++){
    printf(" %d ", *(TrackConsumersArray+i));
  }
  printf("\n");
}

//Queue Implementations
int dequeue_item(){
  int item = *(BufferArr);//Get Head of Queue
  //First item was removed so copy all data in array over to the left 1 slot
  for(int i = 0; i < BufferRear; i++){
    *(BufferArr + i) = *(BufferArr + i + 1);
  }
  addConsumerTrack(item);//Add tracked consumer
  BufferRear--;//Decrement buffer
  return item;
}
int enqueue_item(int item){
  BufferRear++;//Next element in array
  *(BufferArr + BufferRear) = item;//set value to new element
  addProducerTrack(item);//Add Producer item that is being tracked
  return item;
}
//Producer-Consumer thread functions
void* Producer( void* arg ){
  struct timespec ts = {Ptime, 0 };
    for(int i = 1; i <= X; i++){
        int idx = *((int *)arg);
        sem_wait(&Empty);
        pthread_mutex_lock(&Mutex);
        printf("%d was produced by producer->     %d\n", enqueue_item(i), idx);
        pthread_mutex_unlock(&Mutex);
        sem_post(&Full);
        nanosleep(&ts, NULL);
    }
}
void* Consumer( void* arg ){
  struct timespec ts = {Ctime, 0 };
  int idx = *((int *)arg);
    for(int i = 1; i <= ConsumeItemCount; i++){
        sem_wait(&Full);
        pthread_mutex_lock(&Mutex);
        printf("%d was consumed by consumer->     %d\n", dequeue_item(),idx);
        pthread_mutex_unlock(&Mutex);
        sem_post(&Empty);
        nanosleep(&ts, NULL);
    }
    //If OverConsume == 1 on last thread then consume one more item
    if(idx == C && OverConsume > 0){
      sem_wait(&Full);
      pthread_mutex_lock(&Mutex);
      printf("%d was consumed by consumer->     %d\n", dequeue_item(),idx);
      OverConsume--;
      pthread_mutex_unlock(&Mutex);
      sem_post(&Empty);
      nanosleep(&ts, NULL);
    }
}
//====================== PRINTING ===================================
void displayTimestamp(){
  time_t t = time(NULL);
  struct tm tm = *localtime(&t);
  printf("Current time: %d/%d/%d - %d:%d:%d\n", tm.tm_mon+1, tm.tm_mday, tm.tm_year+1900, tm.tm_hour, tm.tm_min, tm.tm_sec);
}
void displayTableOfValues(int n, int p, int c, int x, int ptime, int ctime){
  printf("\n                         Number of Buffers : %d\n", n);
  printf("                       Number of Producers : %d\n", p);
  printf("                       Number of Consumers : %d\n", c);
  printf(" Number of items Produced by each producer : %d\n", x);
  printf(" Number of items consumed by each consumer : %d\n", (p*x)/c);
  printf("                          Over consume on? : %d\n", (p*x)%c != 0);
  printf("                       Over consume amount : %d\n", (p*x)%c);
  printf("       Time each Producer Sleeps (seconds) : %d\n",ptime);
  printf("       Time each Consumer Sleeps (seconds) : %d\n\n",ctime);
}
void displayInvalidInput(){
  printf("Not Enough Arguments Entered.\n");
  printf("Example valid command: ./pandc 7 7 9 15 1 1 \n");
  printf("Your output will contain a table similar to: \n");
  displayTableOfValues(7,7,9,15,1,1);
}

int main(int argc, char** argv) {
  displayTimestamp();

  if(argc >= 7){
    //init
    N = atoi(argv[1]);
    P = atoi(argv[2]);
    C = atoi(argv[3]);
    X = atoi(argv[4]);
    Ptime = atoi(argv[5]);
    Ctime = atoi(argv[6]);
    //Array init
    BufferArr = malloc(sizeof(int)*N);
    BufferRear = -1;//set to -1 because buffer contains nothing
    //Show Table
    displayTableOfValues(N,P,C,X,Ptime,Ctime);

    //init consumer values
    ConsumeItemCount = (P*X)/C;
    OverConsume = (P*X)%C != 0;

    //init Test Strategy - Tracker
    TrackConsumersArray = malloc(sizeof(int)*C*ConsumeItemCount + sizeof(int)*OverConsume);
    TrackProducersArray = malloc(sizeof(int)*P*X);
    TrackConsumersIndex = -1;
    TrackProducersIndex = -1;

    //init synchronization
    if(sem_init(&Full, 0, 0) < 0){
      perror("sem_init failed to initialize Full\n");
    }
    if(sem_init(&Empty, 0, N)){
      perror("sem_init failed to initialize Empty");
    }
    //Producer Threads
    pthread_t p_ids[P];
    for(int i = 0 ; i < P ; i++){
        int idx = i+1;
        pthread_create(&p_ids[i],NULL,Producer,(void *)&idx);
    }
    //Consumer Threads
    pthread_t c_ids[C];
    for(int i = 0; i < C; i++){
      int idx = i+1;
      pthread_create(&c_ids[i],NULL,Consumer,(void *)&idx);
    }
    //Join Producers
    for(int i = 0; i < P; i++)
    {
        pthread_join(p_ids[i],NULL);
    }
    //Join Consumers
    for(int i = 0; i < C; i++)
    {
        pthread_join(c_ids[i],NULL);
    }
    printTrackedArrays();
    printf("\n");
    displayTimestamp();
  }else{
    displayInvalidInput();
  }
  return 0;
}
