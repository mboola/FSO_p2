
/*****************************************************************************/
/*									     */
/*				     Tennis0.c				     */
/*									     */
/*  Programa inicial d'exemple per a les practiques 2 i 3 de FSO.	     */
/*     Es tracta del joc del tennis: es dibuixa un camp de joc rectangular   */
/*     amb una porteria a cada costat, una paleta per l'usuari, una paleta   */
/*     per l'ordinador i una pilota que va rebotant per tot arreu; l'usuari  */
/*     disposa de dues tecles per controlar la seva paleta, mentre que l'or- */
/*     dinador mou la seva automaticament (amunt i avall). Evidentment, es   */
/*     tracta d'intentar col.locar la pilota a la porteria de l'ordinador    */
/*     (porteria de la dreta), abans que l'ordinador aconseguixi col.locar   */
/*     la pilota dins la porteria de l'usuari (porteria de l'esquerra).      */
/*									     */
/*  Arguments del programa:						     */
/*     per controlar la posicio de tots els elements del joc, cal indicar    */
/*     el nom d'un fitxer de text que contindra la seguent informacio:	     */
/*		n_fil n_col m_por l_pal					     */
/*		pil_pf pil_pc pil_vf pil_vc pil_ret			     */
/*		ipo_pf ipo_pc po_vf pal_ret				     */
/*									     */
/*     on 'n_fil', 'n_col' son les dimensions del taulell de joc, 'm_por'    */
/*     es la mida de les dues porteries, 'l_pal' es la longitud de les dues  */
/*     paletes; 'pil_pf', 'pil_pc' es la posicio inicial (fila,columna) de   */
/*     la pilota, mentre que 'pil_vf', 'pil_vc' es la velocitat inicial,     */
/*     pil_ret es el percentatge respecte al retard passat per paràmetre;    */
/*     finalment, 'ipo_pf', 'ipo_pc' indicara la posicio del primer caracter */
/*     de la paleta de l'ordinador, mentre que la seva velocitat vertical    */
/*     ve determinada pel parametre 'po_fv', i pal_ret el percentatge de     */
/*     retard en el moviment de la paleta de l'ordinador.		     */
/*									     */
/*     A mes, es podra afegir un segon argument opcional per indicar el      */
/*     retard de moviment de la pilota i la paleta de l'ordinador (en ms);   */
/*     el valor d'aquest parametre per defecte es 100 (1 decima de segon).   */
/*									     */
/*  Compilar i executar:					  	     */
/*     El programa invoca les funcions definides en 'winsuport.o', les       */
/*     quals proporcionen una interficie senzilla per a crear una finestra   */
/*     de text on es poden imprimir caracters en posicions especifiques de   */
/*     la pantalla (basada en CURSES); per tant, el programa necessita ser   */
/*     compilat amb la llibreria 'curses':				     */
/*									     */
/*	   $ gcc tennis0.c winsuport.o -o tennis0 -lcurses		     */
/*	   $ tennis0 fit_param [retard]					     */
/*									     */
/*  Codis de retorn:						  	     */
/*     El programa retorna algun dels seguents codis al SO:		     */
/*	0  ==>  funcionament normal					     */
/*	1  ==>  numero d'arguments incorrecte 				     */
/*	2  ==>  fitxer no accessible					     */
/*	3  ==>  dimensions del taulell incorrectes			     */
/*	4  ==>  parametres de la pilota incorrectes			     */
/*	5  ==>  parametres d'alguna de les paletes incorrectes		     */
/*	6  ==>  no s'ha pogut crear el camp de joc (no pot iniciar CURSES)   */
/*****************************************************************************/

#include "tennis.h"

/* variables globals */
int n_fil, n_col, m_por;	/* dimensions del taulell i porteries */
int l_pal;			/* longitud de les paletes */

int ipu_pf, ipu_pc;      	/* posicio del la paleta d'usuari */

t_paleta	paletes[MAX_PROCS];	/* paletes del programa */
t_mem		shared_mem;

int			screen_id_sem;
int			move_id_sem;
int			pause_id_sem;

