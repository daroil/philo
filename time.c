/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   time.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dhendzel <dhendzel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/11 01:35:59 by dhendzel          #+#    #+#             */
/*   Updated: 2023/03/11 01:36:23 by dhendzel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

long long	get_time(void)
{
	static struct timeval	time;

	gettimeofday(&time, NULL);
	return ((time.tv_usec / 1000 + time.tv_sec * 1000));
}

int	get_other_time(pthread_mutex_t *time_lock)
{
	static long long		start = 0;
	int						res;

	pthread_mutex_lock(time_lock);
	if (start == 0)
	{
		start = get_time();
		pthread_mutex_unlock(time_lock);
		return (0);
	}
	else
	{
		res = (int)(get_time() - start);
		pthread_mutex_unlock(time_lock);
		return (res);
	}
}