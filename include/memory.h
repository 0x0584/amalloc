/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   memory.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: archid- <archid-@student.1337.ma>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/09 23:33:43 by archid-           #+#    #+#             */
/*   Updated: 2023/01/14 20:47:27 by archid-          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MEMORY_H
# define MEMORY_H

# include <assert.h>
# define ASSERT assert

# include <stdlib.h>
# include <stdint.h>
# include <unistd.h>
# include <signal.h>
# include <sys/mman.h>
# include <stdbool.h>

# include "libft.h"

#include "t_set.h"

/*
**
**  - when calling `malloc()' yields a pointer to memory preceeded by metadata
**
**  - when calling `calloc()' similar to `malloc()' but guarentes a wiping of memory
**
**  - when calling `realloc()' the incoming pointer is NOT guarented
**    to remain valid, as it may relocate the memory in case it could NOT
**    fit in the same page a new allocation would be reserved, and the data
**    is guarented to be duplicated
*/
typedef void		     	*t_ptr_generic;

typedef struct	s_alloc    	t_alloc;
typedef struct	s_alloc    	*t_ptr_alloc;

struct				    	s_alloc
{
	/*
	** points to the beginning of available memory
	** (ptr - offset is the actual allocation footprint)
	*/
	t_ptr_generic		ptr;

	/*
	** size of the available memory for the allocation
	** (without regard to the offset)
	*/
	size_t				size;

	/*
	** indicates whether a memory of `size' is available
	** (without regard to the offset)
	*/
	bool				available;

	/*
	** allocations are mapped
	*/
	t_ptr_alloc	      	next;
};

/*
**
*/
bool						alloc_init(t_ptr_alloc chunk, size_t size);

/*
** a page is allocated via `mmap()' where the size is considered as a buffer that
** will be granuraly managed for all allocations that may fit in the page
**
** each page reserves a metadata header (only once), then every allocation would
** have its own metadata header, followed by the allocated memory
**
** each type of object has their own separated root page node
*/
typedef struct s_page		t_page;
typedef struct s_page		*t_ptr_page;
typedef struct	s_alloc    	*t_memory;
struct						s_page {
    /*
	** this alias is to indicate it's the root node of all allocations in the page
	*/
	t_memory	    layout;

	/*
	** size of the memory allocated by `mmap()' - sizeof(t_page)
	*/
	size_t			quota;

	/*
	** number of active allocation on the page
	*/
	size_t			n_alloced;

	/*
	** incase a page was full, or could not find suitable memory to fit
	** the object, a new page is allocated
	*/
	t_ptr_page	    next;
};

/*
** the offset is always considered of the allocation
**
** although the page has an offset too these functions are utilitises
** to adjust for the offset of `s_alloc'
*/
size_t						size_offset(size_t size);

# define ZONE_NUM_ELEMENTS 128
# define PAGE_SIZE(__S) (ZONE_NUM_ELEMENTS * size_offset(__S))

// FIXME: make max_sizes relative to sys_page_size
# define TINY_ZONE_MAX_SIZE 32
# define SMALL_ZONE_MAX_SIZE 512
# define LARGE_ZONE_MAX_SIZE 2048


/*
** page constructor, allocates memory using `mmap()'
*/
bool						page_construct(t_ptr_page page, int page_size);
bool						page_init(t_ptr_page *page_ref, int page_size);

/*
**
*/
void				    	page_del(t_ptr_page *page);

/*
** @return the remaining memory if available or pointer to ta fragment
*/
t_ptr_alloc	          		page_alloc(t_ptr_page page, size_t size, const ssize_t size_limit);

void				    	page_release_alloc(t_ptr_page page, t_ptr_alloc alloc);
t_ptr_page					fetch_alloc_page(t_ptr_alloc chunk);

extern t_page				g_arena[3];
extern t_hashtable			g_alloced;

#endif /* MEMORY_H */
