// thread used by the user

#include "tennis.h"

/* funcio per moure la paleta de l'usuari en funcio de la tecla premuda */
static void	mou_paleta_usuari(int tecla)
{
	char	strin[100];

	pthread_mutex_lock(&screen_control); /* tanca semafor */
	if ((tecla == TEC_AVALL) && (win_quincar(ipu_pf+l_pal,ipu_pc) == ' '))
	{
		win_escricar(ipu_pf,ipu_pc,' ',NO_INV);	   /* esborra primer bloc */
		ipu_pf++;					   /* actualitza posicio */
		win_escricar(ipu_pf+l_pal-1,ipu_pc,'0',INVERS); /* impri. ultim bloc */
		pthread_mutex_unlock(&screen_control); /* obre semafor */ 
		pthread_mutex_lock(&movement_control); /* tanca semafor */
		if (count_moves && moviments > 0)
		{
			moviments--;    /* he fet un moviment de la paleta */
			sprintf(strin,"Temps: [%.2d:%.2d]. Moviments: [%d/%d].", timer.min, timer.sec, moviments, total_moves);
			pthread_mutex_lock(&screen_control); /* tanca semafor */
			win_escristr(strin);
			pthread_mutex_unlock(&screen_control); /* obre semafor */ 
		}
		pthread_mutex_unlock(&movement_control); /* obre semafor */
	}
	else
		pthread_mutex_unlock(&screen_control); /* obre semafor */
	pthread_mutex_lock(&screen_control); /* tanca semafor */
	if ((tecla == TEC_AMUNT) && (win_quincar(ipu_pf-1,ipu_pc) == ' '))
	{
		win_escricar(ipu_pf+l_pal-1,ipu_pc,' ',NO_INV); /* esborra ultim bloc */
		ipu_pf--;					    /* actualitza posicio */
		win_escricar(ipu_pf,ipu_pc,'0',INVERS);	    /* imprimeix primer bloc */
		pthread_mutex_unlock(&screen_control); /* obre semafor */ 
		pthread_mutex_lock(&movement_control); /* tanca semafor */
		if (count_moves && moviments > 0)
		{
			moviments--;    /* he fet un moviment de la paleta */
			sprintf(strin,"Temps: [%.2d:%.2d]. Moviments: [%d/%d].", timer.min, timer.sec, moviments, total_moves);
			pthread_mutex_lock(&screen_control); /* tanca semafor */
			win_escristr(strin);
			pthread_mutex_unlock(&screen_control); /* obre semafor */ 
		}
		pthread_mutex_unlock(&movement_control); /* obre semafor */
	}
	else
		pthread_mutex_unlock(&screen_control); /* obre semafor */
}

void	*user_functionality()
{
	char	pause_control;
	int		i;

	pause_control = 0;
	while (!start && !creation_failed);
	if (!creation_failed)
	{
		while(!end)
		{
			pthread_mutex_lock(&screen_control); /* tanca semafor */
			tecla = win_gettec();
			pthread_mutex_unlock(&screen_control); /* obre semafor */ 
			if (tecla == TEC_ESPAI)
			{
				if (!pause_control)
				{
					pthread_mutex_lock(&timer_pause_control);
					pthread_mutex_lock(&ball_pause_control);
					for (i = 0; i < n_paletes; i++)
						pthread_mutex_lock(&computer_pause_control[i]);
					pause_control = 1;
				}
				else
				{
					pthread_mutex_unlock(&timer_pause_control);
					pthread_mutex_unlock(&ball_pause_control);
					for (i = 0; i < n_paletes; i++)
						pthread_mutex_unlock(&computer_pause_control[i]);
					pause_control = 0;
				}
			}
			else if (tecla != 0 && !pause_control)
			{
				mou_paleta_usuari(tecla);
				win_retard(retard);
			}
		}
	}
	pthread_exit(0);
}
