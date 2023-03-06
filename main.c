/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dhendzel <dhendzel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/26 16:58:09 by dhendzel          #+#    #+#             */
/*   Updated: 2023/03/06 18:31:46 by dhendzel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

long long	get_time(void)
{
	struct timeval	time;

	gettimeofday(&time, NULL);
	return ((time.tv_usec / 1000 + time.tv_sec * 1000));
}

void	say(t_philo *philo, char *message, long long start_time)
{
	pthread_mutex_lock(&philo->shared->print);
	printf("%lld id: %d %s\n", get_time() - start_time, philo->philo_id, message);
	pthread_mutex_unlock(&philo->shared->print);
}

void* trial_routine(void *p)
{
	t_philo *philo;

	philo = (t_philo *) p;
	long long	start_time;
	
	start_time = get_time();
	if (philo->philo_id % 2 == 1)
	{
		usleep(10000);
	}
	pthread_mutex_lock(&philo->left_fork);
	say(philo, "took left fork", start_time);
	pthread_mutex_lock(philo->right_fork);
	say(philo, "took right fork", start_time);
	say(philo, "started eating", start_time);
	usleep(10000);
	say(philo, "finished eating", start_time);
	pthread_mutex_unlock(philo->right_fork);
	say(philo, "lay down right fork", start_time);
	pthread_mutex_unlock(&philo->left_fork);
	say(philo, "lay down left fork", start_time);
	pthread_exit(NULL);
}

int main(void)
{
	t_philo		*philo;
	t_shared	shared_info;	
	int			i;
	// Declare variable for thread's ID:
	// pthread_t	*id;
	int			number_of_philos;


	if (pthread_mutex_init(&shared_info.print, NULL) != 0)
    {
        printf("\n mutex init failed\n");
        return (1);
    }
	number_of_philos = 3;
	philo = malloc(sizeof(t_philo) * number_of_philos);
	if (pthread_mutex_init(&philo[0].left_fork, NULL) != 0)
    {
        printf("\n mutex init failed\n");
        return (1);
    }
	if (pthread_mutex_init(&philo[1].left_fork, NULL) != 0)
    {
        printf("\n mutex init failed\n");
        return (1);
    }
	if (pthread_mutex_init(&philo[2].left_fork, NULL) != 0)
    {
        printf("\n mutex init failed\n");
        return (1);
    }
	philo[0].right_fork = &philo[2].left_fork;
	philo[1].right_fork = &philo[0].left_fork;
	philo[2].right_fork = &philo[1].left_fork;
	philo[0].philo_id = 1;
	philo[1].philo_id = 2;
	philo[2].philo_id = 3;
	philo[0].shared = &shared_info;
	philo[1].shared = &shared_info;
	philo[2].shared = &shared_info;
	i = 0;
	while (i < number_of_philos)
	{
		pthread_create(&philo[i].id, NULL, &trial_routine, &philo[i]);
		i++;
	}
	i = 0;
	while (i < number_of_philos)
	{
		pthread_join(philo[i].id, NULL);
		i++;
	}
    // Wait for trial_routine();
	return (0);
}
