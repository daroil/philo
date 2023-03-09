/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dhendzel <dhendzel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/26 16:58:09 by dhendzel          #+#    #+#             */
/*   Updated: 2023/03/09 18:44:21 by dhendzel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

long long	get_time(void)
{
	struct timeval	time;

	gettimeofday(&time, NULL);
	return ((time.tv_usec / 1000 + time.tv_sec * 1000));
}

void	unlock_and_exit(t_philo *philo)
{
	pthread_mutex_unlock(philo->chopstick_r);
	pthread_mutex_unlock(&philo->chopstick_l);
	pthread_mutex_unlock(&philo->shared->print);
	pthread_exit(NULL);	
}

void	say(t_philo *philo, char *message, long long start_time)
{
	if (check_death(philo, start_time))
		unlock_and_exit(philo);
	pthread_mutex_lock(&philo->shared->print);
	if (check_death(philo, start_time))
		unlock_and_exit(philo);
	printf("%lld id: %d %s\n", get_time() - start_time, philo->philo_id, message);
	pthread_mutex_unlock(&philo->shared->print);
	if (check_death(philo, start_time))
		unlock_and_exit(philo);
}

int	check_death(t_philo *philo, long long start_time)
{	
	if ((get_time() - philo->last_meal) > philo->shared->time_to_die)
	{
		pthread_mutex_lock(&philo->shared->print);
		if (!philo->shared->dead)
			printf("%lld id: %d died\n", get_time() - start_time, philo->philo_id);
		philo->shared->dead = 1;
		pthread_mutex_unlock(&philo->shared->print);
	}
	if (philo->shared->dead)
		return (1);
	return (0);
}

void	half_asleep(long long sleep_time, t_philo *philo, long long start_time)
{
	long long	i;

	i = 0;
	while (i < sleep_time * 1000)
	{
		usleep(200);
		if (check_death(philo, start_time))
			unlock_and_exit(philo);
		i += 200;
	}
	if (check_death(philo, start_time))
		unlock_and_exit(philo);
}

void* trial_routine(void *p)
{
	t_philo		*philo;
	long long	start_time;
	long long	time_to_die;
	long long	time_to_eat;
	long long	time_to_sleep;
	int			normal_exit;
	int			i;

	philo = (t_philo *) p;
	i = 0;
	normal_exit = 100900000;
	time_to_die = philo->shared->time_to_die;
	time_to_eat = philo->shared->time_to_eat;	
	time_to_sleep = philo->shared->time_to_sleep;	
	start_time = get_time();
	philo->last_meal = get_time();
	if (philo->philo_id % 2 == 1)
	{
		usleep(time_to_die/2);
	}
	while ((get_time() - philo->last_meal) < time_to_die && i < normal_exit)
	{
		if (philo->philo_id % 2 == 1)
		{
			if (check_death(philo, start_time))
				unlock_and_exit(philo);
			pthread_mutex_lock(&philo->chopstick_l);
			say(philo, "took left chopstick", start_time);
			pthread_mutex_lock(philo->chopstick_r);
			say(philo, "took right chopstick", start_time);
		}
		else
		{
			pthread_mutex_lock(philo->chopstick_r);
			say(philo, "took right chopstick", start_time);	
			pthread_mutex_lock(&philo->chopstick_l);
			say(philo, "took left chopstick", start_time);
		}
		if ((get_time() - start_time) > time_to_die)
		{
			if (check_death(philo, start_time))
				unlock_and_exit(philo);
		}
		say(philo, "started eating", start_time);
		philo->last_meal = get_time();
		half_asleep(time_to_eat, philo, start_time);
		say(philo, "finished eating", start_time);
		pthread_mutex_unlock(philo->chopstick_r);
		// say(philo, "lay down right chopstick", start_time);
		pthread_mutex_unlock(&philo->chopstick_l);
		// say(philo, "lay down left chopstick", start_time);
		say(philo, "started sleeping", start_time);
		half_asleep(time_to_sleep, philo, start_time);
		say(philo, "finished sleeping", start_time);
		i++;
	}
	if (i == normal_exit)
		say(philo, "survived", start_time);
	else
	{
		if (check_death(philo, start_time))
				unlock_and_exit(philo);
	}
	pthread_exit(NULL);
}

void* overseer_routine(void *p)
{
	t_philo		**philo;
	int			i;
	int			number;

	philo = (t_philo **) p;
	i = 0;
	number = (*philo)->shared->number_of_philos;
	while (i < number)
	{
		i++;
	}
	pthread_exit(NULL);
}

int	philo_init(t_shared *shared_info, t_philo *philo)
{
	int	i;

	i = 0;
	while (i < shared_info->number_of_philos)
	{
		if (pthread_mutex_init(&philo[i].chopstick_l, NULL))
		{
			printf("\n mutex init failed\n");
			return (1);
		}
		philo[i].philo_id = i;
		philo[i].shared = shared_info;
		philo[i].chopstick_r = &philo[(i + 1) % shared_info->number_of_philos].chopstick_l;
		i++;
	}
	return (0);
}

int main(void)
{
	t_philo		*philo;
	t_shared	shared_info;
	int			i;
	// pthread_t	overseer;
	
	shared_info.number_of_philos = 3;
	shared_info.dead = 0;
	shared_info.time_to_die = 601;
	shared_info.time_to_eat = 200;
	shared_info.time_to_sleep = 200;
	if (pthread_mutex_init(&shared_info.print, NULL) != 0)
    {
        printf("\n mutex init failed\n");
        return (1);
    }
	philo = malloc(sizeof(t_philo) * shared_info.number_of_philos);
	if (philo_init(&shared_info, philo))
		return (1);
	printf("init succes\n");
	i = 0;
	while (i < shared_info.number_of_philos)
	{
		pthread_create(&philo[i].id, NULL, &trial_routine, &philo[i]);
		i++;
	}
	i = 0;
	// pthread_create(&overseer, NULL, &overseer_routine, &philo);
	// pthread_join(overseer, NULL);
    // Wait for routine();
	while (i < shared_info.number_of_philos)
	{
		pthread_join(philo[i].id, NULL);
		i++;
	}
	return (0);
}
