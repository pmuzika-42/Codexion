/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   threads.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pmuzika <pmuzika@student.42malaga.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/14 20:04:51 by pmuzika           #+#    #+#             */
/*   Updated: 2026/09/14 20:07:44 by pmuzika          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

// Checks if the value of finished_flag changed

int	dead_loop(t_coder *coder)
{
	pthread_mutex_lock(coder->program->finished_lock);
	if (coder->program->finished_flag == 1)
		return (pthread_mutex_unlock(coder->program->finished_lock), 1);
	pthread_mutex_unlock(coder->program->finished_lock);
	return (0);
}

