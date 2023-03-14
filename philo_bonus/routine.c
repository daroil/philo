/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   routine.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dhendzel <dhendzel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/14 15:52:14 by dhendzel          #+#    #+#             */
/*   Updated: 2023/03/14 17:19:54 by dhendzel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	eating_routine(t_philo *philo)
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
			half_asleep(philo->shared->time_to_eat, philo);
		}
		pthread_mutex_unlock(philo->chopstick_r);
	}	
	pthread_mutex_unlock(&philo->chopstick_l);
}

void	dying_scream(t_philo *philo)
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

void	*philo_routine(void *p)
{
	t_philo		*philo;

	philo = (t_philo *) p;
	philo->last_meal = get_other_time(&philo->shared->time);
	if (philo->shared->number_of_philos == 1)
		lonely_dude(philo);
	if (philo->philo_id % 2 == 0)
		half_asleep(min(philo->shared->time_to_eat,
				philo->shared->time_to_sleep) / 2, philo);
	while ((get_other_time(&philo->shared->time)
			- philo->last_meal) <= philo->shared->time_to_die)
	{
		if (philo->meals == philo->shared->to_be_fed)
			break ;
		if (!check_death(philo))
			eating_routine(philo);
		say(philo, "is sleeping");
		half_asleep(philo->shared->time_to_sleep, philo);
		say(philo, "is thinking");
	}
	if (philo->meals != philo->shared->to_be_fed)
		dying_scream(philo);
	pthread_exit(NULL);
}
