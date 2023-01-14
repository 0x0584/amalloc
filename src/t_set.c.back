/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   t_set.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: archid- <archid-@student.1337.ma>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/09 23:33:05 by archid-           #+#    #+#             */
/*   Updated: 2023/01/14 16:36:05 by archid-          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "t_set.h"

t_set	set_alloc(t_del del, t_cmp cmp, t_hashalgo hasher)
{
	return hashtable_alloc(g_max_set_size, del, cmp, hasher);
}

void	set_del(t_set *s)
{
	hashtable_del(s);
}

bool	set_insert(t_set s, void *ptr)
{
	return hashtable_add(s, ptr, ptr);
}

bool	set_erase(t_set s, void *ptr)
{
	return hashtable_remove(s, ptr);
}

size_t	set_size(t_set s)
{
	return hashtable_count(s);
}

bool	set_probe(t_set s, void *key)
{
	t_hashtable_node	*tmp;
	t_lstnode			walk;

	if (!s || !key)
		return (false);
	walk = lst_front(s->array[s->hasher(key, s->size)]);
	while (walk)
	{
		tmp = walk->blob;
		if (!s->cmp(tmp->key, key))
			return (true);
	}
	return false;
}

size_t	g_max_set_size = MAX_SIZE;
