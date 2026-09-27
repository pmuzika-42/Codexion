/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heap.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pmuzika <pmuzika@student.42malaga.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/22 16:21:35 by pmuzika           #+#    #+#             */
/*   Updated: 2026/09/23 22:04:43 by pmuzika          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

// Add a request to program->heap

void	add_request_to_heap(t_request request, t_program *program)
{
	int	i;

	i = program->heap_size;
	program->heap[i] = request;
	program->heap_size++;
	heapify_up(program, i);
}

// Move a request up the heap if necessary

void	heapify_up(t_program *program, int i)
{
	int			parent;
	t_request	temp;
	t_request	*h;

	h = program->heap;
	while (i != 0)
	{
		parent = (i - 1) / 2;
		if (h[i].priority >= h[parent].priority)
			break ;
		temp = h[i];
		h[i] = h[parent];
		h[parent] = temp;
		i = parent;
	}
}

// Move a request down the heap if necessary

void	heapify_down(t_program *program, int i)
{
	int			smallest;
	int			left;
	int			right;
	t_request	temp;
	t_request	*a;

	smallest = i;
	left = 2 * i + 1;
	right = 2 * i + 2;
	a = program->heap;
	if (left < program->heap_size
		&& a[left].priority < a[smallest].priority)
		smallest = left;
	if (right < program->heap_size
		&& a[right].priority < a[smallest].priority)
		smallest = right;
	if (smallest != i)
	{
		temp = a[i];
		a[i] = a[smallest];
		a[smallest] = temp;
		heapify_down(program, smallest);
	}
}

// Remove request from the heap

void	remove_request(t_coder *coder)
{
	int			i;
	t_request	*h;

	h = coder->program->heap;
	i = 0;
	while (i < coder->program->heap_size)
	{
		if (h[i].coder == coder)
			break ;
		i++;
	}
	if (i == coder->program->heap_size)
		return ;
	h[i] = h[coder->program->heap_size - 1];
	coder->program->heap_size--;
	if (i < coder->program->heap_size)
	{
		heapify_up(coder->program, i);
		heapify_down(coder->program, i);
	}
}

// Check if request 'rival' has higher priority than current request

int	priority_request(t_request *rival, t_request *request)
{
	t_coder	*competitor;
	t_coder	*coder;
	size_t	cooldown;

	competitor = rival->coder;
	coder = request->coder;
	cooldown = coder->program->dongle_cooldown;
	if (competitor->id == coder->id)
		return (0);
	if (rival->priority >= request->priority)
		return (0);
	if (competitor->first_dongle->taken || competitor->second_dongle->taken)
		return (0);
	if (!dongle_is_cold(competitor->first_dongle, cooldown))
		return (0);
	if (!dongle_is_cold(competitor->second_dongle, cooldown))
		return (0);
	return (1);
}
