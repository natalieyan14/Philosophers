/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   routie_philo.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: natalieyan <natalieyan@student.42.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/24 03:57:28 by natalieyan        #+#    #+#             */
/*   Updated: 2025/08/24 03:57:35 by natalieyan       ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"
#include <unistd.h>

static void	take_forks(t_philo *p)
{
	pthread_mutex_lock(&p->rules->forks[p->id - 1]);
	safe_print(p->rules, p->id, "has taken a fork");
	pthread_mutex_lock(&p->rules->forks[p->id % p->rules->nb_philos]);
	safe_print(p->rules, p->id, "has taken a fork");
}

static void	release_forks(t_philo *p)
{
	pthread_mutex_unlock(&p->rules->forks[p->id - 1]);
	pthread_mutex_unlock(&p->rules->forks[p->id % p->rules->nb_philos]);
}

static void	eat_sleep_think(t_philo *p)
{
	pthread_mutex_lock(&p->rules->meal_mx);
	p->last_meal = timestamp();
	p->meals_eaten++;
	pthread_mutex_unlock(&p->rules->meal_mx);
	safe_print(p->rules, p->id, "is eating");
	ft_usleep(p->rules->time_to_eat);
	release_forks(p);
	safe_print(p->rules, p->id, "is sleeping");
	ft_usleep(p->rules->time_to_sleep);
	safe_print(p->rules, p->id, "is thinking");
}

void	*philo_routine(void *arg)
{
	t_philo	*p;

	p = arg;
	if (p->id % 2 == 0)
		usleep(500);
	while (!finished(p->rules))
	{
		take_forks(p);
		eat_sleep_think(p);
		if (p->rules->must_eat > 0 && p->meals_eaten >= p->rules->must_eat)
			break ;
	}
	return (NULL);
}
