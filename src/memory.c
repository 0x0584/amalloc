#include "memory.h"

t_pages 	g_arena[3] = {
  {NULL,0}, {NULL,0}, {NULL,0},
};

void		chunk_init(t_ptr_chunk chunk, t_generic_ptr page, size_t chunk_size, t_ptr_chunk next) {
  assert(chunk_size < MAX_CHUNK_SIZE);
  assert(chunk->blob == NULL);
  assert(chunk->available != NULL);
  assert(chunk->chunk_size == 0);
}

void		page_init(t_ptr_pages arena, int page_size) {
  assert(arena->mem == NULL);
  assert(page_size > sizeof(t_chunk));
  arena->mem = (t_ptr_chunk)mmap(NULL, page_size, PROT_NONE, MAP_ANON, -1, 0);
  arena->mem->blob = NULL;
  arena->mem->chunk_size = 0;
  arena->mem->available = arena->mem + sizeof(t_chunk);
  arena->next = NULL;
}


t_ptr_chunk				page_alloc_tiny_chunk(t_ptr_page page, size_t size) {
  return NULL;
}

t_ptr_chunk				page_alloc_chunk(t_ptr_page page, size_t size) {
  return NULL;
}

t_ptr_chunk				page_alloc_large_chunk(t_ptr_page page, size_t size) {
  return NULL;
}


#define DEFAULT_SIZE_FACTOR 16

int		arena_init(size_t current_alloc_size) {
  register int index;
  register int page_size;

  page_size = getpagesize();
  if (current_alloc_size <= TINY_CHUNK) {
    index = 0;
    page_size *= (2 * DEFAULT_SIZE_FACTOR)
  } else if (current_alloc_size <= CHUNK_SIZE) {
    index = 1;
    page_size *= DEFAULT_SIZE_FACTOR;
  } else {
    index = 2;
    int tmp = current_alloc_size % page_size;
    current_alloc_size /= page_size;
    page_size *= (current_alloc_size + (tmp == 0))
  }
  if (g_arena[index].mem != NULL) {
    page_init(&g_arena[index], page_size);
  }
  return index;
}

void		show_alloc_mem() {

}
