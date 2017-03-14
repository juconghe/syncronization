#include "ProducerConsumer.h"
#include <ctime>
#include <pthread.h>
#include <chrono>
#include <thread>

//TODO: add BoundedBuffer, locks and any global variables here
BoundedBuffer *buffer = new BoundedBuffer(5);
pthread_mutex_t plock;
pthread_mutex_t consumer_lock;
pthread_mutex_t write_lock;
int maxitems;
int itemcount = 0;
int producer_sleep;
int consumer_sleep;
ofstream myfile;

void InitProducerConsumer(int p, int c, int psleep, int csleep, int items){
	//TODO: constructor to initialize variables declared
	//also see instruction for implementation
  // std::cout << "Called" << '\n';
  myfile.open("output.txt",ios::trunc|ios::out);
  plock = PTHREAD_MUTEX_INITIALIZER;
  consumer_lock = PTHREAD_MUTEX_INITIALIZER;
  write_lock = PTHREAD_MUTEX_INITIALIZER;
  producer_sleep = psleep;
  consumer_sleep = csleep;
  maxitems = items;
  pthread_t pthreads[p];
  pthread_t cthreads[c];
  int pid[p],cid[p];
  for (int i = 0; i < p; i++) {
    // std::cout << i << '\n'
    // std::cout << &temp << '\n';
    pid[i] = i;
    pthread_create(&pthreads[i], NULL, producer, &pid[i]);
    // pthread_join(pthreads[i],(void **) &m);
    // std::cout << "Created one producer thread" << '\n';
  }
  for (int j = 0; j < c; j++) {
    // std::cout << j << '\n';
    cid[j] = j;
    pthread_create(&cthreads[j], NULL, consumer,&cid[j]);
    // pthread_join(pthreads[j],(void **) &m);
  }
  for (int i = 0; i < p; i++) {
    pthread_join(pthreads[i],NULL);
  }
  for (int j = 0; j < c; j++) {
    pthread_join(pthreads[j],NULL);
  }
  myfile.close();
}

void* producer(void* threadID){
	//TODO: producer thread, see instruction for implementation
  // printf("max number of items is %d\n",maxitems);
  while (itemcount < maxitems) {
    // std::cout << "ProcSleeping" << '\n';
    // std::cout << "pWaked" << '\n';
    // sleep(1);
    pthread_mutex_lock(&plock);
    // printf("number of items is %d\n",itemcount);
    // printf("max number of items is %d\n",maxitems);
    if (itemcount < maxitems) {
      std::this_thread::sleep_for(chrono::milliseconds(producer_sleep));
      int *id = (int*)threadID;
      int num = rand();
      buffer->append(num);
      itemcount += 1;
      // printf("number of items is %d\n",itemcount);
      pthread_mutex_lock(&write_lock);
      myfile << "Producer #"<<*id<<" time = "<<std::time(NULL)<<"producing data item #"<<itemcount<<" item value=" << num <<endl;
      pthread_mutex_unlock(&write_lock);
      // printf("Producer #%d, time = %ld, producing data item #%d, item value=%d\n",*id,std::time(NULL),itemcount,num);
    }
    // std::cout << "Appending" << '\n';
    pthread_mutex_unlock(&plock);
  }
  // std::cout << "PExit" << '\n';
  // pthread_exit(NULL);
  return 0;
}

void* consumer(void* threadID){
	//TODO: consumer thread, see instruction for implementation
  while (itemcount != maxitems || !buffer->isEmpty()) {
    // std::cout << "cSleeping" << '\n';
    // std::cout << "cWaked" << '\n';
    pthread_mutex_lock(&consumer_lock);
    std::this_thread::sleep_for(chrono::milliseconds(consumer_sleep));
    int value = buffer->remove();
    int *id = (int*)threadID;
    pthread_mutex_lock(&write_lock);
    myfile << "Consumer #"<<*id<<" time = "<<std::time(NULL)<<" consuming data item with value=" << value <<endl;
    pthread_mutex_unlock(&write_lock);
    // printf("Consumer #%d time: %ld, consuming data item with value=%d\n",*id,std::time(NULL),value);
    pthread_mutex_unlock(&consumer_lock);
  }
  // std::cout << "CExit" << '\n';
  // pthread_exit(NULL);
  return 0;
}
