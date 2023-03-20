/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dhendzel <dhendzel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/26 16:58:09 by dhendzel          #+#    #+#             */
/*   Updated: 2023/03/20 18:41:34 by dhendzel         ###   ########.fr       */
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

	// atexit(check_leaks);
	number_of_philos = 6;
	philo.time_to_die = 120;
	philo.time_to_eat = 200;
	philo.time_to_sleep = 200;
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
			sem_wait(philo.forks);
			sem_wait(philo.print);
			printf("%d philo %d took a fork\n", get_other_time(), i);
			sem_post(philo.print);
			half_asleep(philo.time_to_eat, &philo);
			sem_wait(philo.print);
			printf("%d philo %d released a fork\n", get_other_time(), i);
			sem_post(philo.print);
			sem_post(philo.forks);
			exit(0);
		}
		i++;
	}
	i = 0;
	while (i < number_of_philos)
	{
		waitpid(philosopher[i],&exit_code, 0);
		printf("i: %d, exit code: %d\n", i, exit_code);
		if (WEXITSTATUS(exit_code))
		{
			sem_wait(philo.print);
			printf("%d philo %d died\n", get_other_time(), i);
			// sem_post(print);
			while (i < number_of_philos - 1)
				kill(philosopher[++i], SIGTERM);
			// break;
		}
		// waitpid(philosopher[i], NULL, 0);
		i++;
	}
	sem_unlink("forks");
	sem_close(philo.forks);
	sem_unlink("print");
	sem_close(philo.print);
	return (0);
}
