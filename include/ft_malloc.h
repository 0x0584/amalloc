#ifndef FT_MALLOC_H
#define FT_MALLOC_H

#include <assert.h>
#include <errno.h>

#include "memory.h"

void				*ft_malloc(size_t size);
void				*ft_calloc(size_t n_elems, size_t size);
void				ft_free(void *ptr);

struct				s_alloc_info {
  t_chunk		chunk;
  t_page		page;
  struct s_arena_info	arena;
};

void				alloc_info_init(t_generic_ptr ptr, struct s_alloc_info *info);

struct s_alloc_info		ft_malloc_info(t_ptr_generic ptr);

#endif /* FT_MALLOC_H */
