/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   memory.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: archid- <archid-@student.1337.ma>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/09 23:33:05 by archid-           #+#    #+#             */
/*   Updated: 2023/01/12 23:39:20 by archid-          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "memory.h"

size_t          size_offset(size_t size)
{
    return size + sizeof(t_chunk);
}

size_t          size_no_offset(size_t size)
{
    return size - sizeof(t_chunk);
}

bool			alloc_init(t_ptr_alloc chunk, size_t size, const size_t max_size)
{
    assert(chunk != NULL);
    assert(size > sizeof(t_alloc));

    chunk->ptr = (unsigned char *)chunk + sizeof(t_chunk);
    chunk->size = size_no_offset(mmap_page_size);
    chunk->available = true;
    chunk->next = NULL;
    return true;
}

bool			page_init(t_ptr_page *page_ref, int page_size)
{
	t_ptr_page page;

    ASSERT(page_ref != NULL);
	ASSERT(page_size % getpagesize() == 0);

	*page_ref = mmap(NULL, page_size, PROT_READ | PROT_WRITE | PROT_EXEC,
					 MAP_ANON | MAP_PRIVATE, -1, 0);
	if (*page_ref == MAP_FAILED)
		return false;
	page = *page_ref;

    page->layout = (unsigned char *)page + sizeof(t_page);
    alloc_init(&page->mem, page->mem, page_size);
    page->n_alloced = 0;
    page->quota = page_size;
    page->next = NULL;
	return true;
}

void			page_del(t_ptr_page *page)
{
	t_ptr_page tmp;
	size_t size;

	tmp = *page;
	size = page->quota + sizeof(t_page);
	unmmap(tmp - sizeof(t_page), size);
	*page = NULL;
}

static void				slice_memory(t_ptr_alloc chunk)
{
	t_ptr_alloc old_next = chunk->next;
	t_ptr_alloc remaining_memory = chunk->ptr + size;

	chunk->next = remaining_memory;

	remaining_memory->size = sz_remaining;
	remaining_memory->ptr = remaining_memory + sizeof(t_alloc);
	remaining_memory->available = true;
	remaining_memory->next = old_next;

}



t_ptr_alloc				page_alloc(t_ptr_page page, size_t size, const size_t hard_limit)
{
    t_ptr_alloc     chunk;
    t_ptr_alloc     tmp;
    size_t          old_size;
	ssize_t			sz_remaining;

	ASSERT(page->quota > size_offset(size));

    chunk = page->layout;
    while (!chunk->available || (chunk->available && chunk->size < size_offset(size)))
    {
        chunk = chunk->next;
        ASSERT(chunk != NULL);
    }

    old_size = chunk->size;
	ASSERT(old_size > size_offset(size));
	sz_remaining = old_size - size_offset(size);
	if (sz_remaining > hard_limit)
	{
		t_ptr_alloc old_next = chunk->next;
		t_ptr_alloc remaining_memory = chunk->ptr + size;

		chunk->next = remaining_memory;

		remaining_memory->size = sz_remaining;
		remaining_memory->ptr = remaining_memory + sizeof(t_alloc);
		remaining_memory->available = true;
		remaining_memory->next = old_next;
    }
    else
    {
		ASSERT(false && "memory fragment");
    }
    chunk->size = size;
    chunk->available = false;
}

void				page_release_alloc(t_ptr_page page, t_ptr_alloc alloc)
{

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
        info->page_size *= (current_alloc_size + (tmp != 0));
    }
}

t_page  g_arena[3] = {
    {NULL,0,0,NULL}, {NULL,0,0,NULL}, {NULL,0,0,NULL},
};
