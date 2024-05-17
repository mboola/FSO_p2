#include "tennis.h"

static void	destroy_mutex(pthread_mutex_t *mutex_to_destroy, char *value)
{
	if (*value == 0)
		exit (2);
	pthread_mutex_destroy(mutex_to_destroy);	// Lock destroyed
	*value -= 1;
}

static void	mutex_creation_error(char mutex_init)
{
	char	value;
	int		i;

	value = mutex_init;
	printf("Program initialization failed.\n");
	destroy_mutex(&screen_control, &value);		// Screen lock destroyed
	destroy_mutex(&movement_control, &value);	// Movement lock destroyed
	destroy_mutex(&ball_pause_control, &value);	// Ball pause lock destroyed
	for (i = 0; i < n_paletes; i++)
		destroy_mutex(&computer_pause_control[i], &value);	// Computer pause lock destroyed
}

static void	thread_creation_error(char threads_init, t_lock_data *lock_data, char mutex_init)
{
	int	i;

	creation_failed = 1;
	if (threads_init == 0)
		goto end;
	pthread_join(lock_data->id_user, NULL);
	threads_init--;
	if (threads_init == 0)
		goto end;
	pthread_join(lock_data->id_ball, NULL);
	threads_init--;
	if (threads_init == 0)
		goto end;
	pthread_join(lock_data->id_timer, NULL);
	threads_init--;
	if (threads_init == 0)
		goto end;
	i = 0;
	while (threads_init)
	{
		pthread_join(lock_data->id_computer[i], NULL);
		threads_init--;
		i++;
	}
	end:
	win_fi();
	mutex_creation_error(mutex_init);
}

void	init_threads(t_lock_data *lock_data)
{
	int		i;
	char	mutex_init;
	char	threads_init;

	mutex_init = 0;
	threads_init = 0;
	creation_failed = 0;

	//Create locks used
	if (pthread_mutex_init(&screen_control, NULL))		// Screen lock initialized
		mutex_creation_error(mutex_init);
	mutex_init++;
	if (pthread_mutex_init(&movement_control, NULL))	// Movement lock initialized
		mutex_creation_error(mutex_init);
	mutex_init++;
	if (pthread_mutex_init(&ball_pause_control, NULL))
		mutex_creation_error(mutex_init);
	mutex_init++;
	for (i = 0; i < n_paletes; i++)
	{
		if (pthread_mutex_init(&computer_pause_control[i], NULL))
			mutex_creation_error(mutex_init);
		mutex_init++;
	}

	//Create threads used
	if (pthread_create(&(lock_data->id_user), NULL, user_functionality, NULL))
		thread_creation_error(threads_init, lock_data, mutex_init);
	threads_init++;
	if (pthread_create(&(lock_data->id_ball), NULL, ball_functionality, NULL))
		thread_creation_error(threads_init, lock_data, mutex_init);
	threads_init++;
	if (pthread_create(&(lock_data->id_timer), NULL, timer_functionality, NULL))
		thread_creation_error(threads_init, lock_data, mutex_init);
	threads_init++;
	for (i = 0; i < n_paletes; i++)
	{
		if (pthread_create(&(lock_data->id_computer[i]), NULL, system_functionality, ((void*)(&(paletes[i])))))
			thread_creation_error(threads_init, lock_data, mutex_init);
		threads_init++;
	}
}

void	end_threads(t_lock_data lock_data)
{
	int	i;

	pthread_join(lock_data.id_user, NULL);
	pthread_join(lock_data.id_ball, NULL);
	pthread_join(lock_data.id_timer, NULL);
	for (i = 0; i < n_paletes; i++)
		pthread_join(lock_data.id_computer[i], NULL);

	pthread_mutex_destroy(&screen_control);		// Screen sempahore destroyed
	pthread_mutex_destroy(&movement_control);	// movement sempahore destroyed
	pthread_mutex_destroy(&ball_pause_control);		// Pause lock initialized
	for (i = 0; i < n_paletes; i++)
		pthread_mutex_destroy(&computer_pause_control[i]);		// Pause lock initialized
	win_fi();
}
