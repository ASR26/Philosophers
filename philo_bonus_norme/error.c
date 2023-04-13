/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asolano- <asolano-@student.42malaga.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/16 08:26:20 by asolano-          #+#    #+#             */
/*   Updated: 2023/03/28 08:16:07 by asolano-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	ft_error(char *str)
{
	int	len;

	len = 0;
	while (str[len])
		len++;
	write(2, "Error: ", 7);
	write(2, str, len);
	write(2, "\n", 1);
	return (1);
}

int	error_select(int error)
{
	if (error == 1)
	{
		return (ft_error("One or more arguments are wrong"));
		usage();
	}
	if (error == 2)
		return (ft_error("Fatal error during semaphore initialization"));
	return (1);
}
