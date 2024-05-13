// thread used by the timer

#include "tennis.h"

void	*timer_functionality()
{
	char	strin[60];

	*shared_mem.timer_min_ptr = 0;
	*shared_mem.timer_sec_ptr = 0;
	while (!(*shared_mem.start_ptr) && !(*shared_mem.creation_failed_ptr));
	if (!(*shared_mem.creation_failed_ptr))
	{
		while(!(*shared_mem.end_ptr))
		{
			sleep(1);
			(*shared_mem.timer_sec_ptr)++;
			if (*shared_mem.timer_sec_ptr >= 60)
			{
				(*shared_mem.timer_min_ptr)++;
				*shared_mem.timer_sec_ptr = 0;
			}
			sprintf(strin,"Temps: [%.2d:%.2d]. Moviments: [%d/%d].", *shared_mem.timer_min_ptr, *shared_mem.timer_sec_ptr, *(shared_mem.moviments_ptr), total_moves);
			waitS(screen_id_sem);//pthread_mutex_lock(&screen_control); /* tanca semafor */
			win_escristr(strin);
			signalS(screen_id_sem);//pthread_mutex_unlock(&screen_control); /* obre semafor */ 
			//pthread_mutex_lock(&pause_control);
			//pthread_mutex_unlock(&pause_control);
		}
	}
	pthread_exit(0);
}
