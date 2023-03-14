/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   routine.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dhendzel <dhendzel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/14 15:52:14 by dhendzel          #+#    #+#             */
/*   Updated: 2023/03/14 17:08:14 by dhendzel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	*philo_routine(void *p)
{
	t_philo		*philo;
	long long	time_to_eat;
	long long	time_to_sleep;
	int			normal_exit;

	philo = (t_philo *) p;
	normal_exit = philo->shared->to_be_fed;
	time_to_eat = philo->shared->time_to_eat;
	time_to_sleep = philo->shared->time_to_sleep;
	philo->last_meal = get_other_time(&philo->shared->time);
	if (philo->shared->number_of_philos == 1)
		lonely_dude(philo);
	if (philo->philo_id % 2 == 0)
		half_asleep(min(time_to_eat, time_to_sleep) / 2, philo);
	while ((get_other_time(&philo->shared->time)
			- philo->last_meal) <= philo->shared->time_to_die)
	{
		if (philo->meals == normal_exit)
			break ;
		if (!check_death(philo))
		{
			pthread_mutex_lock(&philo->chopstick_l);
			say(philo, "took left chopstick");
			if (!check_death(philo))
			{
				pthread_mutex_lock(philo->chopstick_r);
				say(philo, "took right chopstick");
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
		say(philo, "is sleeping");
		half_asleep(time_to_sleep, philo);
		say(philo, "is thinking");
	}
	if (philo->meals != normal_exit)
	{
		if (!philo->shared->dead)
		{
			pthread_mutex_lock(&philo->shared->print);
			printf("%d id: %d died\n", get_other_time(&philo->shared->time),
				philo->philo_id);
			philo->shared->dead = 1;
			pthread_mutex_unlock(&philo->shared->print);
		}
	}
	pthread_exit(NULL);
}
