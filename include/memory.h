#ifndef MEMORY_H
#define MEMORY_H

#include <stdlib.h>
#include <stdint.h>
#include <unistd.h>
#include <signals.h>
#include <sys/mman.h>

enum e_chunk_sizes {
  TINY_CHUNK_SIZE = 128,
  CHUNK_SIZE = (TINY_CHUNK_SIZE << 2),

  MAX_CHUNK_SIZE = (2 * MAX_CHUNK_SIZE),
  DEFAULT_NUM_CHUNK_PER_PAGE = 16
};

typedef void			*t_ptr_generic;

typedef struct	s_chunk		t_chunk;
typedef struct	s_chunk		*t_ptr_chunk;
typedef struct	s_chunk		*t_memory;
struct				s_chunk;
{
  t_generic_ptr		blob;
  size_t		chunk_size;
  t_ptr_chunk		available;
};

void				chunk_init(t_ptr_chunk chunk);

typedef struct			s_page t_page;
typedef struct			s_page *t_ptr_page;
struct				s_page {
  t_memory		mem;
  size_t		available;
  t_ptr_page		next;
};

void				page_init(t_ptr_page pages);
t_ptr_chunk				page_alloc_tiny_chunk(t_ptr_page page, size_t size);
t_ptr_chunk				page_alloc_chunk(t_ptr_page page, size_t size);
t_ptr_chunk				page_alloc_large_chunk(t_ptr_page page, size_t size);

extern t_ptr_page 		g_arena[3];
extern t_set			g_alloced;

int				arena_init(t_ptr_page arena);

#endif /* MEMORY_H */
