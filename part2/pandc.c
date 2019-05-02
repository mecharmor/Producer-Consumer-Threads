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


/*
 * Function to remove item.
 * Item removed is returned
 */
int dequeue_item()
{

}

/*
 * Function to add item.
 * Item added is returned.
 * It is up to you to determine
 * how to use the ruturn value.
 * If you decide to not use it, then ignore
 * the return value, do not change the
 * return type to void.
 */
int enqueue_item(int item)
{

}

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
  printf("       Time each Consumer Sleeps (seconds) : %d\n",ctime);
}
void displayInvalidInput(){
  printf("Incorrect parameters.\n");
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

    displayTableOfValues(N,P,C,X,Ptime,Ctime);
  }else{
    displayInvalidInput();
  }
  return 0;
}
