/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pmuzika <pmuzika@student.42malaga.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/15 12:40:47 by pmuzika           #+#    #+#             */
/*   Updated: 2026/09/15 14:24:57 by pmuzika          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

void	init_dongles(t_dongles *dongles, char **argv)
{
	int	i;

	i = 0;
	while (i < atoi(argv[0]))
	{
		dongles[i]->id = i + 1;
		pthread_mutex_init(&dongle[i]->lock, NULL);
		pthread_cond_init(&dongle[i]->cond, NULL);
		dongle[i]->last_released = get_current_time() - atoi(argv[6]);
		i++;
	}
}

void	init_coders(t_coders *coders, t_program *program, t_dongles *dongles, char **argv)
{
	int	i;
	int	nb_of_coders;
	
	nb_of_coders = atoi(argv[0]);
	i = 0;
	while (i < nb_of_coders)
	{
		coders[i]->id = i + 1;
		coders[i]->compiles_achieved = 0;
		coders[i]->left_dongle = &dongles[i];
		if (i == nb_of_coders - 1)
			coders[i]->right_dongle = &dongles[0];
		else
			coders[i]->right_dongle = &dongles[i + 1];
		coders[i]->program = program;
		coders[i]->last_compile_start = coders[i]->program->start_time;
		i++;
	}
}

void	init_program(t_program *program, t_coders *coders, t_dongles *dongles, char **argv)
{
	program->number_of_coders = atoi(argv[0]);
	program->time_to_burnout = atoi(argv[1]);
	program->time_to_compile = atoi(argv[2]);
	program->time_to_debug = atoi(argv[3]);
	program->time_to_refactor = atoi(argv[4]);
	program->number_of_compiles_required = atoi(argv[5]);
	program->dongle_cooldown = atoi(argv[6]);
	ft_strcpy(program->scheduler, argv[7]);
	program->start_time = get_current_time();
	program->finished_flag = 0;
	pthread_mutex_init(&program->finished_lock, NULL);
	pthread_mutex_init(&program->compile_lock, NULL);
	pthread_mutex_init(&program->write_lock, NULL);
	program->coders = coders;
	program->dongles = dongles;
}

void	init_all(t_program *program, t_coder *coders, t_dongle *dongles, char **argv)
{
	init_dongles(dongles, argv);
	init_program(program, coders, dongles, argv);
	init_coders(coders, program, dongles, argv);
}
