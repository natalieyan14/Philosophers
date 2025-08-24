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
			pthread_mutex_lock(&r->meal_mx);
			if (timestamp() - r->philos[i].last_meal > r->time_to_die)
			{
				pthread_mutex_lock(&r->stop_mx);
				r->stop = 1;
				pthread_mutex_unlock(&r->stop_mx);
				safe_print(r, r->philos[i].id, "died");
				pthread_mutex_unlock(&r->meal_mx);
				return (NULL);
			}
			pthread_mutex_unlock(&r->meal_mx);
			i++;
		}
		usleep(1000);
	}
	return (NULL);
}
