// thread used by the timer

#include "tennis.h"
#include "winsuport.h"

void	*timer_functionality()
{
	char	strin[20];

	timer.min = 0;
	timer.sec = 0;
	while (!start && !creation_failed);
	if (!creation_failed)
	{
		while(!end)
		{
			if (!pause_game)
			{
				sleep(1);
				timer.sec++;
				if (timer.sec >= 60)
				{
					timer.min++;
					timer.sec = 0;
				}
				sprintf(strin,"Temps: [%.2d:%.2d].", timer.min, timer.sec);
				pthread_mutex_lock(&screen_control); /* tanca semafor */
				win_escristr(strin);
				pthread_mutex_unlock(&screen_control); /* obre semafor */
			}
		}
	}
	pthread_exit(0);
}
