/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: natalieyan <natalieyan@student.42.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/24 03:10:50 by natalieyan        #+#    #+#             */
/*   Updated: 2025/08/24 03:52:14 by natalieyan       ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	init_all(t_rules *r, int ac, char **av)
{
	int	i;

	r->nb_philos = atoi(av[1]);
	r->time_to_die = atol(av[2]);
	r->time_to_eat = atol(av[3]);
	r->time_to_sleep = atol(av[4]);
	if (ac == 6)
		r->must_eat = atoi(av[5]);
	else
		r->must_eat = -1;
	r->stop = 0;
	r->start_ms = timestamp();
	r->philos = malloc(sizeof(t_philo) * r->nb_philos);
	r->forks = malloc(sizeof(pthread_mutex_t) * r->nb_philos);
	if (!r->philos || !r->forks)
		return (0);
	i = 0;
	while (i < r->nb_philos)
	{
		r->philos[i].id = i + 1;
		r->philos[i].meals_eaten = 0;
		r->philos[i].last_meal = r->start_ms;
		r->philos[i].rules = r;
		pthread_mutex_init(&r->forks[i], NULL);
		i++;
	}
	pthread_mutex_init(&r->print_mx, NULL);
	pthread_mutex_init(&r->stop_mx, NULL);
	pthread_mutex_init(&r->meal_mx, NULL);
	return (1);
}

void	destroy_all(t_rules *r)
{
	int	i;

	i = 0;
	while (i < r->nb_philos)
		pthread_mutex_destroy(&r->forks[i++]);
	pthread_mutex_destroy(&r->print_mx);
	pthread_mutex_destroy(&r->stop_mx);
	pthread_mutex_destroy(&r->meal_mx);
	free(r->forks);
	free(r->philos);
}

int	finished(t_rules *r)
{
	int	val;

	pthread_mutex_lock(&r->stop_mx);
	val = r->stop;
	pthread_mutex_unlock(&r->stop_mx);
	return (val);
}
