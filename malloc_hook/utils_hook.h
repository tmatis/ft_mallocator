/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_hook.h                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmatis <tmatis@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/26 19:42:42 by tmatis            #+#    #+#             */
/*   Updated: 2021/10/27 15:42:28 by tmatis           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef UTILS_HOOK_H
# define UTILS_HOOK_H

typedef struct
{
	const char *dli_fname;
	void *dli_fbase;
	const char *dli_sname;
	void *dli_saddr;
} Dl_info;

int 		dladdr(void *address, Dl_info *dlip);
char const	*get_func_name(void *addr);
int			should_ignore(void *caller);
int 		routes_eq(void *a[], void *b[]);
int 		route_eq_stack(void *a[], void *b[]);
void 		route_copy(void *dst[], void *src[]);

#endif
