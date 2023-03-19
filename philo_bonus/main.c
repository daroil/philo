/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dhendzel <dhendzel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/26 16:58:09 by dhendzel          #+#    #+#             */
/*   Updated: 2023/03/17 14:23:18 by dhendzel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	main(int argc, char **argv)
{
	sem_t	*forks;
	sem_t	*print;
	pid_t	*philosopher;
	int		i;
	int		number_of_philos;
	int		exit_code;

	forks = sem_open("forks", O_CREAT, 0644, 3);
	print = sem_open("death", O_CREAT, 0644, 1);
	printf("argc: %d argv[0]: %s\n",argc, argv[0]);
	number_of_philos = 4;
	philosopher = malloc(sizeof(pid_t) * number_of_philos);
	i = 0;
	while (i < number_of_philos)
	{
		philosopher[i] = fork();
		if (!philosopher[i])
		{
			if (i == 0)
				exit(1);
			sem_wait(forks);
			sem_wait(print);
			printf("took a fork\n");
			sem_post(print);
			usleep(100000);
			sem_wait(print);
			printf("released a fork\n");
			sem_post(print);
			sem_post(forks);
			exit(0);
			
		}
		i++;
	}
	i = 0;
	w1`hile (i < number_of_philos)
	{
		waitpid(philosopher[i],&exit_code, 0);
		printf("i: %d, exit code: %d\n", i, exit_code);
		if (WEXITSTATUS(exit_code))
		{
			sem_wait(print);
			printf("died\n");
			while (i < number_of_philos)
				kill(philosopher[++i], SIGTERM);
			break;
		}
		// waitpid(philosopher[i], NULL, 0);
		i++;
	}
	sem_post(print);
	sem_unlink("forks");
	sem_close(forks);
	return (0);
}
