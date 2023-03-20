/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   time.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dhendzel <dhendzel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/20 15:10:30 by dhendzel          #+#    #+#             */
/*   Updated: 2023/03/20 15:12:10 by dhendzel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

long long	get_time(void)
{
	static struct timeval	time;

	gettimeofday(&time, NULL);
	return ((time.tv_usec / 1000 + time.tv_sec * 1000));
}

int	get_other_time(void)
{
	static long long		start = 0;
	int						res;

	if (start == 0)
	{
		start = get_time();
		return (0);
	}
	else
	{
		res = (int)(get_time() - start);
		return (res);
	}
}