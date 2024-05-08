/***********************************************************/
/*                                                         */
/*                      tennis.h                           */
/*                                                         */
/* 				blablabla			                       */
/*														   */
/***********************************************************/

#include "tennis.h"

static void	remove_mem(t_mem *mem)
{
	elim_mem(mem->moviments_mem);
	elim_mem(mem->creation_failed_mem);
	elim_mem(mem->start_mem);
	elim_mem(mem->end_mem);
	elim_mem(mem->pause_game_mem);
	elim_mem(mem->control_mem);
	elim_mem(mem->count_moves_mem);
	exit (2);
}

static void	mutex_creation_error(char mutex_init, t_mem *mem)
{
	printf("Program initialization failed.\n");
	if (mutex_init == 0)
		remove_mem(mem);
	pthread_mutex_destroy(&screen_control);		// Screen lock destroyed
	mutex_init--;
	if (mutex_init == 0)
		remove_mem(mem);
	pthread_mutex_destroy(&movement_control);	// Movement lock destroyed
	mutex_init--;
	if (mutex_init == 0)
		remove_mem(mem);
	pthread_mutex_destroy(&pause_control);		// Pause lock destroyed
	remove_mem(mem);
}

static void	thread_creation_error(char threads_init, char mutex_init, t_lock_data *lock_data, t_mem *mem)
{
	*(mem->creation_failed_ptr) = 1;
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
	end:
	win_fi();
	mutex_creation_error(mutex_init, mem);
}

void	init_threads(t_lock_data *lock_data, t_mem *mem)
{
	char	mutex_init;
	char	threads_init;

	mutex_init = 0;
	threads_init = 0;

	//Create locks used
	if (pthread_mutex_init(&screen_control, NULL))		// Screen lock initialized
		mutex_creation_error(mutex_init, mem);
	mutex_init++;
	if (pthread_mutex_init(&movement_control, NULL))	// Movement lock initialized
		mutex_creation_error(mutex_init, mem);
	mutex_init++;
	if (pthread_mutex_init(&pause_control, NULL))		// Pause lock initialized
		mutex_creation_error(mutex_init, mem);

	//Create threads used
	if (pthread_create(&(lock_data->id_user), NULL, user_functionality, NULL))
		thread_creation_error(threads_init, mutex_init, lock_data, mem);
	threads_init++;
	if (pthread_create(&(lock_data->id_ball), NULL, ball_functionality, NULL))
		thread_creation_error(threads_init, mutex_init, lock_data, mem);
	threads_init++;
	if (pthread_create(&(lock_data->id_timer), NULL, timer_functionality, NULL))
		thread_creation_error(threads_init, mutex_init, lock_data, mem);
}

void	end_threads(t_lock_data lock_data)
{
	pthread_join(lock_data.id_user, NULL);
	pthread_join(lock_data.id_ball, NULL);
	pthread_join(lock_data.id_timer, NULL);

	pthread_mutex_destroy(&screen_control);		// Screen sempahore destroyed
	pthread_mutex_destroy(&movement_control);	// movement sempahore destroyed
	pthread_mutex_destroy(&pause_control);		// pause semaphore destroyed
	win_fi();
}

void	end_program(int process_created, t_lock_data *lock_data, t_mem *mem)
{
	*(mem->creation_failed_ptr) = 1;

	while (process_created > 0)
	{
		wait(NULL);
		process_created--;
	}
	thread_creation_error(2, 2, lock_data, mem);
}
