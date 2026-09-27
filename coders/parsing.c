/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pmuzika <pmuzika@student.42malaga.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/15 18:03:34 by pmuzika           #+#    #+#             */
/*   Updated: 2026/09/23 21:14:22 by pmuzika          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

// Check if a string is convertible to int

int	ft_str_is_int(const char *nptr)
{
	int			i;
	long long	sign;
	long long	result;

	i = 0;
	sign = 1;
	result = 0;
	while ((nptr[i] == 32) || (nptr[i] >= 9 && nptr[i] <= 13))
		i++;
	if (nptr[i] == '+' || nptr[i] == '-')
	{
		if (nptr[i] == '-')
			sign = -1;
		i++;
	}
	if (!(ft_isdigit(nptr[i])))
		return (0);
	while (nptr[i] >= '0' && nptr[i] <= '9')
	{
		result = result * 10 + (nptr[i] - '0');
		if (sign * result < -2147483648 || sign * result > 2147483647)
			return (0);
		i++;
	}
	return (nptr[i] == '\0');
}

// Check if parameter scheduler is either "edf" or "fifo"

int	validate_scheduler(char *str)
{
	if (strcmp(str, "edf") != 0 && strcmp(str, "fifo") != 0)
		return (0);
	return (1);
}

// Check if args are numerical

int	validate_numerical_args(char **argv)
{
	int	i;

	i = 0;
	while (i < 6)
	{
		if ((!ft_str_is_int(argv[i])) || atoi(argv[i]) <= 0)
		{
			printf("Invalid arg number %d\n", i);
			return (0);
		}
		i++;
	}
	if ((!ft_str_is_int(argv[6])) || atoi(argv[6]) < 0)
	{
		printf("Invalid arg number %d\n", 6);
		return (0);
	}
	return (1);
}

// Check all args

int	validate_all_args(int argc, char **argv)
{
	if (argc != 8)
	{
		printf("Program needs to receive exactly 8 arguments\n");
		return (0);
	}
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
