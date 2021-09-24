#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include "deq.h"
#include "error.h"
#include "mtq.h"

pthread_mutex_t mutex;
pthread_cond_t condv;
pthread_mutex_t mutex2;
pthread_cond_t condv2;
int len2;

extern Mtq new_mtq(){
  Mtq mtq = (Mtq) deq_new();
  pthread_mutex_init(&mutex,0);
  pthread_cond_init(&condv,0);
  pthread_mutex_init(&mutex2,0);
  pthread_cond_init(&condv2,0);
  len2 = 1;
  return mtq;
}

extern void mtq_tail_put(Mtq q, Mole m) {
  Deq deq = (Deq) q;
  pthread_mutex_lock(&mutex);
  len2++;

  while(!len2){
      pthread_cond_wait(&condv,&mutex);
  }
  printf("PRODUCING.....................................PRODUCING MOLE -> %d  LENGTH -> %d\n", m, len2);

  deq_head_put(deq,m);
  pthread_cond_signal(&condv);
  pthread_mutex_unlock(&mutex);
}


extern Mole  mtq_head_get(Deq q) { 
  Deq deq = (Deq) q;
  pthread_mutex_lock(&mutex2);
  len2--;
  
  while(!len2){
       printf("in loop \n");
       pthread_cond_wait(&condv2,&mutex2);  
  }
  Mole data = (Data)malloc(sizeof(long));
  Mole data2 = deq_head_get(deq);
  data = deq_head_rem(deq, data2);
  Mole m = (Mole) data;
  
 printf("CONSUMING....................................CONSUMING MOLE -> %d  LENGTH -> %d\n", m, len2);
  
  pthread_cond_signal(&condv2);
  pthread_mutex_unlock(&mutex2);

  return m; 
}