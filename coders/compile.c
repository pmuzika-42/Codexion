/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   compile.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pmuzika <pmuzika@student.42malaga.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/14 18:39:20 by pmuzika           #+#    #+#             */
/*   Updated: 2026/09/28 15:06:18 by pmuzika          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

// Try to take dongles or wait

int	attempt_loop(t_coder *coder, t_request request)
{
	int			result;
	t_program	*p;

	p = coder->program;
	result = 0;
	while (!finished_loop(coder))
	{
		result = try_take_dongles(coder, request);
		if (result == 1)
			break ;
		if (result == 2)
		{
			pthread_mutex_unlock(&p->arbiter_lock);
			usleep(1000);
			pthread_mutex_lock(&p->arbiter_lock);
		}
		else
			pthread_cond_wait(&p->arbiter_cond, &p->arbiter_lock);
	}
	return (result);
}

// Request dongles and take them

int	take_dongles(t_coder *coder)
{
	int			result;
	t_request	request;

	pthread_mutex_lock(&coder->program->arbiter_lock);
	request = request_dongles(coder);
	result = 0;
	result = attempt_loop(coder, request);
	if (result == 1)
		pthread_cond_broadcast(&coder->program->arbiter_cond);
	pthread_mutex_unlock(&coder->program->arbiter_lock);
	return (result == 1);
}

// Release both dongles

void	release_dongles(t_coder *coder)
{
	pthread_mutex_unlock(&coder->first_dongle->lock);
	pthread_mutex_unlock(&coder->second_dongle->lock);
	pthread_mutex_lock(&coder->program->arbiter_lock);
	coder->first_dongle->taken = 0;
	coder->second_dongle->taken = 0;
	coder->first_dongle->last_released = get_current_time();
	coder->second_dongle->last_released = get_current_time();
	pthread_cond_broadcast(&coder->program->arbiter_cond);
	pthread_mutex_unlock(&coder->program->arbiter_lock);
}

// take dongles, compile and release dongles

int	compile(t_coder *coder)
{
	if (!take_dongles(coder))
		return (0);
	print_log("has taken a dongle", coder);
	print_log("has taken a dongle", coder);
	print_log("is compiling", coder);
	pthread_mutex_lock(&coder->program->compile_lock);
	coder->last_compile_start = get_current_time();
	coder->compiles_achieved++;
	pthread_mutex_unlock(&coder->program->compile_lock);
	ft_usleep(coder->program->time_to_compile);
	release_dongles(coder);
	return (1);
}
