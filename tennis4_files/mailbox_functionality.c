
#include "tennis.h"

static void	print_chars()
{

}

static void	clear_chars()
{

}

void	*mailbox_functionality()
{
	char	msg[TAM_MAX_MIS];
	char	pal_touched[n_paletes];	//maybe change to n_procs??
	char	has_touched;
	int		i;

	while (!(*shared_mem.start_ptr) && !(*shared_mem.creation_failed_ptr));
	if (!(*shared_mem.creation_failed_ptr))
	{
		while(!(*shared_mem.end_ptr))
		{
			for (i = 0; i < n_paletes; i++)
				pal_touched[i] = 0;
			has_touched = 0;
			receiveM(shared_mem.mailbox_ptr[(paleta.id + 0)], msg);	//wait for a msg to be recived from the ball or from other paletes
			if (end_proc)
				pthread_exit(0);
			if (msg[0] > 0 && n_col == MIN_COL + 1)	//if I have to go to the left and im at limit
			{
				clear_chars();
				end_proc = 1;
				pthread_exit(0);
			}
			else if (msg[0] < 0 && n_col == MAX_COL) //if I have to go to the right and im at limit
			{
				clear_chars();
				end_proc = 1;
				pthread_exit(0);
			}

			// check if all the chars in the direction I must move are empty
			
			for (i = 0; i < n_paletes; i++)
			{
				if (pal_touched[i] == 1)
				{
					has_touched = 1;
					sendM(shared_mem.mailbox_ptr[i], (void *) msg, sizeof(int));
				}
			}
			if (!has_touched)	//if there was not a single paleta touching
			{
				clear_chars();
				n_col += msg[0];	//move the paleta to one direction or the other
				print_chars();
			}
		}
	}
	pthread_exit(0);
}
