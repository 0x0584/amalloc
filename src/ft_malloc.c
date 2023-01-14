/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_malloc.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: archid- <archid-@student.1337.ma>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/09 23:33:05 by archid-           #+#    #+#             */
/*   Updated: 2023/01/14 16:31:27 by archid-          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_malloc.h"
#include "alloc_info.h"

/*
 * allocation routine, based on `alloc_size' and its ZONE,
 *   - we may either find a page
 *   - or create a new page if no memory is found in amongst all pages
 */

static t_ptr_page			get_alloc_page(size_t alloc_size)
{
    struct s_page_info	info;
	t_ptr_page			page;

    get_page_info(&info, alloc_size);
    if (g_arena[info.page_index].layout != NULL)
        if (!page_construct(&g_arena[info.page_index], info.page_size))
			return NULL;
	page = fetch_page(g_arena + info.page_index, alloc_size);
	if (page->next == NULL && page->quota < size_offset(alloc_size))
	{
		get_page_info(&info, alloc_size);
		page_init(&page->next, info.page_size);
		page = page->next;
	}
	ASSERT(page->quota > size_offset(alloc_size));
	return page;
}

void						*ft_malloc(size_t size)
{
	t_ptr_page	page;
	t_ptr_alloc alloc;

	if ((page = get_alloc_page(size)) == NULL)
		return NULL;
	if (size <= TINY_ZONE_MAX_SIZE)
		alloc = page_alloc(page, size, TINY_ZONE_MAX_SIZE);
	else if (size <= SMALL_ZONE_MAX_SIZE)
		alloc = page_alloc(page, size, SMALL_ZONE_MAX_SIZE);
	else if (size <= LARGE_ZONE_MAX_SIZE)
		alloc = page_alloc(page, size, LARGE_ZONE_MAX_SIZE);
	else
		return NULL;
	ASSERT(alloc != NULL);
	ASSERT(is_valid_memory_ptr(alloc->ptr));
	return alloc->ptr;
}

void						*ft_realloc(void *ptr, size_t size)
{
	if (ptr == NULL && size != 0)
		return ft_malloc(size);
	else if (ptr != NULL && size == 0)
	{
		ft_free(ptr);
		ASSERT(!is_valid_memory_ptr(ptr));
		return ptr;
	}
	return NULL;
}

void						*ft_calloc(size_t n_elems, size_t size)
{
	void			*mem;

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
