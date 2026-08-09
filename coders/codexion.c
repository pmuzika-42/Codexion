/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   codexion.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pmuzika <pmuzika@student.42malaga.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/06 14:26:19 by pmuzika           #+#    #+#             */
/*   Updated: 2026/08/08 20:23:27 by pmuzika          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

int	validate_scheduler(char *str)
{
	if (strcmp(str, "edf") != 0 && strcmp(str, "fifo") != 0)
		return (0);
	return (1);
}

int	validate_numerical_args(char **argv)
{
	int	i;

	i = 0;
	while (i < 7)
	{
		if (!ft_str_is_int(argv[i]))
		{
			printf("Error in arg number %d\n", i);
			return (0);
		}
		i++;
	}
	return (1);
}

int	validate_all_args(char **argv)
{
	if (!validate_numerical_args(argv))
	{
		printf("Error in numerical args\n");
		return (0);
	}
	if (!validate_scheduler(argv[7]))
	{
		printf("Error in scheduler. Must be either 'fifo' or 'edf'\n");
		return (0);
	}
	return (1);
}

void	save_args(t_codexion_args **args, char **argv)
{
	(*args)->number_of_coders = atoi(argv[0]);
	(*args)->time_to_burnout = atoi(argv[1]);
	(*args)->time_to_compile = atoi(argv[2]);
	(*args)->time_to_debug = atoi(argv[3]);
	(*args)->time_to_refactor = atoi(argv[4]);
	(*args)->number_of_compiles_required = atoi(argv[5]);
	(*args)->dongle_cooldown = atoi(argv[6]);
	ft_strcpy((*args)->scheduler, argv[7]);
}

// void    print_args(t_codexion_args *args)
// {
//     printf("----- Program arguments -----\n");
//     printf("Number of coders: %d\n", args->number_of_coders);
//     printf("Time to burnout: %d\n", args->time_to_burnout);
//     printf("Time to compile: %d\n", args->time_to_compile);
//     printf("Time to debug: %d\n", args->time_to_debug);
//     printf("Time to refactor: %d\n", args->time_to_refactor);
//     printf("Compiles required: %d\n", args->number_of_compiles_required);
//     printf("Dongle cooldown: %d\n", args->dongle_cooldown);
//     printf("Scheduler: %s\n", args->scheduler);
//     printf("-----------------------------\n");
// }

int	main(int argc, char **argv)
{
	t_codexion_args	*args;

	if (argc != 9)
	{
		printf("Program needs to receive exactly 8 arguments\n");
		return (1);
	}
	argv++;
	if (!validate_all_args(argv))
		return (1);
	args = malloc(sizeof(t_codexion_args));
	if (!args)
		return (-1);
	save_args(&args, argv);
	print_args(args);
	free(args);
	return (0);
}
