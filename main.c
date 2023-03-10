/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dhendzel <dhendzel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/26 16:58:09 by dhendzel          #+#    #+#             */
/*   Updated: 2023/03/10 16:09:06 by dhendzel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

long long	get_time(void)
{
	static struct timeval	time;

	gettimeofday(&time, NULL);
	return ((time.tv_usec / 1000 + time.tv_sec * 1000));
}

int	get_other_time(pthread_mutex_t *time_lock)
{
	static long long		start = 0;
	int						res;

	pthread_mutex_lock(time_lock);
	if (start == 0)
	{
		start = get_time();
		pthread_mutex_unlock(time_lock);
		return (0);
	}
	else
	{
		res = (int)(get_time() - start);
		pthread_mutex_unlock(time_lock);
		return (res);
	}
}

void	unlock_and_exit(t_philo *philo)
{
	pthread_mutex_unlock(philo->chopstick_r);
	pthread_mutex_unlock(&philo->chopstick_l);
	pthread_mutex_unlock(&philo->shared->print);
	pthread_mutex_unlock(&philo->shared->time);
	pthread_exit(NULL);	
}

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
			// say(philo, "died");
		philo->shared->dead = 1;
		pthread_mutex_unlock(&philo->shared->dead_mut);
	}
	if (philo->shared->dead)
		return (1);
	return (0);
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
	normal_exit = 100000;
	time_to_die = philo->shared->time_to_die;
	time_to_eat = philo->shared->time_to_eat;	
	time_to_sleep = philo->shared->time_to_sleep;
	philo->last_meal = get_other_time(&philo->shared->time);
	if (philo->philo_id % 2 == 0)
	{
		// usleep((time_to_sleep/2)*1000);
		half_asleep(time_to_eat/2, philo);
	}
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
		// check_death(philo);
	}
	pthread_exit(NULL);
}

void* overseer_routine(void *p)
{
	t_philo		**philo;
	int			i;
	int			number;
	long long	start_time;
	
	start_time = get_time();
	philo = (t_philo **) p;
	i = 0;
	number = (*philo)->shared->number_of_philos;
	while(1)
	{
		i = 0;	
		while (i < number)
		{
			if (check_death(philo[i]))
					unlock_and_exit(philo[i]);
			i++;
		}
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
		if (pthread_mutex_init(&philo[i].eat, NULL))
		{
			printf("\n mutex init failed\n");
			return (1);
		}
		if (pthread_mutex_init(&philo[i].sleep, NULL))
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
	
	shared_info.number_of_philos = 200;
	shared_info.dead = 0;
	shared_info.time_to_die = 420;
	shared_info.time_to_eat = 200;
	shared_info.time_to_sleep = 200;
	if (pthread_mutex_init(&shared_info.print, NULL) != 0)
    {
        printf("\n mutex init failed\n");
        return (1);
    }
	if (pthread_mutex_init(&shared_info.time, NULL) != 0)
    {
        printf("\n mutex init failed\n");
        return (1);
    }
	if (pthread_mutex_init(&shared_info.dead_mut, NULL) != 0)
    {
        printf("\n mutex init failed\n");
        return (1);
    }
	philo = malloc(sizeof(t_philo) * shared_info.number_of_philos);
	if (philo_init(&shared_info, philo))
		return (1);
	// printf("init succes\n");
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
	i = 0;
	while (i < shared_info.number_of_philos)
	{
		pthread_mutex_destroy(&philo[i].chopstick_l);
		i++;
	}
	pthread_mutex_destroy(&shared_info.print);
	pthread_mutex_destroy(&shared_info.time);
	return (0);
}
