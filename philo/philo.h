/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asolano- <asolano-@student.42malaga.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/16 08:14:31 by asolano-          #+#    #+#             */
/*   Updated: 2023/03/23 08:28:37 by asolano-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_H
# define PHILO_H
# define ERROR_MUTEX	2

# include <sys/time.h>
# include <unistd.h>
# include <stdio.h>
# include <string.h>
# include <stdlib.h>
# include <pthread.h>

struct	s_rules;

typedef struct s_philo
{
	int				id;
	int				nb_meals;
	int				left_fork_id;
	int				right_fork_id;
	long long		t_last_meal;
	struct s_rules	*rules;
	pthread_t		thread_id;
}				t_philo;

typedef struct s_rules
{
	int				nb_philo;
	int				tt_die;
	int				tt_eat;
	int				tt_sleep;
	int				nb_eat;
	int				dead;
	int				all_eaten;
	long long		first_time;
	pthread_mutex_t	meal_check;
	pthread_mutex_t	forks[200];
	pthread_mutex_t	writing;
	t_philo			philo[200];
}					t_rules;

int			ft_error(char *str);
int			error_select(int error);

int			init_mutex(t_rules *r);
int			init_philo(t_rules *r);
int			init_all(t_rules *r, char **argv);

void		philo_eating(t_philo *p);
void		*p_thread(void *v_philo);
void		exit_starter(t_rules *r, t_philo *p);
void		death_checker(t_rules *r, t_philo *p);
int			starter(t_rules *r);

void		usage(void);
long long	get_time(void);
long long	time_passed(long long past, long long current);
void		smart_sleep(long long time, t_rules *r);
int			ft_atoi(const char *str);
void		printer(t_rules *r, int id, char *str);
int			ft_isdigit(int n);
#endif
