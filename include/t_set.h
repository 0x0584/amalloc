#ifndef T_SET_H
#define T_SET_H

#include "hashtable.h"

typedef t_hasttable t_set;

#define MAX_SIZE 1024

t_set set_alloc(t_del del, t_cmp cmp);

extern size_t max_set_size;

#endif
