/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dhendzel <dhendzel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/26 16:58:40 by dhendzel          #+#    #+#             */
/*   Updated: 2023/03/07 13:08:09 by dhendzel         ###   ########.fr       */
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
}	t_shared;

typedef struct philo_s
{
	int		input;
	int		output;
	int		philo_id;
	pthread_mutex_t left_fork;
	pthread_mutex_t *right_fork;
	pthread_t	id;
	t_shared	*shared;
}	t_philo;




#endif