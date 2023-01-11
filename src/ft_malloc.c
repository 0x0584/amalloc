#include "ft_malloc.h"

static inline t_ptr_page        first_fit_page(t_ptr_page page, size_t size) {
	struct arena_info info;
	while (page->next != NULL && page->quota < size_offset(size)) {
		page = page->next;
	}
	if (page->next == NULL && page->quota < size_offset(size)) {
		get_arena_info(&info, size);
		page_init(&page->next, info.page_size);
		page = page->next;
	}
	return page;
}

static inline t_ptr_page	best_fit_page(t_ptr_page page, size_t size) {
	struct arena_info	info;
	bool			found;
	t_ptr_page		tmp;

	found = false;
	tmp = NULL;
	while (page->next != NULL) {
		if (page->quota >= size_offset(size)) {
			if (tmp != NULL && page->quota < tmp->quota) {
				tmp = page;
			}
			found = true;
		}
		page = page->next;
	}
	if (!found) {
		get_arena_info(&info, size);
		page_init(&page->next, info.page_size);
		page = page->next;
		assert(tmp == NULL);
		tmp = page;
	}
	return tmp;
}

typedef t_ptr_page	(*t_fetch_algorithm)(t_ptr_page page, size_t size);
static t_fetch_algorithm fetch_page = first_fit_page;

t_ptr_page fetch_page_hook(size_t size) {
	register int		index;

	index = arena_init(size);
	return fetch_page(g_arena + index, size);
}

void				*ft_malloc(size_t size) {
	t_ptr_page	page;

	page = fetch_page_hook(size);
	if (size <= TINY_CHUNK_SIZE) {
		page_alloc_tiny_chunk(page, size);
	} else if (size <= CHUNK_SIZE) {
		page_alloc_chunk(page, size);
	} else {
		page_alloc_large_chunk(page, size);
	}
	return NULL;
}

void				*ft_realloc(void *ptr, size_t size) {
	if (ptr == NULL && size != 0) {
		return ft_malloc(size);
	} else if (ptr != NULL && size == 0) {
		ft_free(ptr);
		return ptr;
	}

	assert(is_valid_memory_ptr(ptr));
	return NULL;
}

void				*ft_calloc(size_t n_elems, size_t size) {
	return ft_malloc(size * n_elems);
}
