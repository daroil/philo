/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dhendzel <dhendzel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/26 16:58:09 by dhendzel          #+#    #+#             */
/*   Updated: 2023/03/14 17:23:37 by dhendzel         ###   ########.fr       */
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
			printf("%d id: %d %s\n", get_other_time(&philo->shared->time),
				philo->philo_id, message);
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
			usleep(200);
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

void	destruction(t_philo *philo, t_shared *shared_info)
{
	int	i;

	i = 0;
	while (i < shared_info->number_of_philos)
	{
		pthread_mutex_destroy(&philo[i].chopstick_l);
		i++;
	}
	pthread_mutex_destroy(&shared_info->print);
	pthread_mutex_destroy(&shared_info->time);
	free(philo);
}

int	main(int argc, char **argv)
{
	t_philo		*philo;
	t_shared	shared_info;
	int			i;

	if (parse_input(argc, argv, &shared_info))
		return (printf("Error\n Wrong input"), 1);
	philo = malloc(sizeof(t_philo) * shared_info.number_of_philos);
	if (philo_init(&shared_info, philo))
		return (1);
	i = 0;
	while (i < shared_info.number_of_philos)
	{
		pthread_create(&philo[i].id, NULL, &philo_routine, &philo[i]);
		i++;
	}		
	i = 0;
	while (i < shared_info.number_of_philos)
	{
		pthread_join(philo[i].id, NULL);
		i++;
	}
	destruction(philo, &shared_info);
	return (0);
}
