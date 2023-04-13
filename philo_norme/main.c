/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asolano- <asolano-@student.42malaga.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/12 10:13:50 by asolano-          #+#    #+#             */
/*   Updated: 2023/03/24 11:22:19 by asolano-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>
#include <stdio.h>
#include "philo.h"

int	main(int argc, char **argv)
{
	int		ret;
	t_rules	rules;

	if (argc != 5 && argc != 6)
	{
		ft_error("Wrong number of arguments");
		usage();
		return (1);
	}
	ret = init_all(&rules, argv);
	if (ret)
		return (error_select(ret));
	if (starter(&rules))
		return (ft_error("There was an error during threads creation"));
	return (0);
}
