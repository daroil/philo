/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dhendzel <dhendzel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/26 16:58:40 by dhendzel          #+#    #+#             */
/*   Updated: 2023/03/22 15:05:05 by dhendzel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_H
# define PHILO_H

# include <stdio.h>
# include <string.h>
# include <pthread.h>
# include <stdlib.h>
# include <semaphore.h>
# include <fcntl.h>
# include <sys/stat.h>
# include <sys/wait.h>
# include <sys/types.h>
# include <signal.h>
# include <sys/time.h> // gettimeofday
# include <unistd.h> // usleep
# include <stddef.h> // NULL

typedef struct philo_s
{
	int				philo_id;
	int				to_die;
	int				number_of_philos;
	int				to_be_fed;
	int				meals;
	int				taken_chops;	
	long long		time_to_die;
	long long		time_to_eat;
	long long		time_to_sleep;
	long long		last_meal;
	sem_t			*chopsticks;
	sem_t			*print;
	sem_t			*dead;
}	t_philo;

//time.c
long long	get_time(void);
int			get_other_time(void);

//parse_init.c
int			philo_init(t_philo *philo, char **argv, int argc);
int			ph_atoi(char *str);
int			check_input(t_philo *philo);
void		set_num_values(t_philo *philo, char **argv);

//checks.c
int			check_death_flag(t_philo *philo);
int			check_death(t_philo *philo);

//
void		*philo_routine(void *p);
long long	min(long long a, long long b);
void		lonely_dude(t_philo *philo);

//routine.c
void		half_asleep(long long sleep_time, t_philo *philo);
void		waiting(t_philo *philo, pid_t *philosopher);
void		say(t_philo *philo, char *message);
void		set_meal(t_philo *philo);

#endif