pthread_mutex_t	screen_control = PTHREAD_MUTEX_INITIALIZER;		//Lock to control the resource screen
pthread_mutex_t	movement_control = PTHREAD_MUTEX_INITIALIZER;	//Lock to control the moviments value
pthread_mutex_t pause_control = PTHREAD_MUTEX_INITIALIZER;

int ipil_pf, ipil_pc;		/* posicio de la pilota, en valor enter */
float pil_pf, pil_pc;		/* posicio de la pilota, en valor real */
float pil_vf, pil_vc;		/* velocitat de la pilota, en valor real*/
float pil_ret;			/* percentatge de retard de la pilota */

int	n_paletes;	/* nombre total de paletes carregades des del fitxer */

int retard;		/* valor del retard de moviment, en mil.lisegons */
int	total_moves;

int	tecla;
int	control;

/* funcio per realitzar la carrega dels parametres de joc emmagatzemats */
/* dins un fitxer de text, el nom del qual es passa per referencia en   */
/* 'nom_fit'; si es detecta algun problema, la funcio avorta l'execucio */
/* enviant un missatge per la sortida d'error i retornant el codi per-	*/
/* tinent al SO (segons comentaris del principi del programa).		*/
void carrega_parametres(const char *nom_fit)
{
	FILE *fit;

	fit = fopen(nom_fit,"rt");		/* intenta obrir fitxer */
	if (fit == NULL)
	{
		fprintf(stderr,"No s'ha pogut obrir el fitxer \'%s\'\n",nom_fit);
		exit(2);
	}

	if (!feof(fit))
		fscanf(fit,"%d %d %d %d\n",&n_fil,&n_col,&m_por,&l_pal);
	if ((n_fil < MIN_FIL) || (n_fil > MAX_FIL) ||
		(n_col < MIN_COL) || (n_col > MAX_COL) || (m_por < 0) ||
		(m_por > n_fil-3) || (l_pal < MIN_PAL) || (l_pal > n_fil-3))
	{
		fprintf(stderr,"Error: dimensions del camp de joc incorrectes:\n");
		fprintf(stderr,"\t%d =< n_fil (%d) =< %d\n",MIN_FIL,n_fil,MAX_FIL);
		fprintf(stderr,"\t%d =< n_col (%d) =< %d\n",MIN_COL,n_col,MAX_COL);
		fprintf(stderr,"\t0 =< m_por (%d) =< n_fil-3 (%d)\n",m_por,(n_fil-3));
		fprintf(stderr,"\t%d =< l_pal (%d) =< n_fil-3 (%d)\n",MIN_PAL,l_pal,(n_fil-3));
		fclose(fit);
		exit(3);
	}

	if (!feof(fit))
		fscanf(fit,"%d %d %f %f %f\n",&ipil_pf,&ipil_pc,&pil_vf,&pil_vc,&pil_ret);
	if ((ipil_pf < 1) || (ipil_pf > n_fil-3) ||
		(ipil_pc < 1) || (ipil_pc > n_col-2) ||
		(pil_vf < MIN_VEL) || (pil_vf > MAX_VEL) ||
		(pil_vc < MIN_VEL) || (pil_vc > MAX_VEL) ||
		(pil_ret < MIN_RET) || (pil_ret > MAX_RET))
	{
		fprintf(stderr,"Error: parametre pilota incorrectes:\n");
		fprintf(stderr,"\t1 =< ipil_pf (%d) =< n_fil-3 (%d)\n",ipil_pf,(n_fil-3));
		fprintf(stderr,"\t1 =< ipil_pc (%d) =< n_col-2 (%d)\n",ipil_pc,(n_col-2));
		fprintf(stderr,"\t%.1f =< pil_vf (%.1f) =< %.1f\n",MIN_VEL,pil_vf,MAX_VEL);
		fprintf(stderr,"\t%.1f =< pil_vc (%.1f) =< %.1f\n",MIN_VEL,pil_vc,MAX_VEL);
		fprintf(stderr,"\t%.1f =< pil_ret (%.1f) =< %.1f\n",MIN_RET,pil_ret,MAX_RET);
		fclose(fit);
		exit(4);
	}

	n_paletes = 0;
	while (!feof(fit))
	{
		fscanf(fit,"%d %d %f %f\n",&(paletes[n_paletes]).ipo_pf,&(paletes[n_paletes]).ipo_pc,&(paletes[n_paletes]).v_pal,&(paletes[n_paletes]).pal_ret);
		if ((paletes[n_paletes].ipo_pf < 1) || (paletes[n_paletes].ipo_pf+l_pal > n_fil-2) ||
			(paletes[n_paletes].ipo_pc < 5) || (paletes[n_paletes].ipo_pc > n_col-2) ||
			(paletes[n_paletes].v_pal < MIN_VEL) || (paletes[n_paletes].v_pal > MAX_VEL) ||
			(paletes[n_paletes].pal_ret < MIN_RET) || (paletes[n_paletes].pal_ret > MAX_RET))
		{
			fprintf(stderr,"Error: parametres paleta ordinador incorrectes:\n");
			fprintf(stderr,"\t1 =< ipo_pf (%d) =< n_fil-l_pal-3 (%d)\n",paletes[n_paletes].ipo_pf,(n_fil-l_pal-3));
			fprintf(stderr,"\t5 =< ipo_pc (%d) =< n_col-2 (%d)\n",paletes[n_paletes].ipo_pc,(n_col-2));
			fprintf(stderr,"\t%.1f =< v_pal (%.1f) =< %.1f\n",MIN_VEL,paletes[n_paletes].v_pal,MAX_VEL);
			fprintf(stderr,"\t%.1f =< pal_ret (%.1f) =< %.1f\n",MIN_RET,paletes[n_paletes].pal_ret,MAX_RET);
			fclose(fit);
			exit(5);
		}
		paletes[n_paletes].id = n_paletes + '0' + 1;
		n_paletes++;
	}
	fclose(fit);			/* fitxer carregat: tot OK! */
}


