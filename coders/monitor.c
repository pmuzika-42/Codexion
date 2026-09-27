/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   monitor.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pmuzika <pmuzika@student.42malaga.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/15 17:30:56 by pmuzika           #+#    #+#             */
/*   Updated: 2026/09/23 21:45:40 by pmuzika          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

// Check if any coder burnt out

int	coder_burned_out(t_coder *coders)
{
	int		i;
	size_t	burnout_time;

	burnout_time = coders[0].program->time_to_burnout;
	i = 0;
	while (i < coders[0].program->number_of_coders)
	{
		pthread_mutex_lock(&coders[0].program->compile_lock);
		if (get_current_time() - coders[i].last_compile_start >= burnout_time)
		{
			pthread_mutex_unlock(&coders[0].program->compile_lock);
			print_log("burned out", &coders[i]);
			return (1);
		}
		pthread_mutex_unlock(&coders[0].program->compile_lock);
		i++;
	}
	return (0);
}

// Check if all required compiles have been done

int	all_compiles_achieved(t_coder *coders)
{
	int	i;
	int	compiles_required;

	i = 0;
	compiles_required = coders[0].program->number_of_compiles_required;
	while (i < coders[0].program->number_of_coders)
	{
		pthread_mutex_lock(&coders[0].program->compile_lock);
		if (coders[i].compiles_achieved < compiles_required)
		{
			pthread_mutex_unlock(&coders[0].program->compile_lock);
			return (0);
		}
		pthread_mutex_unlock(&coders[0].program->compile_lock);
		i++;
	}
	return (1);
}

// Monitor thread routine

void	*monitor(void *pointer)
{
	t_coder	*coders;

	coders = (t_coder *)pointer;
	while (1)
	{
		if (coder_burned_out(coders) || all_compiles_achieved(coders))
		{
			pthread_mutex_lock(&coders[0].program->finished_lock);
			coders[0].program->finished_flag = 1;
			pthread_mutex_unlock(&coders[0].program->finished_lock);
			pthread_cond_broadcast(&coders[0].program->arbiter_cond);
			break ;
		}
	}
	return (pointer);
}
