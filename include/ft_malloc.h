#ifndef FT_MALLOC_H
#define FT_MALLOC_H

#include <assert.h>

#include "memory.h"

void		*ft_malloc(size_t size);
void		*ft_calloc(size_t n_elems, size_t size);
void		ft_free(void *ptr);

#endif /* FT_MALLOC_H */
