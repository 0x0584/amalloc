#ifndef MEMORY_H
#define MEMORY_H

#include <assert.h>
#define ASSERT assert

#include <stdlib.h>
#include <stdint.h>
#include <unistd.h>
#include <signals.h>
#include <sys/mman.h>
#include <stdbool.h>

typedef void		     	*t_ptr_generic;

typedef struct	s_alloc    	t_alloc;
typedef struct	s_alloc    	*t_ptr_alloc;

struct				    	s_alloc;
{
	t_ptr_generic		ptr;
	size_t				size;
	bool				available;
	t_ptr_alloc	      	next;
};

size_t						size_with_offset(size_t size);
size_t						size_no_offset(size_t size);

void						alloc_init(t_ptr_alloc alloc);

#define ZONE_NUM_ELEMENTS 128

// FIXME: make max_sizes relative to sys_page_size
#define TINY_ZONE_MAX_SIZE 32
#define SMALL_ZONE_MAX_SIZE 512
#define LARGE_ZONE_MAX_SIZE 2048

typedef struct s_page		t_page;
typedef struct s_page		*t_ptr_page;

typedef struct	s_alloc    	*t_memory;

struct						s_page {
	t_memory	    layout;
	size_t			quota;
	size_t			n_alloced;
	t_ptr_page	    next;
};

void		      	    	page_init(t_ptr_page page);
void				    	page_del(t_ptr_page *page);

t_ptr_alloc	          		page_alloc(t_ptr_page page, size_t size, const size_t hard_limit);

t_ptr_alloc	          		page_alloc_tiny(t_ptr_page page, size_t size);
t_ptr_alloc	          		page_alloc_small(t_ptr_page page, size_t size);
t_ptr_alloc	        		page_alloc_large(t_ptr_page page, size_t size);

void				    	page_release_alloc(t_ptr_page page, t_ptr_alloc alloc);


struct						s_arena_info {
	int			page_size;
	int			page_index;
};

void						get_arena_info(struct arena_info *info, size_t size);


extern t_ptr_page 		    g_arena[3];
extern t_set			    g_alloced;

#endif /* MEMORY_H */
