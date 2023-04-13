/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asolano- <asolano-@student.42malaga.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/12 10:13:50 by asolano-          #+#    #+#             */
/*   Updated: 2023/03/20 08:45:13 by asolano-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>
#include <stdio.h>
#include "philo.h"

/*
 * This is the main function which makes the program work
 */

int	main(int argc, char **argv)
{
	int		ret; //this will be the return value in case there is an error
	t_rules	rules; //this is the struct which will hold the necessary values for the program to work

	if (argc != 5 && argc != 6) //if there are not 5 or 6 arguments (including "./philo") the program will end showing an error message and an usage guide
	{
		ft_error("Wrong number of arguments"); //this function will show the error message and return 1
		usage(); //this function will show a usage guide message
		return (1);
	}
	ret = init_all(&rules, argv); //ret will get the value returned from init_all, which will be 0 for success or an error value for an error
	if (ret) //if ret is different from 0 we will call the "error_select" function which will show an error message depending on the error given
		return (error_select(ret)); //since this function return 1 we will return it directly
	if (starter(&rules)) //we call the function starter which will do all the process after initializing everything, this function will return 0 for success or another number for error. If it is not 0 we will return an error message
		return (ft_error("There was an error during threads creation")); // if starter does not succeed we return an error message
	return (0); //we finish the program
}
