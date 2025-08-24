/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   routie_monitor.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: natalieyan <natalieyan@student.42.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/24 03:20:00 by natalieyan        #+#    #+#             */
/*   Updated: 2025/08/24 03:55:00 by natalieyan       ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"
#include <unistd.h>

static int	check_death(t_rules *r, int i)
{
	int	dead;

	dead = 0;
	pthread_mutex_lock(&r->meal_mx);
	if (timestamp() - r->philos[i].last_meal > r->time_to_die)
	{
		safe_print(r, r->philos[i].id, "died");
		pthread_mutex_lock(&r->stop_mx);
		r->stop = 1;
		pthread_mutex_unlock(&r->stop_mx);
		dead = 1;
	}
	pthread_mutex_unlock(&r->meal_mx);
	return (dead);
}

static int	all_done(t_rules *r)
{
	int	i;

	i = 0;
	while (i < r->nb_philos)
	{
		pthread_mutex_lock(&r->meal_mx);
		if (r->philos[i].meals_eaten < r->must_eat)
		{
			pthread_mutex_unlock(&r->meal_mx);
			return (0);
		}
		pthread_mutex_unlock(&r->meal_mx);
		i++;
	}
	return (1);
}

void	*monitor_routine(void *arg)
{
	t_rules	*r;
	int		i;

	r = arg;
	while (!finished(r))
	{
		i = 0;
		while (i < r->nb_philos)
		{
			if (check_death(r, i))
				return (NULL);
			i++;
		}
		if (r->must_eat > 0 && all_done(r))
		{
			pthread_mutex_lock(&r->stop_mx);
			r->stop = 1;
			pthread_mutex_unlock(&r->stop_mx);
		}
		usleep(100);
	}
	return (NULL);
}