/* funcio per inicialitar les variables i visualitzar l'estat inicial del joc */
int inicialitza_joc(void)
{
	int i, i_port, f_port;
	int j;
	char strin[60];

	i_port = n_fil/2 - m_por/2;	    /* crea els forats de la porteria */
	if (n_fil%2 == 0) i_port--;
	if (i_port == 0) i_port=1;
	f_port = i_port + m_por -1;
	for (i = i_port; i <= f_port; i++)
	{
		waitS(screen_id_sem);//pthread_mutex_lock(&screen_control); /* tanca semafor */
		win_escricar(i,0,' ',NO_INV);
		win_escricar(i,n_col-1,' ',NO_INV);
		signalS(screen_id_sem);//pthread_mutex_unlock(&screen_control); /* obre semafor */ 
	}


	ipu_pf = n_fil/2; ipu_pc = 3;		/* inicialitzar pos. paletes */
	if (ipu_pf+l_pal >= n_fil-3) ipu_pf = 1;
	for (i=0; i< l_pal; i++)	    /* dibuixar paleta inicialment */
	{
		waitS(screen_id_sem);//pthread_mutex_lock(&screen_control); /* tanca semafor */
		win_escricar(ipu_pf +i, ipu_pc, '0',INVERS);
		for (j = 0; j < n_paletes; j++)
			win_escricar(paletes[j].ipo_pf + i, paletes[j].ipo_pc, paletes[j].id, INVERS);
		signalS(screen_id_sem);//pthread_mutex_unlock(&screen_control); /* obre semafor */ 
	}
	for (j = 0; j < n_paletes; j++)	/* fixar valor real paleta ordinador */
		paletes[j].po_pf = paletes[j].ipo_pf;

	pil_pf = ipil_pf; pil_pc = ipil_pc;	/* fixar valor real posicio pilota */
	waitS(screen_id_sem);//pthread_mutex_lock(&screen_control); /* tanca semafor */
	win_escricar(ipil_pf, ipil_pc, '.',INVERS);	/* dibuix inicial pilota */

	sprintf(strin,"Temps: [%.2d:%.2d]. Moviments: [%d/%d].", 0, 0, *(shared_mem.moviments_ptr), total_moves);
	win_escristr(strin);
	signalS(screen_id_sem);//pthread_mutex_unlock(&screen_control); /* obre semafor */ 
	return(0);
}

