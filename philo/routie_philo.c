/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   routie_philo.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: natalieyan <natalieyan@student.42.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/25 00:17:06 by natalieyan        #+#    #+#             */
/*   Updated: 2025/08/26 01:38:33 by natalieyan       ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   routie_philo.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: natalieyan <natalieyan@student.42.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/25 00:17:06 by natalieyan        #+#    #+#             */
/*   Updated: 2025/08/26 01:36:40 by natalieyan       ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

static void	take_right_forks(t_philo *p, int left, int right)
{
	pthread_mutex_lock(&p->rules->forks[right]);
	safe_print(p->rules, p->id, "has taken a fork");
	pthread_mutex_lock(&p->rules->forks[left]);
	safe_print(p->rules, p->id, "has taken a fork");
}

static void	take_left_forks(t_philo *p, int left, int right)
{
	usleep(100);
	pthread_mutex_lock(&p->rules->forks[left]);
	safe_print(p->rules, p->id, "has taken a fork");
	pthread_mutex_lock(&p->rules->forks[right]);
	safe_print(p->rules, p->id, "has taken a fork");
}

static void	take_forks(t_philo *p)
{
	t_rules	*r;
	int		left;
	int		right;

	r = p->rules;
	if (r->nb_philos == 1)
	{
		handle_one_philo(p);
		return ;
	}
	left = p->id - 1;
	right = p->id % r->nb_philos;
	if (p->id % 2 == 0)
		take_right_forks(p, left, right);
	else
		take_left_forks(p, left, right);
}

void	eat_sleep_think(t_philo *p)
{
	t_rules	*r;
	long	eat_start;
	long	sleep_start;

	r = p->rules;
	take_forks(p);
	if (r->nb_philos == 1)
		return ;
	eat_start = timestamp();
	pthread_mutex_lock(&r->meal_mx);
	p->last_meal = eat_start;
	pthread_mutex_unlock(&r->meal_mx);
	safe_print(r, p->id, "is eating");
	safe_usleep(p, r->time_to_eat - (timestamp() - eat_start));
	pthread_mutex_lock(&r->meal_mx);
	p->meals_eaten++;
	pthread_mutex_unlock(&r->meal_mx);
	pthread_mutex_unlock(&r->forks[p->id - 1]);
	pthread_mutex_unlock(&r->forks[p->id % r->nb_philos]);
	safe_print(r, p->id, "is sleeping");
	sleep_start = timestamp();
	safe_usleep(p, r->time_to_sleep - (timestamp() - sleep_start));
	safe_print(r, p->id, "is thinking");
}

void	handle_one_philo(t_philo *p)
{
	t_rules	*r;

	r = p->rules;
	pthread_mutex_lock(&r->forks[0]);
	safe_print(r, p->id, "has taken a fork");
	safe_usleep(p, r->time_to_die + 10);
	pthread_mutex_unlock(&r->forks[0]);
}
