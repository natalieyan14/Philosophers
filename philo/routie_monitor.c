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

static int	check_death(t_rules *r, int i)
{
	long	now;
	long	last;

	pthread_mutex_lock(&r->meal_mx);
	last = r->philos[i].last_meal;
	pthread_mutex_unlock(&r->meal_mx);
	now = timestamp();
	if (now - last > r->time_to_die)
	{
		announce_death(r, r->philos[i].id);
		return (1);
	}
	return (0);
}

static int	all_ate(t_rules *r)
{
	int	i;
	int	all_ate_count;

	if (r->must_eat <= 0)
		return (0);
	all_ate_count = 0;
	pthread_mutex_lock(&r->meal_mx);
	i = 0;
	while (i < r->nb_philos)
	{
		if (r->philos[i].meals_eaten >= r->must_eat)
			all_ate_count++;
		i++;
	}
	pthread_mutex_unlock(&r->meal_mx);
	return (all_ate_count == r->nb_philos);
}

void	*monitor_routine(void *arg)
{
	t_rules	*r;
	int		i;

	r = (t_rules *)arg;
	usleep(1000);
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
		{
			pthread_mutex_lock(&r->stop_mx);
			r->stop = 1;
			pthread_mutex_unlock(&r->stop_mx);
			return (NULL);
		}
		usleep(300);
	}
	return (NULL);
}
