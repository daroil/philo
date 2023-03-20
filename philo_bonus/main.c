/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dhendzel <dhendzel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/26 16:58:09 by dhendzel          #+#    #+#             */
/*   Updated: 2023/03/20 19:42:15 by dhendzel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	half_asleep(long long sleep_time, t_philo *philo)
{
	long long	i;

	i = get_other_time() + sleep_time;
	while (get_other_time() < i)
	{
		if (!check_death(philo))
			usleep(200);
		else
		{
			return ;
		}
	}
}

void	say(t_philo *philo, char *message)
{
	if (!check_death(philo))
	{
		sem_wait(philo->print);
		if (!philo->dead)
			printf("%d id: %d %s\n", get_other_time(), philo->philo_id, message);
		sem_post(philo->print);
	}
}

void	check_leaks(void)
{
	system("leaks philo");
}

int	main(int argc, char **argv)
{
	
	pid_t	*philosopher;
	int		i;
	int		number_of_philos;
	int		exit_code;
	t_philo	philo;

	number_of_philos = 6;
	philo.time_to_die = 120;
	philo.time_to_eat = 200;
	philo.time_to_sleep = 200;
	philo.dead = 0;
	sem_unlink("forks");
	philo.forks = sem_open("forks", O_CREAT, 0644, number_of_philos);
	sem_unlink("print");
	philo.print = sem_open("print", O_CREAT, 0644, 1);
	printf("argc: %d argv[0]: %s\n",argc, argv[0]);
	philosopher = malloc(sizeof(pid_t) * number_of_philos);
	i = 0;
	get_other_time();
	while (i < number_of_philos)
	{
		philosopher[i] = fork();
		if (!philosopher[i])
		{
			philo.philo_id = i;
			if (i % 2 == 0)
				half_asleep(philo.time_to_eat/2, &philo);
			while (!check_death(&philo))
			{
				sem_wait(philo.forks);
				say(&philo, "took a chopstick");
				if (!check_death(&philo))
				{
					sem_wait(philo.forks);
					say(&philo, "took a chopstick");
					philo.last_meal = get_other_time();
					say(&philo, "is eating");
					half_asleep(philo.time_to_eat, &philo);
					say(&philo, "released a chopstick");
					sem_post(philo.forks);
				}
				sem_post(philo.forks);
				say(&philo, "is sleeping");
				half_asleep(philo.time_to_sleep, &philo);
			}
			exit(1);
		}
		i++;
	}
	i = 0;
	while (i < number_of_philos)
	{
		waitpid(philosopher[i], &exit_code, 0);
		printf("i: %d, exit code: %d\n", i, exit_code);
		if (WEXITSTATUS(exit_code))
		{
			while (i < number_of_philos - 1)
				kill(philosopher[++i], SIGTERM);
			break;
		}
		i++;
	}
	sem_unlink("print");
	sem_close(philo.print);
	sem_unlink("forks");
	sem_close(philo.forks);
	return (0);
}
