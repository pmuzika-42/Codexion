/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   scheduler.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pmuzika <pmuzika@student.42malaga.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/14 19:18:56 by pmuzika           #+#    #+#             */
/*   Updated: 2026/09/27 21:08:47 by pmuzika          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

// Create a request and adds it to the heap

t_request	request_dongles(t_coder *coder)
{
	t_request	new_request;

	new_request.coder = coder;
	if (coder->program->scheduler == EDF)
		new_request.priority = deadline(coder);
	else
		new_request.priority = get_current_time();
	add_request_to_heap(new_request, coder->program);
	return (new_request);
}

// check if dongle is cold

int	dongle_is_cold(t_dongle *dongle, size_t cooldown)
{
	if (get_current_time() - dongle->last_released >= cooldown)
		return (1);
	else
		return (0);
}

// Return deadline of coder

size_t	deadline(t_coder *coder)
{
	size_t	time_to_burnout;

	time_to_burnout = coder->program->time_to_burnout;
	return (coder->last_compile_start + time_to_burnout);
}

// Look for request in heap with priority

int	rival_has_priority(t_coder *coder, t_request request)
{
	int			i;
	t_request	rival;

	i = 0;
	while (i < coder->program->heap_size)
	{
		rival = coder->program->heap[i];
		if (priority_request(&rival, &request))
			return (1);
		i++;
	}
	return (0);
}

// Take both dongles if allowed by scheduler

int	try_take_dongles(t_coder *coder, t_request request)
{
	size_t	cooldown;

	cooldown = coder->program->dongle_cooldown;
	if (rival_has_priority(coder, request))
		return (0);
	if (coder->first_dongle->taken || coder->second_dongle->taken)
		return (0);
	if (!dongle_is_cold(coder->first_dongle, cooldown)
		|| !dongle_is_cold(coder->second_dongle, cooldown))
		return (2);
	remove_request(coder);
	pthread_mutex_lock(&coder->first_dongle->lock);
	pthread_mutex_lock(&coder->second_dongle->lock);
	coder->first_dongle->taken = 1;
	coder->second_dongle->taken = 1;
	return (1);
}
