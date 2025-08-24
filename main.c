/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: natalieyan <natalieyan@student.42.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/24 03:39:38 by natalieyan        #+#    #+#             */
/*   Updated: 2025/08/24 03:53:08 by natalieyan       ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

int	main(int ac, char **av)
{
	t_rules	r;
	int		i;

	if (ac != 5 && ac != 6)
		return (printf("Error!! Invalid number of arguments.\n"));
	if (!init_all(&r, ac, av))
		return (printf("Error!! Initialization failed.\n"));
	i = 0;
	while (i < r.nb_philos)
	{
		pthread_create(&r.philos[i].thread, NULL, philo_routine, &r.philos[i]);
		i++;
	}
	pthread_create(&r.monitor_thread, NULL, monitor_routine, &r);
	i = 0;
	while (i < r.nb_philos)
		pthread_join(r.philos[i++].thread, NULL);
	pthread_join(r.monitor_thread, NULL);
	destroy_all(&r);
	return (0);
}
