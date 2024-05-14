// thread used by the ball

#include "tennis.h"

/* funcio per moure la pilota; retorna un valor amb alguna d'aquestes	*/
/* possibilitats:							*/
/*	-1 ==> la pilota no ha sortit del taulell			*/
/*	 0 ==> la pilota ha sortit per la porteria esquerra		*/
/*	>0 ==> la pilota ha sortit per la porteria dreta		*/

static int	moure_pilota(void)
{
	int f_h, c_h, result;
	char rh,rv,rd,pd;
	int	i;

	f_h = pil_pf + pil_vf;		/* posicio hipotetica de la pilota */
	c_h = pil_pc + pil_vc;
	result = -1;		/* inicialment suposem que la pilota no surt */
	rh = rv = rd = pd = ' ';
	if ((f_h != ipil_pf) || (c_h != ipil_pc))
	{		/* si posicio hipotetica no coincideix amb la pos. actual */
		if (f_h != ipil_pf)		/* provar rebot vertical */
		{
			waitS(screen_id_sem);//pthread_mutex_lock(&screen_control); /* tanca semafor */
			rv = win_quincar(f_h,ipil_pc);	/* veure si hi ha algun obstacle */
			signalS(screen_id_sem);//pthread_mutex_unlock(&screen_control); /* obre semafor */ 
			if (rv != ' ')			/* si no hi ha res */
			{
				pil_vf = -pil_vf;		/* canvia velocitat vertical */
				f_h = pil_pf+pil_vf;	/* actualitza posicio hipotetica */
			}
		}
		if (c_h != ipil_pc)		/* provar rebot horitzontal */
		{
			waitS(screen_id_sem);//pthread_mutex_lock(&screen_control); /* tanca semafor */
			rh = win_quincar(ipil_pf,c_h);	/* veure si hi ha algun obstacle */
			signalS(screen_id_sem);//pthread_mutex_unlock(&screen_control); /* obre semafor */ 
			if (rh != ' ')			/* si no hi ha res */
			{
				for (i = 0; i < n_paletes; i++)
				{
					if (rh == '0' + i + 1)	//if char found is paleta
						sendM(((int*)shared_mem.mailbox_ptr)[i], (void *) (intptr_t)((pil_vc > 0) + (pil_vc < 0) * -1), sizeof(int));
				}

				pil_vc = -pil_vc;		/* canvia velocitat horitzontal */
				c_h = pil_pc+pil_vc;	/* actualitza posicio hipotetica */
			}
		}
		if ((f_h != ipil_pf) && (c_h != ipil_pc))	/* provar rebot diagonal */
		{
			waitS(screen_id_sem);//pthread_mutex_lock(&screen_control); /* tanca semafor */
			rd = win_quincar(f_h,c_h);
			signalS(screen_id_sem);//pthread_mutex_unlock(&screen_control); /* obre semafor */ 
			if (rd != ' ')				/* si no hi ha obstacle */
			{
				pil_vf = -pil_vf; pil_vc = -pil_vc;	/* canvia velocitats */
				f_h = pil_pf+pil_vf;
				c_h = pil_pc+pil_vc;		/* actualitza posicio entera */
			}
		}
		waitS(screen_id_sem);//pthread_mutex_lock(&screen_control); /* tanca semafor */
		if (win_quincar(f_h,c_h) == ' ')	/* verificar posicio definitiva */
		{						/* si no hi ha obstacle */
			win_escricar(ipil_pf,ipil_pc,' ',NO_INV);	/* esborra pilota */
			pil_pf += pil_vf; pil_pc += pil_vc;
			ipil_pf = f_h; ipil_pc = c_h;		/* actualitza posicio actual */
			if ((ipil_pc > 0) && (ipil_pc <= n_col))	/* si no surt */
				win_escricar(ipil_pf,ipil_pc,'.',INVERS); /* imprimeix pilota */
			else
				result = ipil_pc;	/* codi de finalitzacio de partida */
		}
		signalS(screen_id_sem);//pthread_mutex_unlock(&screen_control); /* obre semafor */ 
	}
	else
	{
		pil_pf += pil_vf;
		pil_pc += pil_vc;
	}
	return(result);
}

void	*ball_functionality()
{
	while (!(*shared_mem.start_ptr) && !(*shared_mem.creation_failed_ptr));
	if (!(*shared_mem.creation_failed_ptr))
	{
		while(!(*shared_mem.end_ptr))
		{
			*shared_mem.control_ptr = moure_pilota();
			win_retard(retard);
			waitS(pause_id_sem);//pthread_mutex_lock(&pause_control);
			signalS(pause_id_sem);//pthread_mutex_unlock(&pause_control);
		}
	}
	pthread_exit(0);
}
