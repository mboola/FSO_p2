// thread used by the computer

#include "tennis.h"

int	total_moves;
int l_pal;			/* longitud de les paletes */

/* funcio per moure la paleta de l'ordinador autonomament, en funcio de la */
/* velocitat de la paleta (variable global v_pal) */
static void mou_paleta_ordinador(t_paleta *paleta, t_mem shared_mem)
{
	int		f_h;
	char	strin[60];
	/* char rh,rv,rd; */

	f_h = paleta->po_pf + paleta->v_pal;		/* posicio hipotetica de la paleta */
	if (f_h != paleta->ipo_pf)	/* si pos. hipotetica no coincideix amb pos. actual */
	{
		if (paleta->v_pal > 0.0)			/* verificar moviment cap avall */
		{
			//pthread_mutex_lock(&screen_control); /* tanca semafor */
			if (win_quincar(f_h+l_pal - 1, paleta->ipo_pc) == ' ')   /* si no hi ha obstacle */
			{
				win_escricar(paleta->ipo_pf, paleta->ipo_pc, ' ', NO_INV);      /* esborra primer bloc */
				paleta->po_pf += paleta->v_pal; paleta->ipo_pf = paleta->po_pf;		/* actualitza posicio */
				win_escricar(paleta->ipo_pf + l_pal - 1, paleta->ipo_pc, paleta->id, INVERS); /* impr. ultim bloc */
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
			else		/* si hi ha obstacle, canvia el sentit del moviment */
			{
				//pthread_mutex_unlock(&screen_control); /* obre semafor */
				paleta->v_pal = -paleta->v_pal;
			}
		}
		else			/* verificar moviment cap amunt */
		{
			//pthread_mutex_lock(&screen_control); /* tanca semafor */
			if (win_quincar(f_h, paleta->ipo_pc) == ' ')        /* si no hi ha obstacle */
			{
				win_escricar(paleta->ipo_pf + l_pal - 1, paleta->ipo_pc, ' ', NO_INV); /* esbo. ultim bloc */
				paleta->po_pf += paleta->v_pal; paleta->ipo_pf = paleta->po_pf;		/* actualitza posicio */
				win_escricar(paleta->ipo_pf, paleta->ipo_pc, paleta->id, INVERS);	/* impr. primer bloc */
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
			else		/* si hi ha obstacle, canvia el sentit del moviment */
			{
				//pthread_mutex_unlock(&screen_control); /* obre semafor */
				paleta->v_pal = -paleta->v_pal;
			}
		}
	}
	else paleta->po_pf += paleta->v_pal; 	/* actualitza posicio vertical real de la paleta */
}

static t_mem	convert_mem(char **argv)
{
	t_mem	mem;

	//create shared mem of moviments
	mem.moviments_mem = atoi(argv[0]);
	mem.moviments_ptr = map_mem(mem.moviments_mem);
	//create shared mem of creation_failed
	mem.creation_failed_mem = atoi(argv[1]);
	mem.creation_failed_ptr = map_mem(mem.creation_failed_mem);
	//create shared mem of start
	mem.start_mem = atoi(argv[2]);
	mem.start_ptr = map_mem(mem.start_mem);
	//create shared mem of end
	mem.end_mem = atoi(argv[3]);
	mem.end_ptr = map_mem(mem.end_mem);
	//create shared mem of pause_game
	mem.pause_game_mem = atoi(argv[4]);
	mem.pause_game_ptr = map_mem(mem.pause_game_mem);
	//create shared mem of control
	mem.control_mem = atoi(argv[5]);
	mem.control_ptr = map_mem(mem.control_mem);
	//create share mem of moves counted
	mem.count_moves_mem = atoi(argv[6]);
	mem.count_moves_ptr = map_mem(mem.count_moves_mem);
	//
	mem.timer_sec_mem = atoi(argv[16]);
	mem.timer_sec_ptr = map_mem(mem.timer_sec_mem);
	//
	mem.timer_min_mem = atoi(argv[17]);
	mem.timer_min_ptr = map_mem(mem.timer_min_mem);

	return (mem);
}

static t_paleta	convert_paleta(char **argv)
{
	t_paleta	paleta;

	paleta.ipo_pf = atoi(argv[7]);
	paleta.ipo_pc = atoi(argv[8]);
	paleta.po_pf = atof(argv[9]);
	paleta.v_pal = atof(argv[10]);
	paleta.pal_ret = atof(argv[11]);
	paleta.id = atoi(argv[12]);

	return (paleta);
}

int	main(int argc, char **argv)
{
	t_paleta	paleta;
	t_mem		shared_mem;
	int			retard;

	(void)argc;
	
	//char pid_str[20];
    //sprintf(pid_str, "%d", getpid());
	int temp = atoi(argv[18]);
	void* map = map_mem(temp);
	int fils = atoi(argv[19]);
	int cols = atoi(argv[20]);
	win_set(map, fils, cols);

	shared_mem = convert_mem(argv);
	paleta = convert_paleta(argv);
	retard = atoi(argv[13]);
	total_moves = atoi(argv[14]);
	l_pal = atoi(argv[15]);

	while (!(*shared_mem.start_ptr) && !(*shared_mem.creation_failed_ptr));
	if (!(*shared_mem.creation_failed_ptr))
	{
		while(!(*shared_mem.end_ptr))
		{
			mou_paleta_ordinador(&paleta, shared_mem);
			win_retard(retard);
		}
	}
	return(0);
}
