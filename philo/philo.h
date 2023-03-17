/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dhendzel <dhendzel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/26 16:58:40 by dhendzel          #+#    #+#             */
/*   Updated: 2023/03/17 12:58:30 by dhendzel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_H
# define PHILO_H

# include <stdio.h>
# include <string.h>
# include <pthread.h>
# include <stdlib.h>
# include <sys/time.h> // gettimeofday
# include <unistd.h> // usleep
# include <stddef.h> // NULL

typedef struct shared_s
{
	int				number_of_philos;
	pthread_mutex_t	print;
	pthread_mutex_t	time;
	pthread_mutex_t	dead_mut;
	pthread_mutex_t	check_flag_mut;
	int				dead;
	long long		time_to_die;
	long long		time_to_eat;
	long long		time_to_sleep;
	int				to_be_fed;
}	t_shared;

typedef struct philo_s
{
	int				meals;
	int				philo_id;
	long long		last_meal;
	pthread_mutex_t	eat;
	pthread_mutex_t	sleep;
	pthread_mutex_t	set_meal;
	pthread_mutex_t	chopstick_l;
	pthread_mutex_t	*chopstick_r;
	pthread_t		id;
	t_shared		*shared;
}	t_philo;

//time.c
long long	get_time(void);
int			get_other_time(pthread_mutex_t *time_lock);

//parse_init.c
int			philo_init(t_shared *shared_info, t_philo *philo);
int			ph_atoi(char *str);
int			parse_input(int argc, char **argv, t_shared *shared_info);
void		set_num_values(t_shared *shared_info, char **argv);
int			check_input(t_shared *shared_info);

//checks.c
int			check_death_flag(t_philo *philo);
int			check_death(t_philo *philo);
long long	min(long long a, long long b);

//routine.c
void		*philo_routine(void *p);
void		dying_scream(t_philo *philo);
void		lonely_dude(t_philo *philo);

//main.c
void		say(t_philo *philo, char *message);
void		half_asleep(long long sleep_time, t_philo *philo);
void		lonely_dude(t_philo *philo);
void		destruction(t_philo *philo, t_shared *shared_info);

#endif