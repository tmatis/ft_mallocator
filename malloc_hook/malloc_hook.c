/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   malloc_hook.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmatis <tmatis@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/25 13:28:09 by tmatis            #+#    #+#             */
/*   Updated: 2021/10/26 21:33:38 by tmatis           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include <stdio.h>
#include <execinfo.h>
#include <errno.h>
#include <fcntl.h>
#include <string.h>
#include <dlfcn.h>
#include "utils_hook.h"
#include "alloc_list.h"
#include "malloc_hook.h"


int g_malloc_hook_active = 1;

int g_at_exit_hook_active = 0;

int g_fd_out = STDOUT_FILENO; // ./ft_mallocator/res.tmp

t_alloc_list *g_alloc_list = NULL;


void setup_g_fetch(void)
{
}

void setup_g_fd_out(void)
{
	/*
	g_fd_out = open("./ft_mallocator/res.tmp", O_WRONLY | O_CREAT, 0644);
	if (g_fd_out < 0)
	{
		perror("open");
		exit(EXIT_FAILURE);
	}
	*/
}

void at_exit_hook(void)
{
	g_malloc_hook_active = 0;
	dprintf(g_fd_out, "at_exit_hook\n");
	dprintf(g_fd_out, "leaked blocks: %zi\n", size_alloc_list(g_alloc_list));
}

void get_backtrace(void *trace[20])
{
	void *array[23];
	size_t size;

	size = backtrace(array, 23);
	size_t y = 0;
	for (size_t i = 2; i < (size - 2); i++)
	{
		trace[y++] = array[i];
	}
	trace[y] = NULL;
}


void *alloc_hook(size_t size, void *caller)
{
	char *result = NULL;
	
	g_malloc_hook_active = 0;

	if (g_fd_out == -1)
		setup_g_fd_out();

	if (!g_at_exit_hook_active)
	{
		g_at_exit_hook_active = 1;
		atexit(at_exit_hook);
	}

	size_t caller_address = (size_t)(&_end) - (size_t)caller;
	if ((caller_address & 0xffff000000000000) == 0)
	{
		void *trace[20];

		get_backtrace(trace);
		
		result = malloc(size);
		add_alloc_list(&g_alloc_list, result, size, trace);
	}
	else
		result = malloc(size);

	g_malloc_hook_active = 1;

	return (result);
}

void *malloc(size_t size)
{
	void *caller;

	caller = __builtin_return_address(0);
	if (g_malloc_hook_active)
		return (alloc_hook(size, caller));
	return (__libc_malloc(size));
}

void *calloc(size_t nmemb, size_t size)
{
	void *caller;

	caller = __builtin_return_address(0);
	if (g_malloc_hook_active)
	{
		char *result = alloc_hook(nmemb * size, caller);
		if (result)
			memset(result, 0, nmemb * size);
		return (result);
	}
	return (__libc_calloc(nmemb, size));
}

void my_free_hook(void *ptr)
{
	(void)ptr;
	g_malloc_hook_active = 0;
	remove_alloc_list(&g_alloc_list, ptr);
	g_malloc_hook_active = 1;
}


void free(void *ptr)
{
	if (g_malloc_hook_active)
		my_free_hook(ptr);
	__libc_free(ptr);
}
