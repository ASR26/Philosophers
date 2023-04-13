/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asolano- <asolano-@student.42malaga.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/12 10:13:50 by asolano-          #+#    #+#             */
/*   Updated: 2023/03/30 08:29:02 by asolano-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>
#include <stdio.h>
#include "philo.h"

/*
 * This is the main function which will call all the other ones
 */
int	main(int argc, char **argv)
{
	int		ret; //this value will be the return value
	t_rules	rules; //this variable will hold the parameters needed for the execution

	if (argc != 5 && argc != 6) //if there are not 4 or 5 arguments given returns error message and usage guide
	{
		ft_error("Wrong number of arguments");
		usage();
		return (1);
	}
	ret = init_all(&rules, argv); //we will hold the return value
	if (ret) //if the return value is 1 or 2 it means error so we call error_select function
		return (error_select(ret)); //we return the proper number for the error
	if (starter(&rules)) // we call starter function
		return (ft_error("There was an error during threads creation")); //if starter returns an error we call ft_error to print an error message
	return (0);
}
