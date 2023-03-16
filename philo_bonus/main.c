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
	t_philo		*philo;
	t_shared	shared_info;
	int			i;

	if (parse_input(argc, argv, &shared_info))
		return (printf("Error\n Wrong input"), 1);
	philo = malloc(sizeof(t_philo) * shared_info.number_of_philos);
	if (philo_init(&shared_info, philo))
		return (1);

	return (0);
}
