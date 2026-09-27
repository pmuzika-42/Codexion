/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pmuzika <pmuzika@student.42malaga.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/15 12:40:47 by username          #+#    #+#             */
/*   Updated: 2026/09/23 21:43:01 by pmuzika          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

// Initialize dongles values

void	init_dongles(t_dongle *dongles, char **argv)
{
	int	i;

	i = 0;
	while (i < atoi(argv[0]))
	{
		dongles[i].id = i + 1;
		dongles[i].taken = 0;
		pthread_mutex_init(&dongles[i].lock, NULL);
		dongles[i].last_released = get_current_time() - atoi(argv[6]);
		i++;
	}
}

// Initialize coders values

void	init_coders(t_coder *coders, t_program *program, t_dongle *dongles,
	char **argv)
{
	int	i;
	int	nb_of_coders;

	nb_of_coders = atoi(argv[0]);
	i = 0;
	while (i < nb_of_coders)
	{
		coders[i].id = i + 1;
		coders[i].compiles_achieved = 0;
		if (i == nb_of_coders - 1)
		{
			coders[i].first_dongle = &dongles[0];
			coders[i].second_dongle = &dongles[i];
		}
		else
		{
			coders[i].first_dongle = &dongles[i];
			coders[i].second_dongle = &dongles[i + 1];
		}
		coders[i].program = program;
		coders[i].last_compile_start = coders[i].program->start_time;
		i++;
	}
}

// Initialize program values

void	init_program(t_program *program, t_coder *coders,
	t_dongle *dongles, char **argv)
{
	program->number_of_coders = atoi(argv[0]);
	program->time_to_burnout = atoi(argv[1]);
	program->time_to_compile = atoi(argv[2]);
	program->time_to_debug = atoi(argv[3]);
	program->time_to_refactor = atoi(argv[4]);
	program->number_of_compiles_required = atoi(argv[5]);
	program->dongle_cooldown = atoi(argv[6]);
	program->start_time = get_current_time();
	program->finished_flag = 0;
	pthread_mutex_init(&program->finished_lock, NULL);
	pthread_mutex_init(&program->compile_lock, NULL);
	pthread_mutex_init(&program->write_lock, NULL);
	pthread_mutex_init(&program->arbiter_lock, NULL);
	pthread_cond_init(&program->arbiter_cond, NULL);
	program->heap_size = 0;
	program->coders = coders;
	program->dongles = dongles;
	if (strcmp(argv[7], "edf") == 0)
		program->scheduler = EDF;
	else
		program->scheduler = FIFO;
}

// Initialize heap values and call other init functions

int	init_all(t_program *program, t_coder *coders,
	t_dongle *dongles, char **argv)
{
	program->heap = NULL;
	program->heap = malloc(sizeof(t_request) * atoi(argv[0]));
	if (!program->heap)
		return (0);
	init_dongles(dongles, argv);
	init_program(program, coders, dongles, argv);
	init_coders(coders, program, dongles, argv);
	return (1);
}
