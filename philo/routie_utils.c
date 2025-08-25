/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   routie_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: natalieyan <natalieyan@student.42.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/24 23:25:03 by natalieyan        #+#    #+#             */
/*   Updated: 2025/08/25 00:17:29 by natalieyan       ###   ########.fr       */
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
	usleep(500);
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
	usleep(500);
	pthread_mutex_lock(&r->forks[right]);
	safe_print(r, p->id, "has taken a fork");
}
