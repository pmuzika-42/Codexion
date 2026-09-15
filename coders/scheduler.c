/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   scheduler.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pmuzika <pmuzika@student.42malaga.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/14 19:18:56 by pmuzika           #+#    #+#             */
/*   Updated: 2026/09/14 19:18:56 by pmuzika          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

// request function
void	request_dongle(t_coder *coder, t_dongle *dongle)
{
	t_request	request;

	request->coder = coder;
	request->dongle = dongle;
	request->deadline = coder->last_compile_start + coder->program->time_to_burnout;
	request->request_time = get_current_time();
	// add request to dongle->heap ! How?
}

// scheduler function
// the coder calls it with arg: coder, dongle
// look again how pthread_cond_t do work
void	scheduler(t_coder *coder, t_dongle *dongle)
{
	// if only one request for this dongle, then ok, no choice to make
	// if both requests for a dongle: one has it, the other wait (pthread_cond_t to be activated)
	// do i need to lock this code with a mutex, to avoid that both thread mix in the same request?


}

