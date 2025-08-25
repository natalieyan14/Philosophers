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

static int	everyone_ate(t_rules *r)
{
	int	i;
	int	all;

	if (r->must_eat <= 0)
		return (0);
	all = 1;
	pthread_mutex_lock(&r->meal_mx);
	i = 0;
	while (i < r->nb_philos)
	{
		if (r->philos[i].meals_eaten < r->must_eat)
		{
			all = 0;
			break ;
		}
		i++;
	}
	pthread_mutex_unlock(&r->meal_mx);
	return (all);
}

static int	check_death(t_rules *r, int i)
{
	long	now;
	long	last;

	pthread_mutex_lock(&r->meal_mx);
	last = r->philos[i].last_meal;
	pthread_mutex_unlock(&r->meal_mx);
	now = timestamp();
	if (now - last >= r->time_to_die)
	{
		announce_death(r, r->philos[i].id);
		return (1);
	}
	return (0);
}

static int	all_ate(t_rules *r)
{
	if (everyone_ate(r))
	{
		pthread_mutex_lock(&r->stop_mx);
		r->stop = 1;
		pthread_mutex_unlock(&r->stop_mx);
		return (1);
	}
	return (0);
}

void	*monitor_routine(void *arg)
{
	t_rules	*r;
	int		i;

	r = (t_rules *)arg;
	while (!finished(r))
	{
		i = 0;
		while (i < r->nb_philos && !finished(r))
		{
			if (check_death(r, i))
				return (NULL);
			i++;
		}
		if (all_ate(r))
			return (NULL);
		usleep(500);
	}
	return (NULL);
}
