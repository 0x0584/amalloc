#include "t_set.h"

t_set set_alloc(t_del del, t_cmp cmp) {
	return hashtable_alloc(max_set_size, del);
}

void set_del(t_set s) {
	hashtable_del(s);
}

bool set_insert(t_set s, void *ptr) {
	return hashtable_add(s, ptr, ptr);
}

bool set_erase(t_set s, void *ptr) {
	return hashtable_remove(s, ptr);
}

size_t set_size(t_set s) {
	return hashtable_count(s);
}

bool set_probe(t_set s, const void *ptr) {
	t_hashnode *tmp;

	if (!s || !blob)
		return (false);
	walk = lst_front(s->array[hash]);
	while (walk)
	{
		tmp = walk->blob;
		if (!hash->cmp(tmp->key, key))
			return (true);
	}
	return false;
}

size_t max_set_size = MAX_SIZE;
