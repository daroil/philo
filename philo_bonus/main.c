/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dhendzel <dhendzel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/26 16:58:09 by dhendzel          #+#    #+#             */
/*   Updated: 2023/03/21 14:58:02 by dhendzel         ###   ########.fr       */
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
			return ;
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

// void	kill_philos(t_philo *philo, int to_kill, int sig)
// {
// 	static t_philo	*local_philo = NULL;
// 	// int				i;
	
// 	if (!local_philo)
// 		local_philo = philo;
// 	if (to_kill && sig == SIGTERM)
// 	{
// 		local_philo->dead = 1;
// 		// exit(1);
// 	}
// }

// void	die_philo(int sig)
// {
// 	kill_philos(NULL, 1, sig);
// }

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
	philo.taken_chops = 0;
	sem_unlink("chopsticks");
	philo.chopsticks = sem_open("chopsticks", O_CREAT, 0644, number_of_philos);
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
			// kill_philos(&philo, 0, 0);
			// signal(SIGTERM, die_philo);
			philo.philo_id = i;
			if (i % 2 == 0)
				half_asleep(philo.time_to_eat/2, &philo);
			while (!check_death(&philo))
			{
				sem_wait(philo.chopsticks);
				philo.taken_chops++;
				say(&philo, "took a chopstick");
				if (!check_death(&philo))
				{
					sem_wait(philo.chopsticks);
					philo.taken_chops++;
					say(&philo, "took a chopstick");
					philo.last_meal = get_other_time();
					say(&philo, "is eating");
					half_asleep(philo.time_to_eat, &philo);
					sem_post(philo.chopsticks);
					philo.taken_chops--;
				}
				sem_post(philo.chopsticks);
				philo.taken_chops--;
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
			printf("killing children\n");
			// sem_post(philo.print);
			while (i < number_of_philos - 1)
				kill(philosopher[++i], SIGTERM);
			break;
		}
		i++;
	}
	sem_unlink("print");
	sem_close(philo.print);
	sem_unlink("chopsticks");
	sem_close(philo.chopsticks);
	return (0);
}
