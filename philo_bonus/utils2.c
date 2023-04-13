/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils2.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asolano- <asolano-@student.42malaga.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/17 11:20:06 by asolano-          #+#    #+#             */
/*   Updated: 2023/03/28 09:25:51 by asolano-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	printer(t_rules *rules, int id, char *str)
{
	sem_wait(rules->writing);
	if (!(rules->dead) || rules->nb_philo == 1)
	{
		printf("%lli ", get_time() - rules->first_time);
		printf("%i ", id + 1);
		printf("%s\n", str);
	}
	sem_post(rules->writing);
	return ;
}

int	ft_isdigit(int n)
{
	if (n >= '0' && n <= '9')
		return (1);
	else
		return (0);
}
