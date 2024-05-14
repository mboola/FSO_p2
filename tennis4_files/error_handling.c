/***********************************************************/
/*                                                         */
/*                      tennis.h                           */
/*                                                         */
/* 				blablabla			                       */
/*														   */
/***********************************************************/

#include "tennis.h"

void	remove_mem(t_mem *mem)
{
	elim_mem(mem->moviments_mem);
	elim_mem(mem->creation_failed_mem);
	elim_mem(mem->start_mem);
	elim_mem(mem->end_mem);
	elim_mem(mem->pause_game_mem);
	elim_mem(mem->control_mem);
	elim_mem(mem->count_moves_mem);
	elim_mem(mem->timer_sec_mem);
	elim_mem(mem->timer_min_mem);
	elim_mem(mem->camp_mem);
	exit (2);
}

static void	thread_creation_error(char threads_init, t_lock_data *lock_data, t_mem *mem)
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
	printf("Program initialization failed.\n");
	remove_mem(mem);
}

void	init_threads(t_lock_data *lock_data, t_mem *mem)
{
	char	threads_init;

	threads_init = 0;
	//Create threads used
	if (pthread_create(&(lock_data->id_user), NULL, user_functionality, NULL))
		thread_creation_error(threads_init, lock_data, mem);
	threads_init++;
	if (pthread_create(&(lock_data->id_ball), NULL, ball_functionality, NULL))
		thread_creation_error(threads_init, lock_data, mem);
	threads_init++;
	if (pthread_create(&(lock_data->id_timer), NULL, timer_functionality, NULL))
		thread_creation_error(threads_init, lock_data, mem);
}

void	end_threads(t_lock_data lock_data)
{
	pthread_join(lock_data.id_user, NULL);
	pthread_join(lock_data.id_ball, NULL);
	pthread_join(lock_data.id_timer, NULL);
}

void	end_program(int process_created, t_lock_data *lock_data, t_mem *mem)
{
	*(mem->creation_failed_ptr) = 1;
	fprintf(stderr, "ERROR.\n");

	while (process_created > 0)
	{
		wait(NULL);
		process_created--;
	}
	thread_creation_error(2, lock_data, mem);
}
