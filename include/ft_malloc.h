#ifndef FT_MALLOC_H
#define FT_MALLOC_H

#include <assert.h>
#include <errno.h>

#include "memory.h"

/*
** NULL is returned if:
**
**  - the size is > LARGE_ZONE_MAX_SIZE
*/
void				*ft_malloc(size_t size);
void				*ft_calloc(size_t n_elems, size_t size);
void				ft_free(void *ptr);

/*
 * allocation routine, based on `alloc_size' and its ZONE,
 *   - we may either find a page
 *   - or create a new page if no memory is found in amongst all pages
 */
void				get_alloc_page(size_t alloc_size);


struct				s_alloc_info {
	t_chunk				chunk;
	t_page				page;
	struct s_arena_info	arena;
};

void				alloc_info_init(t_generic_ptr ptr, struct s_alloc_info *info);

typedef t_ptr_page			(*t_fetch_algorithm)(t_ptr_page root_page, size_t size);

extern t_fetch_algorithm	fetch_page;

#endif /* FT_MALLOC_H */
