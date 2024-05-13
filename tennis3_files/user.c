// thread used by the user

#include "tennis.h"

/* funcio per moure la paleta de l'usuari en funcio de la tecla premuda */
static void	mou_paleta_usuari(int tecla)
{
	char	strin[100];

	//pthread_mutex_lock(&screen_control); /* tanca semafor */
	if ((tecla == TEC_AVALL) && (win_quincar(ipu_pf+l_pal,ipu_pc) == ' '))
	{
		win_escricar(ipu_pf,ipu_pc,' ',NO_INV);	   /* esborra primer bloc */
		ipu_pf++;					   /* actualitza posicio */
		win_escricar(ipu_pf+l_pal-1,ipu_pc,'0',INVERS); /* impri. ultim bloc */
		//pthread_mutex_unlock(&screen_control); /* obre semafor */ 
		//pthread_mutex_lock(&movement_control); /* tanca semafor */
		if (*(shared_mem.count_moves_ptr) && *(shared_mem.moviments_ptr) > 0)
		{
			(*shared_mem.moviments_ptr)--;    /* he fet un moviment de la paleta */
			sprintf(strin,"Temps: [%.2d:%.2d]. Moviments: [%d/%d].", *shared_mem.timer_min_ptr, *shared_mem.timer_sec_ptr, *(shared_mem.moviments_ptr), total_moves);
			//pthread_mutex_lock(&screen_control); /* tanca semafor */
			win_escristr(strin);
			//pthread_mutex_unlock(&screen_control); /* obre semafor */ 
		}
		//pthread_mutex_unlock(&movement_control); /* obre semafor */
	}
	//else
		//pthread_mutex_unlock(&screen_control); /* obre semafor */
	//pthread_mutex_lock(&screen_control); /* tanca semafor */
	if ((tecla == TEC_AMUNT) && (win_quincar(ipu_pf-1,ipu_pc) == ' '))
	{
		win_escricar(ipu_pf+l_pal-1,ipu_pc,' ',NO_INV); /* esborra ultim bloc */
		ipu_pf--;					    /* actualitza posicio */
		win_escricar(ipu_pf,ipu_pc,'0',INVERS);	    /* imprimeix primer bloc */
		//pthread_mutex_unlock(&screen_control); /* obre semafor */ 
		//pthread_mutex_lock(&movement_control); /* tanca semafor */
		if (*(shared_mem.count_moves_ptr) && *(shared_mem.moviments_ptr) > 0)
		{
			(*shared_mem.moviments_ptr)--;    /* he fet un moviment de la paleta */
			sprintf(strin,"Temps: [%.2d:%.2d]. Moviments: [%d/%d].", *shared_mem.timer_min_ptr, *shared_mem.timer_sec_ptr, *(shared_mem.moviments_ptr), total_moves);
			//pthread_mutex_lock(&screen_control); /* tanca semafor */
			win_escristr(strin);
			//pthread_mutex_unlock(&screen_control); /* obre semafor */ 
		}
		//pthread_mutex_unlock(&movement_control); /* obre semafor */
	}
	//else
		//pthread_mutex_unlock(&screen_control); /* obre semafor */
}

void	*user_functionality()
{
	char	pause_active;

	win_set(map_mem(p_map), n_fil, n_col);

	pause_active = 0;
	while (!(*shared_mem.start_ptr) && !(*shared_mem.creation_failed_ptr));
	if (!(*shared_mem.creation_failed_ptr))
	{
		while(!(*shared_mem.end_ptr))
		{
			//pthread_mutex_lock(&screen_control); /* tanca semafor */
			tecla = win_gettec();
			//pthread_mutex_unlock(&screen_control); /* obre semafor */ 
			if (tecla == TEC_ESPAI)
			{
				if (!pause_active)
				{
					//pthread_mutex_lock(&pause_control);
					pause_active = 1;
				}
				else
				{
					//pthread_mutex_unlock(&pause_control);
					pause_active = 0;
				}
			}
			else if (tecla != 0 && !pause_active)
			{
				mou_paleta_usuari(tecla);
				win_retard(retard);
			}
		}
	}
	pthread_exit(0);
}
