// thread used by the timer

#include "tennis.h"
#include "winsuport.h"

void	*timer_functionality()
{
	timer.min = 0;
	timer.sec = 0;
	char strin[71];
	while(!end)
	{
		sleep(1);
		timer.sec++;
		if (timer.sec >= 60)
		{
			timer.min++;
			timer.sec = 0;
		}
		sprintf(strin,"Tecles: \'%c\'-> amunt, \'%c\'-> avall, RETURN-> sortir. Temps: [%.2d:%.2d].",
			TEC_AMUNT, TEC_AVALL, timer.min, timer.sec);
		win_escristr(strin);
	}
	pthread_exit(0);
}
