#include "memory.h"

size_t				size_offset(size_t size) {
  return size + sizeof(t_chunk);
}

size_t				size_no_offset(size_t size) {
  return size - sizeof(t_chunk);
}

bool		chunk_init(t_ptr_chunk chunk, t_generic_ptr mmap_ptr, size_t mmap_page_size) {
  assert(ptr != NULL);
  assert(chunk_size != 0);
  assert(chunk_size < MAX_CHUNK_SIZE);
  chunk->ptr = (unsigned char *)mmap_ptr + sizeof(t_chunk);
  chunk->size = size_no_offset(mmap_page_size);
  chunk->available = true;
  chunk->next = NULL;
  return true;
}

void		page_init(t_ptr_page arena, int page_size) {
  assert(arena != NULL);
  assert(arena->layout == NULL);
  assert(page_size > sizeof(t_chunk));
  arena->layout = mmap(NULL, page_size, PROT_NONE, MAP_ANON, -1, 0);
  chunk_init(&arena->mem, arena->mem, page_size);
  arena->n_alloced = 0;
  arena->quota = page_size;
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

void get_arena_info(struct arena_info *info, size_t size) {
  assert(info != NULL);
  info->page_size = getpagesize();
  if (current_alloc_size <= TINY_CHUNK) {
    info->index = 0;
  } else if (current_alloc_size <= CHUNK_SIZE) {
    info->index = 1;
    info->page_size *= 2;
  } else {
    info->index = 2;
    int tmp = current_alloc_size % page_size;
    current_alloc_size /= page_size;
    info->page_size *= (current_alloc_size + (tmp != 0))
  }
}


int		arena_init(size_t current_alloc_size) {
  struct page_info info;

  get_page_info(&info, current_alloc_size);
  if (g_arena[info.index].mem != NULL) {
    page_init(&g_arena[info.index], info.page_size);
  }
  return index;
}

void		show_alloc_mem() {

}

t_page 	g_arena[3] = {
  {NULL,0,0,NULL}, {NULL,0,0,NULL}, {NULL,0,0,NULL},
};
