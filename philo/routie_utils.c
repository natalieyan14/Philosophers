/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   routie_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: natalieyan <natalieyan@student.42.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/24 23:25:03 by natalieyan        #+#    #+#             */
/*   Updated: 2025/08/26 01:25:01 by natalieyan       ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	take_even_forks(t_philo *p)
{
	t_rules	*r;
	int		left;
	int		right;

	r = p->rules;
	left = p->id - 1;
	right = p->id % r->nb_philos;
	pthread_mutex_lock(&r->forks[right]);
	safe_print(r, p->id, "has taken a fork");
	pthread_mutex_lock(&r->forks[left]);
	safe_print(r, p->id, "has taken a fork");
}

void	take_odd_forks(t_philo *p)
{
	t_rules	*r;
	int		left;
	int		right;

	r = p->rules;
	left = p->id - 1;
	right = p->id % r->nb_philos;
	pthread_mutex_lock(&r->forks[left]);
	safe_print(r, p->id, "has taken a fork");
	pthread_mutex_lock(&r->forks[right]);
	safe_print(r, p->id, "has taken a fork");
}

void	check_meals(t_philo *p)
{
	t_rules	*r;

	r = p->rules;
	if (r->must_eat > 0)
	{
		pthread_mutex_lock(&r->meal_mx);
		if (p->meals_eaten >= r->must_eat)
		{
			pthread_mutex_unlock(&r->meal_mx);
			return ;
		}
		pthread_mutex_unlock(&r->meal_mx);
	}
}

void	*philo_routine(void *arg)
{
	t_philo	*p;
	t_rules	*r;

	p = (t_philo *)arg;
	r = p->rules;
	if (r->nb_philos == 1)
	{
		handle_one_philo(p);
		return (NULL);
	}
	if (p->id % 2 == 0)
		usleep(1500);
	while (!finished(r))
	{
		eat_sleep_think(p);
		if (finished(r))
			break ;
	}
	return (NULL);
}