static t_mem	create_shared_mem(int mida_camp)
{
	t_mem	shared_mem;

	//create shared mem of moviments
	shared_mem.moviments_mem = ini_mem(sizeof(int));
	shared_mem.moviments_ptr = map_mem(shared_mem.moviments_mem);
	//create shared mem of creation_failed
	shared_mem.creation_failed_mem = ini_mem(sizeof(char));
	shared_mem.creation_failed_ptr = map_mem(shared_mem.creation_failed_mem);
	//create shared mem of start
	shared_mem.start_mem = ini_mem(sizeof(char));
	shared_mem.start_ptr = map_mem(shared_mem.start_mem);
	//create shared mem of end
	shared_mem.end_mem = ini_mem(sizeof(char));
	shared_mem.end_ptr = map_mem(shared_mem.end_mem);
	//create shared mem of pause_game
	shared_mem.pause_game_mem = ini_mem(sizeof(char));
	shared_mem.pause_game_ptr = map_mem(shared_mem.pause_game_mem);
	//create shared mem of control
	shared_mem.control_mem = ini_mem(sizeof(int));
	shared_mem.control_ptr = map_mem(shared_mem.control_mem);
	//create share mem of moves counted
	shared_mem.count_moves_mem = ini_mem(sizeof(int));
	shared_mem.count_moves_ptr = map_mem(shared_mem.count_moves_mem);
	//
	shared_mem.timer_sec_mem = ini_mem(sizeof(char));
	shared_mem.timer_sec_ptr = map_mem(shared_mem.timer_sec_mem);
	//
	shared_mem.timer_min_mem = ini_mem(sizeof(int));
	shared_mem.timer_min_ptr = map_mem(shared_mem.timer_min_mem);

	shared_mem.camp_mem = ini_mem(mida_camp);
	shared_mem.camp_ptr = map_mem(shared_mem.camp_mem);
	return (shared_mem);
}

static void	init_args(char args[N_ARGS][ARGS_LEN], t_mem shared_mem)
{
	sprintf(args[0], "%i", shared_mem.moviments_mem);
	sprintf(args[1], "%i", shared_mem.creation_failed_mem);
	sprintf(args[2], "%i", shared_mem.start_mem);
	sprintf(args[3], "%i", shared_mem.end_mem);
	sprintf(args[4], "%i", shared_mem.pause_game_mem);
	sprintf(args[5], "%i", shared_mem.control_mem);
	sprintf(args[6], "%i", shared_mem.count_moves_mem);
	sprintf(args[7], "%d", shared_mem.timer_sec_mem);
	sprintf(args[8], "%d", shared_mem.timer_min_mem);
	sprintf(args[9], "%i", shared_mem.camp_mem);
	sprintf(args[10], "%d", retard);
	sprintf(args[11], "%d", total_moves);
	sprintf(args[12], "%d", l_pal);
	sprintf(args[13], "%d", n_fil);
	sprintf(args[14], "%d", n_col);
	sprintf(args[21], "%d", screen_id_sem);
	sprintf(args[22], "%d", move_id_sem);
	sprintf(args[23], "%d", pause_id_sem);
}

static void	update_args(char args[N_ARGS][ARGS_LEN], int i)
{
	t_paleta	paleta;

	paleta = paletes[i];
	sprintf(args[15], "%i", paleta.ipo_pf);
	sprintf(args[16], "%i", paleta.ipo_pc);
	sprintf(args[17], "%f", paleta.po_pf);
	sprintf(args[18], "%f", paleta.v_pal);
	sprintf(args[19], "%f", paleta.pal_ret);
	sprintf(args[20], "%d", paleta.id);
}

