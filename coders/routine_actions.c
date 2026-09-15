/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   routine_actions.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pmuzika <pmuzika@student.42malaga.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/14 18:39:20 by pmuzika           #+#    #+#             */
/*   Updated: 2026/09/15 13:04:35 by pmuzika          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

// Debug routine function
void	debug(t_coder *coder)
{
	print_log("is debugging", coder);
	ft_usleep(coder->time_to_debug);
}

// Refactor routine function
void	refactor(t_coder *coder)
{
	print_log("is refactoring", );
	ft_usleep(coder->time_to_refactor);
}

// Compile routine function
void	compile(t_coder *coder)
{
	// send request to take the first dongle (on the left, except if coder->id == n)
	// when allowed = locks the dongle mutex!
	print_log("has taken a dongle", coder);
	// if only one coder in the args, no second dongle: let him die already
	if (coder->program->number_of_coders == 1)
	{
		ft_usleep(coder->program->time_to_burnout);
		pthread_mutex_unlock(dongle->lock);
		return ;
	}
	// send request to take the other dongle
	// when allowed: locks the second mutex
	print_log("has taken a dongle", coder);
	print_log("is compiling", coder);
	// lock mutex compile_lock
	// updates last_compile_start
	// updates compiles achieved
	// unlock compile lock
	ft_usleep(coder->time_to_compile);
	// unlock both dongles
}
