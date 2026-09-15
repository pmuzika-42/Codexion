/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_2.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pmuzika <pmuzika@student.42malaga.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/14 19:57:14 by pmuzika           #+#    #+#             */
/*   Updated: 2026/09/14 19:57:14 by pmuzika          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

// print messages function

void	print_log(char *str, t_coder *coder)
{
	size_t	time;

	pthread_mutex_lock(coder->program->write_lock)
	time = get_current_time() - coder->program->start_time;
	if (!deadloop(coder))
		printf("%zu %d %s\n", time, coder->id, str);
	pthread_mutex_unlock(coder->program->write_lock)
}
