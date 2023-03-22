/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   routine.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dhendzel <dhendzel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/22 14:57:41 by dhendzel          #+#    #+#             */
/*   Updated: 2023/03/22 15:04:07 by dhendzel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	say(t_philo *philo, char *message)
{
	if (!check_death(philo))
	{
		sem_wait(philo->print);
		philo->dead = sem_open("dead", O_CREAT | O_EXCL, 0600, 0);
		if (philo->dead != SEM_FAILED)
		{
			printf("%d id: %d %s\n", get_other_time(), philo->philo_id, message);
			sem_unlink("dead");
			sem_close(philo->dead);
		}
		else
			philo->to_die = 1;
		sem_post(philo->print);
	}
}

void	set_meal(t_philo *philo)
{
	philo->last_meal = get_other_time();
	philo->meals++;
}

void	waiting(t_philo *philo, pid_t *philosopher)
{
	int	i;	

	i = 0;
	while (i < philo->number_of_philos)
	{
		waitpid(philosopher[i], NULL, 0);
		i++;
	}
	sem_unlink("print");
	sem_close(philo->print);
	sem_unlink("chopsticks");
	sem_close(philo->chopsticks);
	sem_unlink("dead");
	sem_close(philo->dead);
}

void	half_asleep(long long sleep_time, t_philo *philo)
{
	long long	i;

	i = get_other_time() + sleep_time;
	while (get_other_time() < i)
	{
		if (!check_death(philo))
			usleep(200);
		else
			return ;
	}
}
