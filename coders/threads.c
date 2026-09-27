/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   threads.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pmuzika <pmuzika@student.42malaga.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/14 20:04:51 by pmuzika           #+#    #+#             */
/*   Updated: 2026/09/27 21:16:03 by pmuzika          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

// Checks if the value of finished_flag changed

int	finished_loop(t_coder *coder)
{
	int	finished;

	pthread_mutex_lock(&coder->program->finished_lock);
	finished = coder->program->finished_flag;
	pthread_mutex_unlock(&coder->program->finished_lock);
	return (finished);
}

// Routine of coders threads

void	*coding_routine(void *pointer)
{
	t_coder	*coder;

	coder = (t_coder *)pointer;
	if (coder->program->number_of_coders == 1)
	{
		print_log("has taken a dongle", coder);
		ft_usleep(coder->program->time_to_burnout);
		return (pointer);
	}
	while (!finished_loop(coder))
	{
		compile(coder);
		print_log("is debugging", coder);
		ft_usleep(coder->program->time_to_debug);
		print_log("is refactoring", coder);
		ft_usleep(coder->program->time_to_refactor);
	}
	return (pointer);
}

// Create monitor and coders threads

int	thread_create(t_program *program)
{
	pthread_t	observer;
	int			i;

	if (pthread_create(&observer, NULL, &monitor, program->coders) != 0)
		return (1);
	i = 0;
	while (i < program->number_of_coders)
	{
		if (pthread_create(&program->coders[i].thread, NULL, &coding_routine,
				&program->coders[i]) != 0)
			return (1);
		i++;
	}
	i = 0;
	if (pthread_join(observer, NULL) != 0)
		return (2);
	while (i < program->number_of_coders)
	{
		if (pthread_join(program->coders[i].thread, NULL) != 0)
			return (2);
		i++;
	}
	return (0);
}

// Control output of thread_create function in case of error

void	create_and_control_threads(t_program *program)
{
	int	control;

	control = thread_create(program);
	if (control == 1)
		printf("Thread creation error");
	if (control == 2)
		printf("Thread join error");
}
