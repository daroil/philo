/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dhendzel <dhendzel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/26 16:58:09 by dhendzel          #+#    #+#             */
/*   Updated: 2023/03/14 13:54:47 by dhendzel         ###   ########.fr       */
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

void	lonely_dude(t_philo *philo)
{
	if (!check_death(philo))
	{
		pthread_mutex_lock(philo->chopstick_r);
		say(philo, "took right chopstick");	
	}
	pthread_mutex_unlock(philo->chopstick_r);
	half_asleep(philo->shared->time_to_die, philo);
	if (!philo->shared->dead)
	{
		say(philo, "died");
		philo->shared->dead = 1;
	}
	pthread_exit(NULL);
}

long long	min(long long a, long long b)
{
	if (a > b)
		return (b);
	else
		return (a);
}

void* trial_routine(void *p)
{
	t_philo		*philo;
	long long	time_to_die;
	long long	time_to_eat;
	long long	time_to_sleep;
	int			normal_exit;

	philo = (t_philo *) p;
	normal_exit = philo->shared->to_be_fed;
	time_to_die = philo->shared->time_to_die;
	time_to_eat = philo->shared->time_to_eat;	
	time_to_sleep = philo->shared->time_to_sleep;
	philo->last_meal = get_other_time(&philo->shared->time);
	if (philo->shared->number_of_philos == 1)
		lonely_dude(philo);
	if (philo->philo_id % 2 == 0)
		half_asleep(min(time_to_eat, time_to_sleep)/2, philo);
	while ((get_other_time(&philo->shared->time) - philo->last_meal) <= time_to_die)
	{
		if (!check_death(philo))
		{
			pthread_mutex_lock(&philo->chopstick_l);
			say(philo, "took left chopstick");
			if (!check_death(philo))
			{
				pthread_mutex_lock(philo->chopstick_r);
				say(philo, "took left chopstick");
				if (!check_death(philo))
				{
					pthread_mutex_lock(&philo->eat);
					philo->last_meal = get_other_time(&philo->shared->time);
					pthread_mutex_unlock(&philo->eat);
					say(philo, "is eating");
					pthread_mutex_lock(&philo->set_meal);
					philo->meals++;
					pthread_mutex_unlock(&philo->set_meal);
					half_asleep(time_to_eat, philo);
				}
				pthread_mutex_unlock(philo->chopstick_r);
			}	
			pthread_mutex_unlock(&philo->chopstick_l);
		}
		if (philo->meals == normal_exit)
			break ;
		say(philo, "is sleeping");
		half_asleep(time_to_sleep, philo);
		say(philo, "is thinking");
	}
	if (philo->meals != normal_exit)
	{
		if (!philo->shared->dead)
		{
			pthread_mutex_lock(&philo->shared->print);
			printf("%d id: %d died\n", get_other_time(&philo->shared->time), philo->philo_id);
			philo->shared->dead = 1;
			pthread_mutex_unlock(&philo->shared->print);
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
