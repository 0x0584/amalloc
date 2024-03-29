/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   t_set.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: archid- <archid-@student.1337.ma>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/09 23:33:05 by archid-           #+#    #+#             */
/*   Updated: 2023/01/14 15:23:38 by archid-          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef T_SET_H
#define T_SET_H

#include "hashtable.h"

typedef t_hashtable t_set;

#define MAX_SIZE 1024

t_set		    set_alloc(t_del del, t_cmp cmp, t_hashalgo hasher);
void		    set_del(t_set *s);
bool		    set_insert(t_set s, void *ptr);
bool		    set_erase(t_set s, void *ptr);
size_t		    set_size(t_set s);
bool		    set_probe(t_set s, void *ptr);

extern size_t	g_max_set_size;

#endif
