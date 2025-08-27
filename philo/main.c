/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nharutyu <nharutyu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/26 21:10:46 by nharutyu          #+#    #+#             */
/*   Updated: 2025/08/26 21:10:49 by nharutyu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

static int	is_valid_number(char *str)
{
	long	n;
	int		i;

	if (!str || !str[0])
		return (0);
	n = 0;
	i = 0;
	if (str[0] == '-')
		return (0);
	if (str[0] == '+')
		i = 1;
	if (str[i] == '0' && str[i + 1] != '\0')
		return (0);
	while (str[i])
	{
		if (!isdigit(str[i]))
			return (0);
		n = n * 10 + (str[i] - '0');
		if (n > INT_MAX)
			return (0);
		i++;
	}
	return (n != 0);
}

static int	validate_args(int ac, char **av)
{
	int	i;

	if (ac != 5 && ac != 6)
		return (printf("Error: invalid number of arguments\n"), 0);
	i = 1;
	while (i < ac)
	{
		if (!is_valid_number(av[i]))
			return (printf("Error: invalid number '%s'\n", av[i]), 0);
		i++;
	}
	return (1);
}

int	main(int ac, char **av)
{
	t_rules	r;
	int		i;

	if (!validate_args(ac, av))
		return (-1);
	if (!init_all(&r, ac, av))
		return (printf("Error: initialization failed\n"), 1);
	i = 0;
	while (i < r.nb_philos)
	{
		if (pthread_create(&r.philos[i].thread, NULL, philo_routine,
				&r.philos[i]) != 0)
			return (printf("Error: thread creation failed\n"), 1);
		i++;
	}
	if (pthread_create(&r.monitor_thread, NULL, monitor_routine, &r) != 0)
		return (printf("Error: monitor creation failed\n"), 1);
	i = 0;
	while (i < r.nb_philos)
		pthread_join(r.philos[i++].thread, NULL);
	pthread_join(r.monitor_thread, NULL);
	destroy_all(&r);
	return (0);
}
