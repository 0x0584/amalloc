/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   alloc_info.h                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: archid- <archid-@student.1337.ma>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/14 20:47:12 by archid-           #+#    #+#             */
/*   Updated: 2023/01/14 20:48:09 by archid-          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef ALLOC_INFO_H
#define ALLOC_INFO_H

#include "memory.h"

struct						s_page_info
{
	int			page_size;
	int			page_index;
};
void						get_page_info(struct s_page_info *info, size_t size);

struct						s_alloc_info
{
	t_alloc				chunk;
	t_page				page;
	struct s_page_info	page_info;
};
void						alloc_info(t_ptr_generic ptr, struct s_alloc_info *info);

bool					    is_valid_memory_ptr(t_ptr_generic ptr);

#endif
