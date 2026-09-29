/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   codexion.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pmuzika <pmuzika@student.42malaga.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/06 14:29:14 by pmuzika           #+#    #+#             */
/*   Updated: 2026/09/28 15:08:07 by pmuzika          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CODEXION_H
# define CODEXION_H

# include <pthread.h>
# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <unistd.h>
# include <sys/time.h>

typedef struct s_program	t_program;
typedef struct s_coder		t_coder;
typedef struct s_request	t_request;

typedef enum e_scheduler
{
	FIFO,
	EDF
}	t_scheduler;

typedef struct s_dongle
{
	int				id;
	int				taken;
	pthread_mutex_t	lock;
	size_t			last_released;
}					t_dongle;

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
	pthread_mutex_t	compile_lock;
	pthread_mutex_t	write_lock;
	pthread_mutex_t	arbiter_lock;
	pthread_cond_t	arbiter_cond;
	t_scheduler		scheduler;
	t_request		*heap;
	int				heap_size;
	t_coder			*coders;
	t_dongle		*dongles;
}					t_program;

typedef struct s_request
{
	t_coder	*coder;
	size_t	priority;
}			t_request;

typedef struct s_coder
{
	int			id;
	int			compiles_achieved;
	size_t		last_compile_start;
	pthread_t	thread;
	t_dongle	*first_dongle;
	t_dongle	*second_dongle;
	t_program	*program;
}				t_coder;

// main
int			main(int argc, char **argv);
void		print_log(char *str, t_coder *coder);
void		free_program_memory(t_program *program, t_coder *coders,
				t_dongle *dongles);
void		destroy_threads_memory(t_program *program, t_dongle *dongles);
void		destroy_and_free(t_program *program, t_coder *coders,
				t_dongle *dongles);

// utils
int			ft_isdigit(int c);
int			ft_usleep(size_t milliseconds);
size_t		get_current_time(void);

// parsing
int			ft_str_is_int(const char *nptr);
int			validate_scheduler(char *str);
int			validate_numerical_args(char **argv);
int			validate_all_args(int argc, char **argv);

// init
void		init_dongles(t_dongle *dongles, char **argv);
void		init_coders(t_coder *coders, t_program *program,
				t_dongle *dongles, char **argv);
void		init_program(t_program *program, t_coder *coders,
				t_dongle *dongles, char **argv);
int			init_all(t_program *program, t_coder *coders,
				t_dongle *dongles, char **argv);

// threads
int			finished_loop(t_coder *coder);
void		*coding_routine(void *pointer);
int			thread_create(t_program *program);
int			join_threads(t_program *program, pthread_t	*observer, int j);
void		create_and_control_threads(t_program *program);

// monitor
int			coder_burned_out(t_coder *coders);
int			all_compiles_achieved(t_coder *coders);
void		*monitor(void *pointer);

// compile
int			attempt_loop(t_coder *coder, t_request request);
int			take_dongles(t_coder *coder);
void		release_dongle(t_coder *coder);
int			compile(t_coder *coder);

// scheduler
t_request	request_dongles(t_coder *coder);
int			dongle_is_cold(t_dongle *dongle, size_t cooldown);
size_t		deadline(t_coder *coder);
int			rival_has_priority(t_coder *coder, t_request request);
int			try_take_dongles(t_coder *coder, t_request request);

// heap
void		add_request_to_heap(t_request request, t_program *program);
void		heapify_up(t_program *program, int i);
void		heapify_down(t_program *program, int i);
void		remove_request(t_coder *coder);
int			priority_request(t_request *rival, t_request *request);

#endif
