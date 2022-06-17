/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tgwin <tgwin@student.21-school.ru>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/14 18:00:42 by tgwin             #+#    #+#             */
/*   Updated: 2022/06/14 18:00:42 by tgwin            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

static int	one_philo(char **argv)
{
	if (argv[1][0] == '-')
		return (ft_puterror("ERROR: wrong num of philo\n"));
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
	int		status;

	i = -1;
	while (++i < info->num_of_philo)
	{
		waitpid(info->philos[i].pid, &status, 0);
		sem_close(info->philos[i].check);
		free(info->philos[i].name);
	}
	free(info->philos);
	sem_close(info->finish);
	sem_close(info->acting);
	sem_close(info->forks);
	sem_close(info->num_of_eat_finish_philo);
}

static void	fork_philos(t_info *info)
{
	int			i;

	gettimeofday(&info->create_at, NULL);
	i = 0;
	while (i < info->num_of_philo)
	{
		info->philos[i].last_time_to_eat = info->create_at;
		info->philos[i].pid = fork();
		if (info->philos[i].pid == 0)
			return (philo(&info->philos[i]));
		else if (info->philos[i].pid < 0)
		{
			ft_puterror("ERROR: fork failed");
			exit(1);
		}
		++i;
	}
}

int	main(int argc, char *argv[])
{
	t_info		info;
	pthread_t	thread;

	if (argc != 5 && argc != 6)
		return (ft_puterror("ERROR: wrong argc\n"));
	if (argv[1][0] == '-' || argv[2][0] == '-' || argv[3][0] == '-'
		|| argv[4][0] == '-' || (argc == 6 && argv[5][0] == '-'))
		return (ft_puterror("ERROR: wrong args\n"));
	if (one_philo(argv))
		return (0);
	memset(&info, 0, sizeof(info));
	if (init(&info, argc, argv))
		return (1);
	if (info.num_of_must_eat != 0)
		pthread_create(&thread, NULL, monitor_each_must_eat, &info);
	pthread_create(&thread, NULL, finish_monitor, &info);
	fork_philos(&info);
	join_and_free_philos(&info);
	return (0);
}
