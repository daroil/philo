/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dhendzel <dhendzel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/26 16:58:09 by dhendzel          #+#    #+#             */
/*   Updated: 2023/03/22 14:18:41 by dhendzel         ###   ########.fr       */
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
		philo->dead = sem_open("dead", O_CREAT | O_EXCL, 0600, 0);
		if (philo->dead != SEM_FAILED)
		{
			printf("%d id: %d %s\n", get_other_time(), philo->philo_id, message);
			sem_unlink("dead");
			sem_close(philo->dead);
		}
		sem_post(philo->print);
	}
}

void	set_meal(t_philo *philo)
{
		philo->dead = sem_open("dead", O_CREAT | O_EXCL, 0600, 0);
		if (philo->dead != SEM_FAILED)
		{
			philo->last_meal = get_other_time();
			philo->meals++;
			sem_unlink("dead");
			sem_close(philo->dead);
		}
}

int	main(int argc, char **argv)
{
	
	pid_t	*philosopher;
	int		i;
	int		exit_code;
	t_philo	philo;

	if (philo_init(&philo, argv, argc))
		return (printf("Error\nWrong input\n"), 1);
	// printf("%lld time to die", philo.time_to_die);
	philosopher = malloc(sizeof(pid_t) * philo.number_of_philos);
	i = 0;
	get_other_time();
	while (i < philo.number_of_philos)
	{
		philosopher[i] = fork();
		if (!philosopher[i])
		{
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
					set_meal(&philo);
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
			// i = 0;		
			// while (i < philo.taken_chops)
			// {
			// 	sem_post(philo.chopsticks);
			// 	i++;
			// }
			exit(1);
		}
		i++;
	}
	i = 0;
	while (i < philo.number_of_philos)
	{
		waitpid(philosopher[i], &exit_code, 0);
		printf("i: %d, exit code: %d\n", i, exit_code);
		if (WEXITSTATUS(exit_code))
		{
			printf("killing children\n");
			while (i < philo.number_of_philos - 1)
				kill(philosopher[++i], SIGTERM);
			break;
		}
		i++;
	}
	sem_unlink("print");
	sem_close(philo.print);
	sem_unlink("chopsticks");
	sem_close(philo.chopsticks);
	sem_unlink("dead");
	sem_close(philo.dead);
	return (0);
}
