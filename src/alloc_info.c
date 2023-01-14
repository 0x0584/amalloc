/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   alloc_info.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: archid- <archid-@student.1337.ma>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/09 23:33:05 by archid-           #+#    #+#             */
/*   Updated: 2023/01/14 03:25:36 by archid-          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "alloc_info.h"

void		alloc_info(t_ptr_generic ptr, struct s_alloc_info *info) {
	assert(info != NULL);
	assert(is_valid_memory_ptr(ptr));
	t_alloc chunk;
	ft_memcpy(&chunk, ptr, sizeof(t_alloc));

}

/* FIXME: account for headers, so that size it covers the headers
 * for TINY and SMALL of ZONE_NUM_ELEMENTS  */
void		get_page_info(struct s_page_info *info, size_t size) {
	int tmp;

    ASSERT(info != NULL);
    info->page_size = getpagesize();
    if (size <= TINY_ZONE_MAX_SIZE)
        info->page_index = 0;
	else if (size <= SMALL_ZONE_MAX_SIZE)
	{
        info->page_index = 1;
        info->page_size *= 2;
    }
	else if (size <= LARGE_ZONE_MAX_SIZE)
	{
        info->page_index = 2;
        tmp = size % info->page_size;
        size /= info->page_size;
        info->page_size *= (size + (tmp != 0));
    }
}

void        show_alloc_mem()
{
	ft_printf("TINY ZONE has ");
}

bool		is_valid_memory_ptr(t_ptr_generic ptr)
{
	ASSERT(ptr != NULL);
	return false;
}
