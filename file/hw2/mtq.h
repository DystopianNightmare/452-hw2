#ifndef MTQ_H
#define MTQ_H

#include "mole.h"

typedef void *Mtq;

extern Mtq new_mtq();
extern void mtq_tail_put(Deq q, Mole d);
extern Mole  mtq_head_get(Deq q);

#endif