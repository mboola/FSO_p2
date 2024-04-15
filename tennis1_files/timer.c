// thread used by the timer

#include "tennis.h"

void	*timer_functionality()
{
	timer.min = 0;
	timer.sec = 0;
	while(!end)
	{
		sleep(1);
		timer.sec++;
		if (timer.sec <= 60)
		{
			timer.min++;
			timer.sec = 0;
		}
	}
	pthread_exit(0);
}
