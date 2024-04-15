// thread used by the timer

#include "tennis.h"

void	*timer_functionality()
{
	crono.min = 0;
	crono.sec = 0;
	while(!end)
	{
		sleep(1);
		crono.sec++;
		if (crono.sec <= 60)
		{
			crono.min++;
			crono.sec = 0;
		}
	}
	pthread_exit(0);
}
