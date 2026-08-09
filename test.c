/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pmuzika <pmuzika@student.42malaga.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/29 19:29:57 by pmuzika           #+#    #+#             */
/*   Updated: 2026/08/06 15:11:48 by pmuzika          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

// void	*routine(void *arg)
// {
// 	int	*result;
	
// 	result = malloc(sizeof(int));
// 	sleep(3);
// 	printf("Thread terminado\n");
// 	*result = 42;
// 	return result;
// }

// int	main(void)
// {
// 	pthread_t	thread;
// 	void	*value;

// 	pthread_create(&thread, NULL, routine, NULL);
// 	pthread_join(thread, &value);
// 	printf("Resultado: %d\n", *(int *)value);
// 	free(value);
// 	printf("Main termina\n");
// 	return (0);
// }

int	main(void)
{
	int x;
	
	x = atoi("hola que tal");
	printf("%d\n", x);
	return (0);
}
