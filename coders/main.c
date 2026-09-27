/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pmuzika <pmuzika@student.42malaga.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/06 14:26:19 by pmuzika           #+#    #+#             */
/*   Updated: 2026/09/23 21:40:55 by pmuzika          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

// print messages

void	print_log(char *str, t_coder *coder)
{
	size_t	time;

	pthread_mutex_lock(&coder->program->write_lock);
	time = get_current_time() - coder->program->start_time;
	if (!finished_loop(coder))
		printf("%zu %d %s\n", time, coder->id, str);
	pthread_mutex_unlock(&coder->program->write_lock);
}

// free dynamically allocated memory

void	free_program_memory(t_program *program, t_coder *coders,
			t_dongle *dongles)
{
	free(coders);
	free(dongles);
	free(program->heap);
}

// destroy pthread_mutex and pthread_cond

void	destroy_threads_memory(t_program *program, t_dongle *dongles)
{
	int	i;

	i = 0;
	while (i < program->number_of_coders)
	{
		pthread_mutex_destroy(&dongles[i].lock);
		i++;
	}
	pthread_mutex_destroy(&program->finished_lock);
	pthread_mutex_destroy(&program->compile_lock);
	pthread_mutex_destroy(&program->write_lock);
	pthread_mutex_destroy(&program->arbiter_lock);
	pthread_cond_destroy(&program->arbiter_cond);
}

// clean the whole memory 

void	destroy_and_free(t_program *program, t_coder *coders, t_dongle *dongles)
{
	destroy_threads_memory(program, dongles);
	free_program_memory(program, coders, dongles);
}

// Main function

int	main(int argc, char **argv)
{
	t_program	program;
	t_coder		*coders;
	t_dongle	*dongles;

	argv++;
	if (!validate_all_args(argc - 1, argv))
		return (1);
	coders = malloc(sizeof(t_coder) * atoi(argv[0]));
	if (!coders)
		return (1);
	dongles = malloc(sizeof(t_dongle) * atoi(argv[0]));
	if (!dongles)
	{
		free(coders);
		return (1);
	}
	if (!init_all(&program, coders, dongles, argv))
	{
		free_program_memory(&program, coders, dongles);
		return (1);
	}
	create_and_control_threads(&program);
	destroy_and_free(&program, coders, dongles);
	return (0);
}
