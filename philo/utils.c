/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nharutyu <nharutyu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/26 21:11:38 by nharutyu          #+#    #+#             */
/*   Updated: 2025/08/26 21:11:40 by nharutyu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

long	timestamp(void)
{
	struct timeval	tv;

	gettimeofday(&tv, NULL);
	return (tv.tv_sec * 1000 + tv.tv_usec / 1000);
}

void	safe_usleep(t_philo *p, long duration)
{
	long	start;
	long	elapsed;
	long	remaining;

	start = timestamp();
	while (!finished(p->rules))
	{
		elapsed = timestamp() - start;
		remaining = duration - elapsed;
		if (remaining <= 0)
			break ;
		if (remaining > 1000)
			usleep(1000);
		else if (remaining > 100)
			usleep(100);
		else
			usleep(50);
	}
}

void	safe_print(t_rules *r, int id, char *msg)
{
	long	t;

	pthread_mutex_lock(&r->print_mx);
	if (!finished(r))
	{
		t = timestamp() - r->start_ms;
		printf("%ld %d %s\n", t, id, msg);
	}
	pthread_mutex_unlock(&r->print_mx);
}

void	announce_death(t_rules *r, int id)
{
	long	t;

	pthread_mutex_lock(&r->print_mx);
	pthread_mutex_lock(&r->stop_mx);
	if (r->stop == 0)
	{
		r->stop = 1;
		t = timestamp() - r->start_ms;
		printf("%ld %d died\n", t, id);
	}
	pthread_mutex_unlock(&r->stop_mx);
	pthread_mutex_unlock(&r->print_mx);
}
