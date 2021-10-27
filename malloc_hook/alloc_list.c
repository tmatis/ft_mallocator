/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   alloc_list.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmatis <tmatis@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/26 21:01:48 by tmatis            #+#    #+#             */
/*   Updated: 2021/10/27 17:02:37 by tmatis           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "alloc_list.h"
#include <stdio.h>

void add_alloc_list(t_alloc_list **list, void *ptr, size_t size, void * const trace[20])
{
	t_alloc_list *new_alloc;

	if (!(new_alloc = malloc(sizeof(t_alloc_list))))
		return ;
	new_alloc->ptr = ptr;
	new_alloc->size = size;
	
	size_t i;
	for (i = 0; trace[i]; i++)
		new_alloc->trace[i] = trace[i];
	new_alloc->trace[i] = NULL;
	
	new_alloc->next = NULL;
	
	if (*list == NULL)
		*list = new_alloc;
	else
	{
		t_alloc_list *tmp = *list;
		while (tmp->next)
			tmp = tmp->next;
		tmp->next = new_alloc;
	}
}

void remove_alloc_list(t_alloc_list **list, void *ptr)
{
	t_alloc_list *tmp = *list;
	t_alloc_list *prev = NULL;

	while (tmp)
	{
		if (tmp->ptr == ptr)
		{
			if (prev)
				prev->next = tmp->next;
			else
				*list = tmp->next;
			free(tmp);
			return ;
		}
		prev = tmp;
		tmp = tmp->next;
	}
}

size_t size_alloc_list(t_alloc_list *list)
{
	size_t size = 0;
	while (list)
	{
		size++;
		list = list->next;
	}
	return (size);
}

void print_alloc_list(t_alloc_list *list, int fd)
{
	dprintf(fd, "leaked block count: %zd\n", size_alloc_list(list));
	while (list)
	{
		dprintf(fd, "\t%p\t%zd\n", list->ptr, list->size);
		list = list->next;
	}
}
