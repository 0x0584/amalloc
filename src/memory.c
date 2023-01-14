/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   memory.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: archid- <archid-@student.1337.ma>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/09 23:33:05 by archid-           #+#    #+#             */
/*   Updated: 2023/01/14 16:45:22 by archid-          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "memory.h"

size_t					size_offset(size_t size)
{
    return sizeof(t_alloc) + size;
}

bool					alloc_init(t_ptr_alloc chunk, size_t size)
{
    ASSERT(chunk != NULL);
    chunk->ptr = sizeof(t_alloc) + (unsigned char *)chunk;
    chunk->size = size;
    chunk->available = true;
    chunk->next = NULL;
    return true;
}

bool					page_construct(t_ptr_page page, int page_size)
{
	return page_init(&page, page_size);
}

bool					page_init(t_ptr_page *page_ref, int page_size)
{
	t_ptr_page	page;

    ASSERT(page_ref != NULL);
	ASSERT(page_size % getpagesize() == 0);

	page = mmap(NULL, page_size, PROT_READ | PROT_WRITE | PROT_EXEC,
				MAP_ANON | MAP_PRIVATE, -1, 0);
	if (page == MAP_FAILED)
		return false;

    page->layout = sizeof(t_page) + (t_memory)(unsigned char *)page;
	ASSERT((size_t)page_size > sizeof(t_page));
    alloc_init(page->layout, page_size - sizeof(t_page));
    page->n_alloced = 0;
    page->quota = page_size;
    page->next = NULL;
	*page_ref = page;
	return true;
}

void					page_del(t_ptr_page *page)
{
	t_ptr_page	tmp;
	size_t		size;

	tmp = *page;
	size = tmp->quota + sizeof(t_page);
	munmap(tmp - sizeof(t_page), size);
	*page = NULL;
}

static void				slice_memory(t_ptr_alloc chunk, size_t size, const ssize_t size_limit)
{
	t_ptr_alloc		old_next;
	t_ptr_alloc		remaining_memory;
    size_t          old_size;
	ssize_t			sz_remaining;

	old_size = chunk->size;
	ASSERT(old_size > size_offset(size));
	/* POST_CONDITION: the remaining memory may result in memory that is NOT emough to hold
	 *                 an object, since all allocations require a header */
	sz_remaining = chunk->size - size_offset(size);
	/* remaining memory emough to hold an object */
	if (sz_remaining > size_limit)
	{
		old_next = chunk->next;
		remaining_memory = (t_ptr_alloc)(unsigned char *)chunk->ptr + size;

		/* insert the remaining memory header */
		chunk->next = remaining_memory;

		remaining_memory->size = sz_remaining; /* old_size - (new_size + offset)  */
		remaining_memory->ptr = sizeof(t_alloc) + remaining_memory;
		ASSERT((t_memory)remaining_memory->ptr < (fetch_alloc_page(chunk)->layout + PAGE_SIZE(size_limit)));

		remaining_memory->available = true;
		remaining_memory->next = old_next;
    }
	/* FIXME: memory would result in a fragment */
	/* TODO: compute fragment */
    else
    {
		ASSERT(false);
    }

}

t_ptr_alloc				page_alloc(t_ptr_page page, size_t size, const ssize_t size_limit)
{
    t_ptr_alloc     chunk;

	ASSERT(page->quota > size_offset(size));

	/* find allocation that sia vailable and have enough memory */
    chunk = page->layout;
    while (!chunk->available || (chunk->available && chunk->size < size_offset(size)))
    {
        chunk = chunk->next;
        ASSERT(chunk != NULL);
    }

	slice_memory(chunk, size, size_limit);
    chunk->size = size;
    chunk->available = false;
	return chunk->next;
}

void					page_release_alloc(t_ptr_page page, t_ptr_alloc alloc)
{

}

t_ptr_page				fetch_alloc_page(t_ptr_alloc chunk)
{
	t_ptr_page page;

	ASSERT(g_alloced != NULL);
	page = hashtable_get(g_alloced, chunk, NULL);
	ASSERT(page != NULL);
	return page;
}

#define DEFAULT_SIZE_FACTOR 16

t_page					g_arena[3] = {
    {NULL, 0, 0, NULL},
	{NULL, 0, 0, NULL},
	{NULL, 0, 0, NULL},
};

t_hashtable					g_alloced = NULL;
