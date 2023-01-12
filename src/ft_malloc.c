#include "ft_malloc.h"

static t_ptr_page			get_alloc_page(size_t current_alloc_size)
{
    register struct page_info	info;
	t_ptr_page					page;

    get_page_info(&info, current_alloc_size);
    if (g_arena[info.index].mem != NULL)
        page_init(&g_arena[info.index], info.page_size);
	page = fetch_page(g_arena + info.index, size);
	if (page->next == NULL && page->quota < size_offset(size))
	{
		get_arena_info(&info, size);
		page_init(&page->next, info.page_size);
		page = page->next;
	}
	ASSERT(page->quota > size_offset(size));
	return page;
}

void						*ft_malloc(size_t size) {
	t_ptr_page	page;
	t_ptr_alloc alloc;

	page = get_alloc_page(size);
	if (size <= TINY_ZONE_MAX_SIZE)
		alloc = page_alloc(page, size, TINY_ZONE_MAX_SIZE);
	else if (size <= SMALL_ZONE_MAX_SIZE)
		alloc = page_alloc(page, size, SMALL_ZONE_MAX_SIZE);
	else if (size <= LARGE_ZONE_MAX_SIZE)
		alloc = page_alloc(page, size, LARGE_ZONE_MAX_SIZE);
	else
		return NULL;
	ASSERT(alloc != NULL);
	ASSERT(is_valid_alloc_ptr(alloc->ptr));
	return alloc->ptr;
}

void						*ft_realloc(void *ptr, size_t size) {
	if (ptr == NULL && size != 0)
		return ft_malloc(size);
	else if (ptr != NULL && size == 0)
	{
		ft_free(ptr);
		ASSERT(!is_valid_alloc_ptr(ptr));
		return ptr;
	}
	return NULL;
}

void						*ft_calloc(size_t n_elems, size_t size) {
	void *mem;

	mem = ft_malloc(size * n_elems);
	ft_memset(mem, 0, size * n_elems);
	return mem;
}

static inline t_ptr_page    first_fit_page(t_ptr_page page, size_t size)
{
	while (page->next != NULL && page->quota < size_offset(size))
		page = page->next;
	return page;
}

static inline t_ptr_page	best_fit_page(t_ptr_page page, size_t size)
{
	t_ptr_page		best;

	best = NULL;
	while (page->next != NULL)
	{
		if (page->quota >= size_offset(size) &&
			(best != NULL && page->quota < best->quota))
			best = page;
		page = page->next;
	}
	if (best != NULL)
		return best;
	else
		return page;
}

t_fetch_algorithm	fetch_page = first_fit_page;
