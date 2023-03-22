/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_init.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dhendzel <dhendzel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/11 01:38:05 by dhendzel          #+#    #+#             */
/*   Updated: 2023/03/22 15:10:16 by dhendzel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	philo_init(t_philo *philo, char **argv, int argc)
{
	if (argc < 5 || argc > 6)
		return (1);
	set_num_values(philo, argv);
	if (check_input(philo))
		return (1);
	if (argv[5])
	{
		philo->to_be_fed = ph_atoi(argv[5]);
		if (philo->to_be_fed < 0)
			return (1);
	}
	else
		philo->to_be_fed = -1;
	philo->taken_chops = 0;
	philo->meals = 0;
	philo->to_die = 0;
	sem_unlink("dead");
	sem_unlink("chopsticks");
	philo->chopsticks = sem_open("chopsticks", O_CREAT,
			0600, philo->number_of_philos);
	sem_unlink("print");
	philo->print = sem_open("print", O_CREAT, 0644, 1);
	return (0);
}

int	ph_atoi(char *str)
{
	int			i;
	int			res;
	int			sign;

	i = 0;
	res = 0;
	sign = 1;
	while (str[i] == ' ' || str[i] == '\n' || str[i] == '\t' || str[i] == '\v'
		|| str[i] == '\r' || str[i] == '\f')
		i++;
	if (str[i] == '-' || str[i] == '+')
		sign += -2 * (str[i++] == '-');
	while (str[i])
	{
		if ('0' <= str[i] && str[i] <= '9')
		{
			res *= 10;
			res += sign * (str[i] - '0');
			i++;
		}
		else
			return (0);
	}
	return (res * (sign == 1));
}

int	check_input(t_philo *philo)
{
	if (philo->time_to_die <= 0
		|| philo->time_to_eat <= 0
		|| philo->time_to_sleep <= 0
		|| philo->number_of_philos <= 0)
		return (1);
	return (0);
}

void	set_num_values(t_philo *philo, char **argv)
{
	philo->number_of_philos = ph_atoi(argv[1]);
	philo->time_to_die = ph_atoi(argv[2]);
	philo->time_to_eat = ph_atoi(argv[3]);
	philo->time_to_sleep = ph_atoi(argv[4]);
}
