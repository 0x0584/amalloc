#ifndef MEMORY_H
#define MEMORY_H

#include <stdlib.h>
#include <stdint.h>
#include <unistd.h>
#include <signals.h>
#include <sys/mman.h>
#include <stdbool.h>

/*

    page of size page_size

     +-------------+<--------------------------| base pointer returned by mmap
     | layout      +------+
     | quota       |	  |
     | next page   |+-----v------+
     +-------------+| ptr        +-------+  ptr always points to the offset of the base pointer
                    | size       | 	 |  size without the offset
                    | available  |     	 |  true if the chunk is free
             +------+ next       |	 |  points to the next chunk, or NULL if it is the last
             |      | ~~~~~~~~   <-------+
             |      | ~~~~~~~    |
             |      | ~~~~~~~~   |
             |      | ~~~~~~~~~  |
             |      | ~~~~~~~    |
             +------> ptr        +-------+
                    | size       |       |
                    | available  |       |
        NULL <------+ next       |       |
                    |            <-------+
                    |            |
       	       	    |            |
		            |            |
		            |            |
		            |            |
		            +------------+

 */

typedef void		     	*t_ptr_generic;

typedef struct	s_alloc    	t_alloc;
typedef struct	s_alloc    	*t_ptr_alloc;
typedef struct	s_alloc    	*t_memory;
struct				    	s_alloc;
{
  t_ptr_generic		ptr;
  size_t	      	size;
  bool			available;
  t_ptr_alloc	      	next;
};

size_t				size_with_offset(size_t size);
size_t				size_no_offset(size_t size);
void		    	alloc_init(t_ptr_alloc alloc);


typedef struct	   		s_page t_page;
typedef struct	   		s_page *t_ptr_page;
struct		      		s_page {
  t_memory	      	layout;
  size_t	      	quota;
  size_t			n_alloced;
  t_ptr_page	    next;
};

struct				s_arena_info {
  int			page_size;
  int			page_index;
};

void					get_arena_info(struct arena_info *info, size_t size);

void		      		page_init(t_ptr_page pages);
t_ptr_alloc	      		page_tiny_alloc(t_ptr_page page, size_t size);
t_ptr_alloc	      		page_small_alloc(t_ptr_page page, size_t size);
t_ptr_alloc	      		page_large_alloc(t_ptr_page page, size_t size);

extern t_ptr_page 		g_arena[3];
extern t_set			g_alloced;

int				arena_init(t_ptr_page arena);

#define ZONE_NUM_ELEMENTS 128

#define TINY_ZONE_MAX_SIZE 32
#define SMALL_ZONE_MAX_SIZE 512
#define LARGE_ZONE_MAX_SIZE 4096

#endif /* MEMORY_H */
