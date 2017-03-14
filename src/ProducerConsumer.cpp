#include "ProducerConsumer.h"
#include <ctime>
#include <pthread.h>
#include <chrono>
#include <thread>

//TODO: add BoundedBuffer, locks and any global variables here
BoundedBuffer *buffer = new BoundedBuffer(10);
pthread_mutex_t plock;
pthread_mutex_t consumer_lock;
int maxitems;
int itemcount = 0;
int producer_sleep;
int consumer_sleep;

void InitProducerConsumer(int p, int c, int psleep, int csleep, int items){
	//TODO: constructor to initialize variables declared
	//also see instruction for implementation
  // std::cout << "Called" << '\n';
  plock = PTHREAD_MUTEX_INITIALIZER;
  consumer_lock = PTHREAD_MUTEX_INITIALIZER;
  producer_sleep = psleep;
  consumer_sleep = csleep;
  maxitems = items;
  pthread_t pthreads[p];
  pthread_t cthreads[c];
  for (int i = 0; i < p; i++) {
    // std::cout << "Creating pthread" << '\n';
    pthread_create(&pthreads[i], NULL, producer, NULL);
  }
  for (int j = 0; j < c; j++) {
    // std::cout << "Creating cthread" << '\n';
    pthread_create(&cthreads[j], NULL, consumer,NULL);
  }
}

void* producer(void* threadID){
	//TODO: producer thread, see instruction for implementation
  while (itemcount < maxitems) {
    // std::cout << "pSleeping" << '\n';
    // std::this_thread::sleep_for(chrono::milliseconds(producer_sleep));
    // std::cout << "pWaked" << '\n';
    // sleep(1);
    pthread_mutex_lock(&plock);
    if (itemcount < maxitems) {
      int num = rand();
      buffer->append(num);
      itemcount += 1;
      // printf("number of items is %d\n",itemcount);
      printf("time: %ld, appending %d\n",std::time(NULL),num);
    }
    // std::cout << "Appending" << '\n';
    pthread_mutex_unlock(&plock);
  }
  std::cout << "PExit" << '\n';
  pthread_exit(NULL);
}

void* consumer(void* threadID){
	//TODO: consumer thread, see instruction for implementation
  while (itemcount != maxitems || !buffer->isEmpty()) {
    // std::cout << "cSleeping" << '\n';
    // std::this_thread::sleep_for(chrono::milliseconds(consumer_sleep));
    // std::cout << "cWaked" << '\n';
    pthread_mutex_lock(&consumer_lock);
    buffer->remove();
    // printf("time: %ld\n",std::time(NULL));
    pthread_mutex_unlock(&consumer_lock);
  }
  std::cout << "CExit" << '\n';
  pthread_exit(NULL);
}
