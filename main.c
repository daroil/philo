/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dhendzel <dhendzel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/26 16:58:09 by dhendzel          #+#    #+#             */
/*   Updated: 2023/03/11 01:42:31 by dhendzel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	say(t_philo *philo, char *message)
{
	if (!check_death(philo))
	{
		pthread_mutex_lock(&philo->shared->print);
		if (!check_death_flag(philo))
		{
			printf("%d id: %d %s\n", get_other_time(&philo->shared->time), philo->philo_id, message);
		}
		pthread_mutex_unlock(&philo->shared->print);	
	}
}

void	half_asleep(long long sleep_time, t_philo *philo)
{
	long long	i;

	i = get_other_time(&philo->shared->time) + sleep_time;
	pthread_mutex_lock(&philo->sleep);
	while (get_other_time(&philo->shared->time) < i)
	{
		if (!check_death(philo))
			usleep(100);
		else
		{
			pthread_mutex_unlock(&philo->sleep);
			return ;
		}
	}
	pthread_mutex_unlock(&philo->sleep);
}

void* trial_routine(void *p)
{
	t_philo		*philo;
	long long	time_to_die;
	long long	time_to_eat;
	long long	time_to_sleep;
	int			normal_exit;
	int			i;

	philo = (t_philo *) p;
	i = 0;
	normal_exit = 120000;
	time_to_die = philo->shared->time_to_die;
	time_to_eat = philo->shared->time_to_eat;	
	time_to_sleep = philo->shared->time_to_sleep;
	philo->last_meal = get_other_time(&philo->shared->time);
	if (philo->philo_id % 2 == 0)
		half_asleep(time_to_eat/2, philo);
	while ((get_other_time(&philo->shared->time) - philo->last_meal) <= time_to_die && i < normal_exit)
	{
		if (!check_death(philo))
		{
			pthread_mutex_lock(philo->chopstick_r);
			say(philo, "took right chopstick");	
			if (!check_death(philo))
			{
				pthread_mutex_lock(&philo->chopstick_l);
				say(philo, "took left chopstick");
				if (!check_death(philo))
				{
					pthread_mutex_lock(&philo->eat);
					philo->last_meal = get_other_time(&philo->shared->time);
					pthread_mutex_unlock(&philo->eat);
					say(philo, "is eating");
					half_asleep(time_to_eat, philo);
				}	
				pthread_mutex_unlock(&philo->chopstick_l);
			}	
			pthread_mutex_unlock(philo->chopstick_r);
		}
		say(philo, "is sleeping");
		half_asleep(time_to_sleep, philo);
		say(philo, "is thinking");
		if (philo->shared->number_of_philos % 2 != 0)
			half_asleep(time_to_eat/2, philo);
		i++;
	}
	if (i == normal_exit)
		say(philo, "survived");
	else
	{
		if (!philo->shared->dead)
		{
			say(philo, "died");
			philo->shared->dead = 1;
		}
	}
	pthread_exit(NULL);
}

void	check_leaks(void)
{
	system("leaks philo");
}

int main(int argc, char **argv)
{
	t_philo		*philo;
	t_shared	shared_info;
	int			i;

	// atexit(check_leaks);	
	if (parse_input(argc, argv, &shared_info))
		return (printf("Error\n Wrong input"), 1);
	philo = malloc(sizeof(t_philo) * shared_info.number_of_philos);
	if (philo_init(&shared_info, philo))
		return (1);
	i = 0;
	while (i < shared_info.number_of_philos)
	{
		pthread_create(&philo[i].id, NULL, &trial_routine, &philo[i]);
		i++;
	}
	i = 0;
    // Wait for routine();
	while (i < shared_info.number_of_philos)
	{
		pthread_join(philo[i].id, NULL);
		i++;
	}
	i = 0;
	while (i < shared_info.number_of_philos)
	{
		pthread_mutex_destroy(&philo[i].chopstick_l);
		i++;
	}
	pthread_mutex_destroy(&shared_info.print);
	pthread_mutex_destroy(&shared_info.time);
	free(philo);
	return (0);
}
