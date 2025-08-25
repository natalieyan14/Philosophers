/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   routie_philo.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: natalieyan <natalieyan@student.42.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/25 00:17:06 by natalieyan        #+#    #+#             */
/*   Updated: 2025/08/25 00:17:08 by natalieyan       ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

static void	take_forks(t_philo *p)
{
	t_rules	*r;

	r = p->rules;
	if (r->nb_philos == 1)
	{
		pthread_mutex_lock(&r->forks[0]);
		safe_print(r, p->id, "has taken a fork");
		safe_usleep(p, r->time_to_die + 1);
		pthread_mutex_unlock(&r->forks[0]);
		return ;
	}
	if (p->id % 2 == 0)
		take_even_forks(p);
	else
		take_odd_forks(p);
}

static void	eat_sleep_think(t_philo *p)
{
	t_rules	*r;

	r = p->rules;
	take_forks(p);
	pthread_mutex_lock(&r->meal_mx);
	p->last_meal = timestamp();
	pthread_mutex_unlock(&r->meal_mx);
	safe_print(r, p->id, "is eating");
	safe_usleep(p, r->time_to_eat);
	pthread_mutex_lock(&r->meal_mx);
	p->meals_eaten++;
	pthread_mutex_unlock(&r->meal_mx);
	pthread_mutex_unlock(&r->forks[p->id - 1]);
	pthread_mutex_unlock(&r->forks[p->id % r->nb_philos]);
	safe_print(r, p->id, "is sleeping");
	safe_usleep(p, r->time_to_sleep);
	safe_print(r, p->id, "is thinking");
}

void	handle_one_philo(t_philo *p)
{
	t_rules	*r;

	r = p->rules;
	pthread_mutex_lock(&r->forks[0]);
	safe_print(r, p->id, "has taken a fork");
	safe_usleep(p, r->time_to_die + 1);
	pthread_mutex_unlock(&r->forks[0]);
}

static void	check_meals(t_philo *p)
{
	t_rules	*r;

	r = p->rules;
	if (r->must_eat > 0)
	{
		pthread_mutex_lock(&r->meal_mx);
		if (p->meals_eaten >= r->must_eat)
		{
			pthread_mutex_unlock(&r->meal_mx);
			pthread_mutex_lock(&r->stop_mx);
			r->stop = 1;
			pthread_mutex_unlock(&r->stop_mx);
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
	if (p->id % 2)
		usleep(5000);
	while (!finished(r))
	{
		eat_sleep_think(p);
		check_meals(p);
		if (finished(r))
			break ;
	}
	return (NULL);
}
