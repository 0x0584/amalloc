/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_malloc.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: archid- <archid-@student.1337.ma>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/09 23:33:05 by archid-           #+#    #+#             */
/*   Updated: 2023/01/14 20:46:38 by archid-          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FT_MALLOC_H
#define FT_MALLOC_H

#include <assert.h>
#include <errno.h>

#include "memory.h"

typedef t_ptr_page			(*t_fetch_algorithm)(t_ptr_page root_page, size_t size);

extern t_fetch_algorithm	fetch_page;

/*
** NULL is returned if:
**
**  - the size is > LARGE_ZONE_MAX_SIZE
*/
void			        	*ft_malloc(size_t size);
void			        	*ft_calloc(size_t n_elems, size_t size);
void			        	ft_free(void *ptr);

#endif /* FT_MALLOC_H */