/* programa principal				    */
int main(int n_args, const char *ll_args[])
{
	t_lock_data	lock_data;
	char		args[N_ARGS][ARGS_LEN];
	int			i;
	pid_t		pids[MAX_PROCS];
	int			mida_camp;

	if ((n_args != 3) && (n_args !=4))
	{
		fprintf(stderr,"Comanda: tennis0 fit_param moviments [retard]\n");
		exit(1);
	}

	carrega_parametres(ll_args[1]);

	mida_camp = win_ini(&n_fil,&n_col,'+',INVERS);
	if (mida_camp < 0)
	{
		switch (mida_camp) /* si no pot crear l'entorn de joc amb les curses */
		{
			case -1: fprintf(stderr,"camp de joc ja creat!\n"); break;
			case -2: fprintf(stderr,"no s'ha pogut inicialitzar l'entorn de curses!\n"); break;
			case -3: fprintf(stderr,"les mides del camp demanades son massa grans!\n"); break;
			case -4: fprintf(stderr,"no s'ha pogut crear la finestra!\n"); break;
		}
		exit(2);
	}

	//create mem shared
	shared_mem = create_shared_mem(mida_camp);

	screen_id_sem = ini_sem(1);
	move_id_sem = ini_sem(1);
	pause_id_sem = ini_sem(1);

	*shared_mem.moviments_ptr = atoi(ll_args[2]);
	total_moves = *shared_mem.moviments_ptr;

	if (*shared_mem.moviments_ptr == 0)
		*shared_mem.count_moves_ptr = 0;
	else
		*shared_mem.count_moves_ptr = 1;

	if (n_args == 4)
		retard = atoi(ll_args[3]);
	else
		retard = 100;

	win_set(shared_mem.camp_ptr, n_fil, n_col);

	if (inicialitza_joc() !=0)    /* intenta crear el taulell de joc */
		exit(4);   /* aborta si hi ha algun problema amb taulell */

	//Variables used get a controlled execution
	*(shared_mem.creation_failed_ptr) = 0;
	*shared_mem.start_ptr = 0;
	*shared_mem.end_ptr = 0;
	*shared_mem.control_ptr = -1;
	*shared_mem.pause_game_ptr = 0;

	init_threads(&lock_data, &shared_mem);
	init_args(args, shared_mem);
	for (i = 0; i < n_paletes; i++)
	{
		pids[i] = fork();
		if (pids[i] < (pid_t) 0) //error
			end_program(i, &lock_data, &shared_mem);
		else if (pids[i] == (pid_t) 0) //child
		{
			//execute pal_ord3.c
			update_args(args, i);
			//fprintf(stderr, "Args main: [%s].\n", args[0]);
			execlp(PAL_ORD_EXE, PAL_ORD,
				args[0], args[1], args[2], args[3], args[4], args[5], args[6],	//mem
				args[7], args[8],	//timer
				args[9],	//camp
				args[10], args[11], args[12], args[13], args[14],	//globals
				args[15], args[16], args[17], args[18], args[19], args[20],	//paleta
				args[21], args[22],	args[23], //semaphore
				NULL);
			exit(0);
		}
	}
	//fprintf(stderr, "All procs created.\n");
	*shared_mem.start_ptr = 1;

	/********** bucle principal del joc **********/
	while ((tecla != TEC_RETURN) && (*shared_mem.control_ptr == -1) &&
		((!*shared_mem.count_moves_ptr && *shared_mem.moviments_ptr == 0) || (*shared_mem.moviments_ptr > 0) || *shared_mem.moviments_ptr == -1)
		&& !*shared_mem.end_ptr)
	{
		waitS(screen_id_sem);//pthread_mutex_lock(&screen_control); /* tanca semafor */
		win_update();			/* actualitza visualitzacio CURSES */
		signalS(screen_id_sem);//pthread_mutex_unlock(&screen_control); /* obre semafor */ 
	}

	fprintf(stderr, "Ended.\n");
	*shared_mem.end_ptr = 1;

	//tell procs to end somehow
	for (i = 0; i < n_paletes; i++)
		wait(NULL);
	
	end_threads(lock_data);
	win_fi();
	elim_sem(screen_id_sem);
	elim_sem(move_id_sem);
	remove_mem(&shared_mem);

	if (*shared_mem.control_ptr == 0 || *shared_mem.moviments_ptr == 0)
		printf("Ha guanyat l'ordinador!\n");
	else
		printf("Ha guanyat l'usuari!\n");
	printf("El temps total de joc ha estat de: [%.2d:%.2d].\n", *shared_mem.timer_min_ptr, *shared_mem.timer_sec_ptr);
	return(0);
}
