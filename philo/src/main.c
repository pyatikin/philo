/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: choyda <chyoda@student.21-school.ru>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/03 19:20:49 by choyda            #+#    #+#             */
/*   Updated: 2022/05/04 14:15:47 by choyda           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

static int	one_philo(char **argv)
{
	if (ft_atoi(argv[1]) == 1)
	{
		printf("0 1 has taken a fork\n");
		usleep(ft_atoi(argv[2]) * 1000);
		printf("%s 1 died\n", argv[2]);
		return (1);
	}
	return (0);
}

static void	join_and_free_philos(t_info *info)
{
	int		i;

	i = 0;
	while (i < info->num_of_philo)
	{
		pthread_join(info->philos[i].thread, NULL);
		pthread_mutex_destroy(&info->philos[i++].check_mutex);
	}
	free(info->philos);
	i = 0;
	while (i < info->num_of_philo)
		pthread_mutex_destroy(&info->forks[i++]);
	free(info->forks);
}

static void	create_philos(t_info *info)
{
	int			i;
	pthread_t	thread;

	gettimeofday(&info->create_at, NULL);
	i = 0;
	while (i < info->num_of_philo)
	{
		info->philos[i].last_time_to_eat = info->create_at;
		pthread_create(&info->philos[i].thread, NULL,
			philo, &info->philos[i]);
		pthread_create(&thread, NULL, monitor, &info->philos[i]);
		pthread_detach(thread);
		++i;
	}
	if (info->num_of_must_eat != 0)
	{
		pthread_create(&thread, NULL, monitor_each_must_eat, info);
		pthread_detach(thread);
	}
}

int	main(int argc, char *argv[])
{
	t_info	info;

	if (one_philo(argv))
		return (0);
	memset(&info, 0, sizeof(info));
	if (argc != 5 && argc != 6)
		return (ft_puterror("ERROR: wrong argc\n"));
	if (init(&info, argc, argv))
		return (1);
	create_philos(&info);
	join_and_free_philos(&info);
	return (0);
}