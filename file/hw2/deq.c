#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "deq.h"
#include "error.h"

// indices and size of array of node pointers
typedef enum {Head,Tail,Ends} End;

typedef struct Node {
  struct Node *np[Ends];		// next/prev neighbors
  Data data;
} *Node;

typedef struct {
  Node ht[Ends];			// head/tail nodes
  int len;
} *Rep;

static Rep rep(Deq q) {
  if (!q) ERROR("zero pointer");
  return (Rep)q;
}
//takes a representation, the end to use and data to put into node. Will place at head or tail depending on whether end = head or tail. Returns void.
static void put(Rep r, End e, Data d) {
  Node n = (struct Node *)malloc(sizeof(struct Node));
  n->data = d;
  if(r->len == 0){
      r->ht[Head] = n;
      r->ht[Tail] = n;
  }else{
      if(e == Head){
        Node old = r->ht[Head];
        old->np[Head] = n;
        n->np[Tail] = old;
        r->ht[Head] = n;
      }else{
        Node old = r->ht[Tail];
        old->np[Tail] = n;
        n->np[Head] = old;
        r->ht[Tail] = n;
      }
  }
  r->len++;
  
}
//takes a representation, the end to use and int. Will get date at location i in either head or tail depending on whether end = head or tail. Returns data if found else returns NULL.
static Data ith(Rep r, End e, int i) {   
          
  if(i > r->len){return "ith greater than count!";}

  Node n = r->ht[e];
  for(int j =1; j<i; j++){
    if(e == Head){
      n = n->np[Tail];
    }else{
      n = n->np[Head];
    }
  }
  return n->data;
}
static Data get(Rep r, End e) { 

  if(r->len == 0){
      return NULL; 
  }
  return r->ht[e]->data;
}

//takes a representation, the end to use and data to get from node. Will get data from head or tail depending on whether end = head or tail. Returns data if found else returns null.
static Data rem(Rep r, End e, Data d) { 
  Data ret = NULL;
  if(r->len == 0){ return "No elements";}
  if(r->len == 1 && d == r->ht[Tail]->data){ 
    ret = r->ht[Tail]->data;
    free(r->ht[Tail]);
    r->ht[Tail] = r->ht[Head] = NULL; 
    r->len--;

    return ret;
  }
  Data retdata = NULL;
  Node n = r->ht[e];
  Node next = NULL;
  while(n){
    if(n->data == d){
        if(n == r->ht[Head]){
            retdata = r->ht[Head]->data;
            next = r->ht[Head]->np[Tail];
            r->ht[Head] = next;
            next->np[Head] = NULL;
            free(n);
            r->len--;
            return retdata;
        }else if(n == r->ht[Tail]){
            retdata = r->ht[Tail]->data;
            next = r->ht[Tail]->np[Head];
            r->ht[Tail] = next;
            next->np[Tail] = NULL;
            free(n);
            r->len--;
            return retdata;
        }else{
            Node L = n->np[Head];
            Node R = n->np[Tail];
            retdata = n->data;
            free(n);
            r->len--;
            L->np[Tail] = R;
            R->np[Head] = L;
            return retdata;
        }
    }
    if(e == Head){
      n = n->np[Tail];
    }else{
      n = n->np[Head];
    }
  }
return NULL;

}

extern Deq deq_new() {
  Rep r=(Rep)malloc(sizeof(*r));
  if (!r) ERROR("malloc() failed");
  r->ht[Head]=0;
  r->ht[Tail]=0;
  r->len=0;
  return (Data)(long)r;
}

extern int deq_len(Deq q) { return rep(q)->len; }

extern void deq_head_put(Deq q, Data d) {        put(rep(q),Head,d); }
extern Data deq_head_get(Deq q)         { return get(rep(q),Head); }
extern Data deq_head_ith(Deq q, int i)  { return ith(rep(q),Head,i); }
extern Data deq_head_rem(Deq q, Data d) { return rem(rep(q),Head,d); }

extern void deq_tail_put(Deq q, Data d) {        put(rep(q),Tail,d); }
extern Data deq_tail_get(Deq q)         { return get(rep(q),Tail); }
extern Data deq_tail_ith(Deq q, int i)  { return ith(rep(q),Tail,i); }
extern Data deq_tail_rem(Deq q, Data d) { return rem(rep(q),Tail,d); }

extern void deq_map(Deq q, DeqMapF f) {
  for (Node n=rep(q)->ht[Head]; n; n=n->np[Tail])
    f(n->data);
}

extern void deq_del(Deq q, DeqMapF f) {
  if (f) deq_map(q,f);
  Node curr=rep(q)->ht[Head];
  while (curr) {
    Node next=curr->np[Tail];
    free(curr);
    curr=next;
  }
  free(q);
}

extern Str deq_str(Deq q, DeqStrF f) {
  char *s=strdup("");
  for (Node n=rep(q)->ht[Head]; n; n=n->np[Tail]) {
    char *d=f ? f(n->data) : n->data;
    char *t; asprintf(&t,"%s%s%s",s,(*s ? " " : ""),d);
    free(s); s=t;
    if (f) free(d);
  }
  return s;
}
