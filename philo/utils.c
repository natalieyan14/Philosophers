/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: natalieyan <natalieyan@student.42.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/24 02:02:16 by natalieyan        #+#    #+#             */
/*   Updated: 2025/08/24 03:52:57 by natalieyan       ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

long	timestamp(void)
{
	struct timeval	tv;

	gettimeofday(&tv, NULL);
	return ((tv.tv_sec * 1000) + (tv.tv_usec / 1000));
}

void	ft_usleep(long ms)
{
	long	start;

	start = timestamp();
	while (timestamp() - start < ms)
		usleep(100);
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
