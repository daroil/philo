/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   checks.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dhendzel <dhendzel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/11 01:37:19 by dhendzel          #+#    #+#             */
/*   Updated: 2023/03/21 14:26:17 by dhendzel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

long long	min(long long a, long long b)
{
	if (a > b)
		return (b);
	else
		return (a);
}

// int	check_death_flag(t_philo *philo)
// {
// 	if (philo->shared->dead)
// 	{
// 		return (1);
// 	}
// 	pthread_mutex_unlock(&philo->shared->check_flag_mut);
// 	return (0);
// }

int	check_death(t_philo *philo)
{
	int i;

	if ((get_other_time()
			- philo->last_meal) >= philo->time_to_die)
	{
		if (!philo->dead)
		{
			philo->dead = 1;
			sem_wait(philo->print);	
			printf("%d id: %d died\n", get_other_time(),
				philo->philo_id);
			sem_post(philo->print);
		}
		i = 0;		
		while (i < philo->taken_chops)
		{
			sem_post(philo->chopsticks);
			i++;
		}
		// sem_unlink("forks");
		// sem_close(philo->forks);
		// sem_unlink("print");
		// sem_close(philo->print);
		exit (1);
		return (1);	
	}
	return (0);
}
