/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dhendzel <dhendzel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/26 16:58:09 by dhendzel          #+#    #+#             */
/*   Updated: 2023/03/22 15:10:00 by dhendzel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	second_chopstick(t_philo *philo)
{
	sem_wait(philo->chopsticks);
	philo->taken_chops++;
	say(philo, "took a chopstick");
	set_meal(philo);
	say(philo, "is eating");
	half_asleep(philo->time_to_eat, philo);
	sem_post(philo->chopsticks);
	philo->taken_chops--;
}

void	child_actions(t_philo *philo, int i)
{
	philo->philo_id = i;
	if (i % 2 == 0)
		half_asleep(philo->time_to_eat / 2, philo);
	while (!check_death(philo))
	{
		if (philo->meals == philo->to_be_fed)
			break ;
		sem_wait(philo->chopsticks);
		philo->taken_chops++;
		say(philo, "took a chopstick");
		if (!check_death(philo))
			second_chopstick(philo);
		sem_post(philo->chopsticks);
		philo->taken_chops--;
		say(philo, "is sleeping");
		half_asleep(philo->time_to_sleep, philo);
		say(philo, "is thinking");
	}
	exit(1);
}

int	main(int argc, char **argv)
{
	pid_t	*philosopher;
	int		i;
	t_philo	philo;

	if (philo_init(&philo, argv, argc))
		return (printf("Error\nWrong input\n"), 1);
	philosopher = malloc(sizeof(pid_t) * philo.number_of_philos);
	i = 0;
	philo.last_meal = get_other_time();
	while (i < philo.number_of_philos)
	{
		philosopher[i] = fork();
		if (!philosopher[i])
			child_actions(&philo, i);
		i++;
	}
	waiting(&philo, philosopher);
	return (0);
}
