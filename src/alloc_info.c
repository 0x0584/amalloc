#include "ft_malloc.h"

void		alloc_info(t_generic_ptr ptr, struct s_alloc_info *info) {
	assert(info != NULL);
	assert(is_valid_ptr(ptr));
	t_chunk chunk;
	chunk = *(ptr - sizeof(t_chunk));

}
