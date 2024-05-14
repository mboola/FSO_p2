// thread used by the computer

#include "tennis.h"

int	total_moves;
int	l_pal;			/* longitud de les paletes */
int	n_fil, n_col;
int	screen_id_sem;
int	move_id_sem;
int	pause_id_sem;

pthread_t	id_mail_thread;
t_mem		shared_mem;
t_paleta	paleta;
char		end_proc;

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
			waitS(screen_id_sem);//pthread_mutex_lock(&screen_control); /* tanca semafor */
			if (win_quincar(f_h+l_pal - 1, paleta->ipo_pc) == ' ')   /* si no hi ha obstacle */
			{
				win_escricar(paleta->ipo_pf, paleta->ipo_pc, ' ', NO_INV);      /* esborra primer bloc */
				paleta->po_pf += paleta->v_pal; paleta->ipo_pf = paleta->po_pf;		/* actualitza posicio */
				win_escricar(paleta->ipo_pf + l_pal - 1, paleta->ipo_pc, paleta->id + '0' + 1, INVERS); /* impr. ultim bloc */
				signalS(screen_id_sem);//pthread_mutex_unlock(&screen_control); /* obre semafor */
				waitS(move_id_sem);//pthread_mutex_lock(&movement_control); /* tanca semafor */
				if (*(shared_mem.count_moves_ptr) && *(shared_mem.moviments_ptr) > 0)
				{
					(*shared_mem.moviments_ptr)--;    /* he fet un moviment de la paleta */
					signalS(move_id_sem);//pthread_mutex_unlock(&movement_control); /* obre semafor */
					sprintf(strin,"Temps: [%.2d:%.2d]. Moviments: [%d/%d].", *shared_mem.timer_min_ptr, *shared_mem.timer_sec_ptr, *(shared_mem.moviments_ptr), total_moves);
					waitS(screen_id_sem);//pthread_mutex_lock(&screen_control); /* tanca semafor */
					win_escristr(strin);
					signalS(screen_id_sem);//pthread_mutex_unlock(&screen_control); /* obre semafor */ 
				}
				else
					signalS(move_id_sem);//pthread_mutex_unlock(&movement_control); /* obre semafor */
			}
			else		/* si hi ha obstacle, canvia el sentit del moviment */
			{
				signalS(screen_id_sem);//pthread_mutex_unlock(&screen_control); /* obre semafor */
				paleta->v_pal = -paleta->v_pal;
			}
		}
		else			/* verificar moviment cap amunt */
		{
			waitS(screen_id_sem);//pthread_mutex_lock(&screen_control); /* tanca semafor */
			if (win_quincar(f_h, paleta->ipo_pc) == ' ')        /* si no hi ha obstacle */
			{
				win_escricar(paleta->ipo_pf + l_pal - 1, paleta->ipo_pc, ' ', NO_INV); /* esbo. ultim bloc */
				paleta->po_pf += paleta->v_pal; paleta->ipo_pf = paleta->po_pf;		/* actualitza posicio */
				win_escricar(paleta->ipo_pf, paleta->ipo_pc, paleta->id + '0' + 1, INVERS);	/* impr. primer bloc */
				signalS(screen_id_sem);//pthread_mutex_unlock(&screen_control); /* obre semafor */ 
				waitS(move_id_sem);//pthread_mutex_lock(&movement_control); /* tanca semafor */
				if (*(shared_mem.count_moves_ptr) && *(shared_mem.moviments_ptr) > 0)
				{
					(*shared_mem.moviments_ptr)--;    /* he fet un moviment de la paleta */
					signalS(move_id_sem);//pthread_mutex_unlock(&movement_control); /* obre semafor */
					sprintf(strin,"Temps: [%.2d:%.2d]. Moviments: [%d/%d].", *shared_mem.timer_min_ptr, *shared_mem.timer_sec_ptr, *(shared_mem.moviments_ptr), total_moves);
					waitS(screen_id_sem);//pthread_mutex_lock(&screen_control); /* tanca semafor */
					win_escristr(strin);
					signalS(screen_id_sem);//pthread_mutex_unlock(&screen_control); /* obre semafor */ 
				}
				else
					signalS(move_id_sem);//pthread_mutex_unlock(&movement_control); /* obre semafor */
			}
			else		/* si hi ha obstacle, canvia el sentit del moviment */
			{
				signalS(screen_id_sem);//pthread_mutex_unlock(&screen_control); /* obre semafor */
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
	mem.moviments_mem = atoi(argv[1]);
	mem.moviments_ptr = map_mem(mem.moviments_mem);
	//create shared mem of creation_failed
	mem.creation_failed_mem = atoi(argv[2]);
	mem.creation_failed_ptr = map_mem(mem.creation_failed_mem);
	//create shared mem of start
	mem.start_mem = atoi(argv[3]);
	mem.start_ptr = map_mem(mem.start_mem);
	//create shared mem of end
	mem.end_mem = atoi(argv[4]);
	mem.end_ptr = map_mem(mem.end_mem);
	//create shared mem of pause_game
	mem.pause_game_mem = atoi(argv[5]);
	mem.pause_game_ptr = map_mem(mem.pause_game_mem);
	//create shared mem of control
	mem.control_mem = atoi(argv[6]);
	mem.control_ptr = map_mem(mem.control_mem);
	//create share mem of moves counted
	mem.count_moves_mem = atoi(argv[7]);
	mem.count_moves_ptr = map_mem(mem.count_moves_mem);
	//
	mem.timer_sec_mem = atoi(argv[8]);
	mem.timer_sec_ptr = map_mem(mem.timer_sec_mem);
	//
	mem.timer_min_mem = atoi(argv[9]);
	mem.timer_min_ptr = map_mem(mem.timer_min_mem);

	mem.camp_mem = atoi(argv[10]);
	mem.camp_ptr = map_mem(mem.camp_mem);

	mem.mailbox_id = atoi(argv[25]);
	mem.mailbox_ptr = map_mem(mem.mailbox_id);
	return (mem);
}

static t_paleta	convert_paleta(char **argv)
{
	t_paleta	paleta;

	paleta.ipo_pf = atoi(argv[16]);
	paleta.ipo_pc = atoi(argv[17]);
	paleta.po_pf = atof(argv[18]);
	paleta.v_pal = atof(argv[19]);
	paleta.pal_ret = atof(argv[20]);
	paleta.id = atoi(argv[21]);

	return (paleta);
}

static void	*mailbox_functionality()
{
	char	msg[TAM_MAX_MIS];
	char	empty[MAX_PROCS];	//maybe change to n_procs??
	char	total_empty;
	int		i;

	while (!(*shared_mem.start_ptr) && !(*shared_mem.creation_failed_ptr));
	if (!(*shared_mem.creation_failed_ptr))
	{
		while(!(*shared_mem.end_ptr))
		{
			for (i = 0; i < MAX_PROCS; i++)
				empty[i] = 0;
			total_empty = 0;
			receiveM(shared_mem.mailbox_ptr[(paleta.id + 0)], msg);	//wait for a msg to be recived from the ball or from other paletes
			if (end_proc)
				pthread_exit(0);
			if (msg[0] > 0 && n_col == MIN_COL + 1)	//if I have to go to the left and im at limit
			{
				//clear all chars of the paleta
				end_proc = 1;
				pthread_exit(0);
			}
			else if (msg[0] < 0 && n_col == MAX_COL) //if I have to go to the right and im at limit
			{
				//clear all chars of the paleta
				end_proc = 1;
				pthread_exit(0);
			}
			// check if all the chars in the direction I must move are empty
			
			for (i = 0; i < MAX_PROCS; i++)
			{
				if (empty[i] == 1)
				{
					total_empty = 1;
					sendM(shared_mem.mailbox_ptr[i], (void *) msg, sizeof(int));
				}
			}
			if (!total_empty)	//if there was not a single paleta touching
			{
				//clear all chars of the paleta
				n_col += msg[0];	//move the paleta to one direction or the other
				//paint all chars of the paleta
			}
		}
	}
	pthread_exit(0);
}

int	main(int argc, char **argv)
{
	int			retard;

	end_proc = 0;
	(void)argc;
	shared_mem = convert_mem(argv);
	paleta = convert_paleta(argv);
	retard = atoi(argv[11]);
	total_moves = atoi(argv[12]);
	l_pal = atoi(argv[13]);
	n_fil = atoi(argv[14]);
	n_col = atoi(argv[15]);

	win_set(shared_mem.camp_ptr, n_fil, n_col);	/* crea acces a finestra oberta pel proces pare */

	screen_id_sem = atoi(argv[22]);
	move_id_sem = atoi(argv[23]);
	pause_id_sem = atoi(argv[24]);

	//fprintf(stderr, "Starting thread.\n");

	//initialize threads
	if (pthread_create(&id_mail_thread, NULL, mailbox_functionality, NULL))
	{
		*shared_mem.creation_failed_ptr = 1;
		return (0);
	}

	//fprintf(stderr, "Proc created.\n");
	while (!(*shared_mem.start_ptr) && !(*shared_mem.creation_failed_ptr));
	if (!(*shared_mem.creation_failed_ptr))
	{
		while(!(*shared_mem.end_ptr) && !end_proc)
		{
			//fprintf(stderr, "Loop.\n");
			mou_paleta_ordinador(&paleta, shared_mem);
			win_retard(retard);
			waitS(pause_id_sem);//pthread_mutex_lock(&pause_control);
			signalS(pause_id_sem);//pthread_mutex_unlock(&pause_control);
		}
	}
	end_proc = 1;
	sendM(shared_mem.mailbox_ptr[(paleta.id + 0)], (void *) NULL, sizeof(int));
	pthread_join(id_mail_thread, NULL);
	return(0);
}
