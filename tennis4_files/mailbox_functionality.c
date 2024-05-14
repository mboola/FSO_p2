
#include "tennis.h"

static void	print_chars()
{
	int	i;

	for (i = 0; i < l_pal; i++)		/* delete paleta */
	{
		waitS(screen_id_sem);//pthread_mutex_lock(&screen_control); /* tanca semafor */
		win_escricar(paleta.ipo_pf + i, paleta.ipo_pc, paleta.id + '0' + 1, INVERS);
		signalS(screen_id_sem);//pthread_mutex_unlock(&screen_control); /* obre semafor */ 
	}
}

static void	clear_chars()
{
	int	i;

	for (i = 0; i < l_pal; i++)		/* delete paleta */
	{
		waitS(screen_id_sem);//pthread_mutex_lock(&screen_control); /* tanca semafor */
		win_escricar(paleta.ipo_pf + i, paleta.ipo_pc, ' ', NO_INV);
		signalS(screen_id_sem);//pthread_mutex_unlock(&screen_control); /* obre semafor */ 
	}
}

void	*mailbox_functionality()
{
	char	msg[TAM_MAX_MIS];
	char	pal_touched[MAX_PROCS];
	char	has_touched;
	int		i, j;
	char	c;

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
			if (msg[0] > 0 && paleta.ipo_pc == MAX_COL)	// If I have to go to the right and I'm at the right limit
			{
				clear_chars();
				end_proc = 1;
				pthread_exit(0);
			}
			else if (msg[0] < 0 && paleta.ipo_pc == MIN_COL + 1) // If I have to go to the left and I'm at the left limit
			{
				clear_chars();
				end_proc = 1;
				pthread_exit(0);
			}

			// check if all the chars in the direction I must move are empty
			for (i = 0; i < l_pal; i++)
			{
				waitS(screen_id_sem);//pthread_mutex_lock(&screen_control); /* tanca semafor */
				c = win_quincar(paleta.ipo_pf + i, paleta.ipo_pc + msg[0]);
				signalS(screen_id_sem);//pthread_mutex_unlock(&screen_control); /* obre semafor */ 
				for (j = 0; j < n_paletes; j++)
				{
					if (c == j + '0' + 1)
					{
						//fprintf(stderr, "Touched: [%c]\n", c);
						pal_touched[j] = 1;
					}
				}
			}

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
				paleta.ipo_pc += msg[0];	//move the paleta to one direction or the other
				print_chars();
			}
		}
	}
	pthread_exit(0);
}
