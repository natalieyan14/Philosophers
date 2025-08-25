/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: natalieyan <natalieyan@student.42.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/26 00:58:58 by natalieyan        #+#    #+#             */
/*   Updated: 2025/08/26 01:03:00 by natalieyan       ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_H
# define PHILO_H

# include <ctype.h>
# include <limits.h>
# include <pthread.h>
# include <stdio.h>
# include <stdlib.h>
# include <sys/time.h>
# include <unistd.h>

typedef struct s_rules	t_rules;

typedef struct s_philo
{
	int					id;
	int					meals_eaten;
	long				last_meal;
	t_rules				*rules;
	pthread_t			thread;
}						t_philo;

typedef struct s_rules
{
	int					nb_philos;
	long				time_to_die;
	long				time_to_eat;
	long				time_to_sleep;
	int					must_eat;
	int					stop;
	long				start_ms;
	t_philo				*philos;
	pthread_mutex_t		*forks;
	pthread_mutex_t		print_mx;
	pthread_mutex_t		stop_mx;
	pthread_mutex_t		meal_mx;
	pthread_t			monitor_thread;
}						t_rules;

int						init_all(t_rules *r, int ac, char **av);
void					destroy_all(t_rules *r);
int						finished(t_rules *r);

long					timestamp(void);
void					safe_usleep(t_philo *p, long duration);
void					safe_print(t_rules *r, int id, char *msg);
void					announce_death(t_rules *r, int id);
void					eat_sleep_think(t_philo *p);

void					handle_one_philo(t_philo *p);
void					*philo_routine(void *arg);

void					*monitor_routine(void *arg);

#endif