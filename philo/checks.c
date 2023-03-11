/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   checks.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dhendzel <dhendzel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/11 01:37:19 by dhendzel          #+#    #+#             */
/*   Updated: 2023/03/11 01:37:41 by dhendzel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	check_death_flag(t_philo *philo)
{
	if ((get_other_time(&philo->shared->time) - philo->last_meal) >= philo->shared->time_to_die)
	{
		pthread_mutex_lock(&philo->shared->dead_mut);
		if (!philo->shared->dead)
			philo->shared->dead = 1;
		pthread_mutex_unlock(&philo->shared->dead_mut);
	}
	if (philo->shared->dead)
		return (1);
	return (0);
}

int	check_death(t_philo *philo)
{
	if ((get_other_time(&philo->shared->time) - philo->last_meal) >= philo->shared->time_to_die)
	{
		pthread_mutex_lock(&philo->shared->dead_mut);
		if (!philo->shared->dead)
		{
			pthread_mutex_lock(&philo->shared->print);
			printf("%d id: %d died\n", get_other_time(&philo->shared->time), philo->philo_id);
			pthread_mutex_unlock(&philo->shared->print);
		}
		philo->shared->dead = 1;
		pthread_mutex_unlock(&philo->shared->dead_mut);
	}
	if (philo->shared->dead)
		return (1);
	return (0);
}
