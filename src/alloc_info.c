/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   alloc_info.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: archid- <archid-@student.1337.ma>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/09 23:33:05 by archid-           #+#    #+#             */
/*   Updated: 2023/01/12 23:37:19 by archid-          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_malloc.h"

void		alloc_info(t_generic_ptr ptr, struct s_alloc_info *info) {
	assert(info != NULL);
	assert(is_valid_ptr(ptr));
	t_chunk chunk;
	chunk = *(ptr - sizeof(t_chunk));

}
