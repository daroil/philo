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

int	main(int argc, char **argv)
{
	sem_t	*forks;
	pid_t	*philosopher;
	int		i;
	int		number_of_philos;

	forks = sem_open("forks", O_CREAT, 0644, 3);
	printf("argc: %d argv[0]: %s\n",argc, argv[0]);
	number_of_philos = 4;
	philosopher = malloc(sizeof(pid_t) * number_of_philos);
	i = 0;
	while (i < number_of_philos)
	{
		philosopher[i] = fork();
		if (!philosopher[i])
		{
			sem_wait(forks);
			printf("took a fork\n");
			usleep(100000);
			printf("released a fork\n");
			sem_post(forks);
			exit(0);
		}
		i++;
	}
	i = 0;
	while (i < number_of_philos)
	{
		waitpid(philosopher[i], NULL, 0);
		i++;
	}
	sem_unlink("forks");
	sem_close(forks);
	return (0);
}
