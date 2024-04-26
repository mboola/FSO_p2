// thread used by the timer

#include "tennis.h"
#include "winsuport.h"

void	*timer_functionality()
{
	char	strin[60];

	timer.min = 0;
	timer.sec = 0;
	while (!start && !creation_failed);
	if (!creation_failed)
	{
		while(!end)
		{
			sleep(1);
			pthread_mutex_lock(&timer_pause_control);
			timer.sec++;
			if (timer.sec >= 60)
			{
				timer.min++;
				timer.sec = 0;
			}
			sprintf(strin,"Temps: [%.2d:%.2d]. Moviments: [%d/%d].", timer.min, timer.sec, moviments, total_moves);
			pthread_mutex_lock(&screen_control); /* tanca semafor */
			win_escristr(strin);
			pthread_mutex_unlock(&screen_control); /* obre semafor */
			pthread_mutex_unlock(&timer_pause_control);
		}
	}
	pthread_exit(0);
}
