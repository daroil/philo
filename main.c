/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dhendzel <dhendzel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/26 16:58:09 by dhendzel          #+#    #+#             */
/*   Updated: 2023/03/03 19:19:35 by dhendzel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"



// Global variable:

void* foo(void* p)
{
	t_philo *philo;

	philo = (t_philo *) p;
	// Print value received as argument:
	printf("Value recevied as argument in starting routine: ");
	printf("%i\n", philo->input);

	// Return reference to global variable:
	pthread_exit((void *)&philo->output);
}

int main(void)
{
	// int i = 2;
	t_philo philo;
  // Declare variable for thread's ID:
	pthread_t id;
	philo.input = 1;
	philo.output = 2;
	// int j = 1;
  	int* ptr;
	
	pthread_create(&id, NULL, foo, &philo);
    // Wait for foo() and retrieve value in ptr;
	pthread_join(id, (void**)&ptr);
	printf("Value recevied by parent from child: ");
	printf("%i\n", *ptr);
}


// int	main(int argc, char **argv)
// {
// 	int	i;

// 	if (argc && argv)
// 		printf("Succesful start\n");
// 	i = 0;
// 	while (argv[i])
// 	{
// 		printf("input %s\n", argv[i]);
// 		i++;
// 	}
// 	return (0);	
// }
