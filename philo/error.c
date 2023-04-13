/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asolano- <asolano-@student.42malaga.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/16 08:26:20 by asolano-          #+#    #+#             */
/*   Updated: 2023/03/20 08:43:54 by asolano-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

/*
 * This function will print an error message given by "str" and return 1
 */
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

/*
 * This function will choose an error message depending on the error received
 */

int	error_select(int error)
{
	if (error == 1)
	{
		return (ft_error("One or more arguments are wrong"));
		usage();
	}
	if (error == 2)
		return (ft_error("Fatal error during mutex initialization"));
	return (1);
}
