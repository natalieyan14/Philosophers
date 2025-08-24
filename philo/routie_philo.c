/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   routie_philo.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: natalieyan <natalieyan@student.42.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/24 03:57:28 by natalieyan        #+#    #+#             */
/*   Updated: 2025/08/24 19:57:34 by natalieyan       ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	take_forks(t_philo *p)
{
	int	left;
	int	right;

	left = p->id - 1;
	right = p->id % p->rules->nb_philos;
	if (left < right)
	{
		pthread_mutex_lock(&p->rules->forks[left]);
		safe_print(p->rules, p->id, "has taken a fork");
		pthread_mutex_lock(&p->rules->forks[right]);
		safe_print(p->rules, p->id, "has taken a fork");
	}
	else
	{
		pthread_mutex_lock(&p->rules->forks[right]);
		safe_print(p->rules, p->id, "has taken a fork");
		pthread_mutex_lock(&p->rules->forks[left]);
		safe_print(p->rules, p->id, "has taken a fork");
	}
}

void	release_forks(t_philo *p)
{
	pthread_mutex_unlock(&p->rules->forks[p->id - 1]);
	pthread_mutex_unlock(&p->rules->forks[p->id % p->rules->nb_philos]);
}

void	eat_sleep_think(t_philo *p)
{
	pthread_mutex_lock(&p->rules->meal_mx);
	p->last_meal = timestamp();
	p->meals_eaten++;
	pthread_mutex_unlock(&p->rules->meal_mx);
	safe_print(p->rules, p->id, "is eating");
	safe_usleep(p, p->rules->time_to_eat);
	release_forks(p);
	safe_print(p->rules, p->id, "is sleeping");
	safe_usleep(p, p->rules->time_to_sleep);
	safe_print(p->rules, p->id, "is thinking");
}

void	handle_one_philo(t_philo *p)
{
	pthread_mutex_lock(&p->rules->forks[0]);
	safe_print(p->rules, p->id, "has taken a fork");
	usleep(p->rules->time_to_die * 1000);
	safe_print(p->rules, p->id, "died");
	pthread_mutex_unlock(&p->rules->forks[0]);
}

void	*philo_routine(void *arg)
{
	t_philo	*p;

	p = arg;
	if (p->rules->nb_philos == 1)
	{
		handle_one_philo(p);
		return (NULL);
	}
	if (p->id % 2 == 0)
		safe_usleep(p, p->rules->time_to_eat / 2 + p->id * 5);
	else
		safe_usleep(p, p->id * 5);
	while (!finished(p->rules))
	{
		take_forks(p);
		eat_sleep_think(p);
		if (p->rules->must_eat > 0 && p->meals_eaten >= p->rules->must_eat)
			break ;
	}
	return (NULL);
}
