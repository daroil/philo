/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dhendzel <dhendzel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/26 16:58:40 by dhendzel          #+#    #+#             */
/*   Updated: 2023/03/09 23:06:14 by dhendzel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_H
# define PHILO_H

#include <stdio.h>
#include <string.h>
#include <pthread.h>
#include <stdlib.h>
#include <sys/time.h> // gettimeofday
#include <unistd.h> // usleep
#include <stddef.h> // NULL

typedef	struct shared_s
{
	int				number_of_philos;
	pthread_mutex_t	print;
	pthread_mutex_t	time;
	int				dead;
	long long		time_to_die;
	long long		time_to_eat;
	long long		time_to_sleep;
}	t_shared;

typedef struct philo_s
{
	int		input;
	int		output;
	int		philo_id;
	long long		last_meal;
	pthread_mutex_t chopstick_l;
	pthread_mutex_t *chopstick_r;
	pthread_t	id;
	t_shared	*shared;
}	t_philo;

int	check_death(t_philo *philo, long long start_time);


#endif