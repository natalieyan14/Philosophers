/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: natalieyan <natalieyan@student.42.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/24 02:02:16 by natalieyan        #+#    #+#             */
/*   Updated: 2025/08/24 19:57:08 by natalieyan       ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

long	timestamp(void)
{
	struct timeval	tv;

	gettimeofday(&tv, NULL);
	return ((tv.tv_sec * 1000) + (tv.tv_usec / 1000));
}

void	safe_usleep(t_philo *p, long duration)
{
	long	start;

	start = timestamp();
	while (!finished(p->rules))
	{
		if (timestamp() - start >= duration)
			break ;
		usleep(500);
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
