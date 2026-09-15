/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   codexion.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pmuzika <pmuzika@student.42malaga.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/06 14:29:14 by pmuzika           #+#    #+#             */
/*   Updated: 2026/09/15 13:37:26 by pmuzika          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CODEXION_H
# define CODEXION_H
# include <pthread.h>
# include <stdio.h>
# include <stdlib.h>
# include <string.h>

typedef struct s_program
{
	int				number_of_coders;
	size_t			time_to_burnout;
	size_t			time_to_compile;
	size_t			time_to_debug;
	size_t			time_to_refactor;
	size_t			start_time;
	int				number_of_compiles_required;
	size_t			dongle_cooldown;
	int				finished_flag;
	pthread_mutex_t	finished_lock;
	pthread_mutex_t	compile_lock; // to protect coder->compiles_achieved
	pthread_mutex_t	write_lock;
	char			scheduler[5]; // "fifo" or "edf"
	t_coder			*coders;
	t_dongle		*dongles;
}					t_program;

typedef struct s_coder
{
	int			id;
	int			compiles_achieved;
	size_t		last_compile_start;
	pthread_t	thread;
	t_dongle	*left_dongle;
	t_dongle	*right_dongle;
	t_program	*program;
}				t_coder;

typedef struct s_dongle
{
	int				id;
	pthread_mutex_t	lock;
	pthread_cond_t	cond;
	t_request		*heap[2];
	size_t			last_released;
}					t_dongle;

typedef struct s_request
{
	t_coder		*coder;
	t_dongle	*dongle;
	size_t		deadline;
	size_t		request_time;
}				t_request;

// utils
int		ft_isdigit(int c);
void	ft_strcpy(char *dst, const char *src);
int		ft_str_is_int(const char *nptr);
int		ft_usleep(size_t milliseconds);
size_t	get_current_time(void);


#endif
