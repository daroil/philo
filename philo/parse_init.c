/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_init.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dhendzel <dhendzel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/11 01:38:05 by dhendzel          #+#    #+#             */
/*   Updated: 2023/03/14 15:46:20 by dhendzel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	philo_init(t_shared *shared_info, t_philo *philo)
{
	int	i;

	i = 0;
	while (i < shared_info->number_of_philos)
	{
		if (pthread_mutex_init(&philo[i].chopstick_l, NULL)
			|| pthread_mutex_init(&philo[i].eat, NULL)
			|| pthread_mutex_init(&philo[i].set_meal, NULL)
			|| pthread_mutex_init(&philo[i].sleep, NULL))
		{
			printf("\n mutex init failed\n");
			return (1);
		}
		philo[i].philo_id = i;
		philo[i].meals = 0;
		philo[i].shared = shared_info;
		philo[i].chopstick_r = &philo[(i + 1)
			% shared_info->number_of_philos].chopstick_l;
		i++;
	}
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

int	check_input(t_shared *shared_info)
{
	if (shared_info->time_to_die <= 0
		|| shared_info->time_to_eat <= 0
		|| shared_info->time_to_sleep <= 0
		|| shared_info->number_of_philos <= 0)
		return (1);
	return (0);
}

void	set_num_values(t_shared *shared_info, char **argv)
{
	shared_info->number_of_philos = ph_atoi(argv[1]);
	shared_info->time_to_die = ph_atoi(argv[2]);
	shared_info->time_to_eat = ph_atoi(argv[3]);
	shared_info->time_to_sleep = ph_atoi(argv[4]);
}

int	parse_input(int argc, char **argv, t_shared *shared_info)
{
	if (argc < 5 || argc > 6)
		return (1);
	set_num_values(shared_info, argv);
	if (check_input(shared_info))
		return (1);
	if (argv[5])
	{
		shared_info->to_be_fed = ph_atoi(argv[5]);
		if (!shared_info->to_be_fed)
			return (1);
	}
	else
		shared_info->to_be_fed = -1;
	shared_info->dead = 0;
	if (pthread_mutex_init(&shared_info->print, NULL)
		|| pthread_mutex_init(&shared_info->time, NULL)
		|| pthread_mutex_init(&shared_info->dead_mut, NULL))
	{
		printf("\n mutex init failed\n");
		return (1);
	}
	return (0);
}
