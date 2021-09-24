#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>

#include "lawn.h"
#include "mole.h"
#include "deq.h"
#include "mtq.h"


typedef struct {
  long* args[2];			// array for arguments
} *Cont;
int done;
pthread_mutex_t mutexmain;
pthread_create(void *, void *, void *, void *);

static Mole produce(void* a) { 
    void **arg=a;
    Mtq *q=(Mtq)arg[0];
    Lawn l=(Lawn)arg[1];
    mtq_tail_put(q,mole_new(l,0,0));
    return 0; }

static void *consume(void* a){ 
    void **arg=a;
    Mtq *q=(Mtq)arg[0];
 //   pthread_mutex_lock(&mutexmain);
    Mole m = (Mole)mtq_head_get(q);
    mole_whack(m);
 //   pthread_mutex_unlock(&mutexmain);
    return 0; 
}

int main() {
  srandom(time(0));
  const int n=10;
  Lawn lawn=lawn_new(0,0);
  Cont cont=(Cont)malloc(sizeof(n));
  Mtq q = new_mtq();
  cont->args[0] = (void *)q;
  cont->args[1] = (void *)lawn;
  pthread_t tids[n*2];
  done = 0;
 pthread_mutex_init(&mutexmain,0);
  for (int i=1; i<=n; i++) {
    pthread_create(&tids[i],0,produce,cont);
  }
  sleep(5);
   for (int i=1; i<=n; i++) {
    pthread_create(&tids[i+n],0,consume,cont);
  }
    for (int i=1; i<=n*2; i++) {
    printf("JOINING.................................................JOINING THREAD -> %d   int i -> %d\n",tids[i],i);
    pthread_join(tids[i],0);
    printf("JOINED SUCCESFULLY \n");
  }
sleep(6);
  lawn_free(lawn);
}
