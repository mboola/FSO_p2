/***********************************************************/
/*                                                         */
/*                      tennis.h                           */
/*                                                         */
/* 				blablabla			                       */
/*														   */
/***********************************************************/

#ifndef TENNIS_H
# define TENNIS_H

# include "winsuport.h"
# include <pthread.h>
# include <unistd.h>

# ifndef MAX_PROCS
#  define MAX_PROCS 9
# endif

# ifndef N_ARGS
#  define N_ARGS 13
# endif

# ifndef ARGS_LEN
#  define ARGS_LEN 100
# endif

# ifndef PAL_ORD_EXE
#  define PAL_ORD_EXE "./tennis3_ord"
# endif

# ifndef PAL_ORD
#  define PAL_ORD "tennis3_ord"
# endif

typedef struct s_paleta
{
	int	ipo_pf;		/* posicio del la paleta de l'ordinador */
	int	ipo_pc;		/* posicio del la paleta de l'ordinador */
	float po_pf;	/* pos. vertical de la paleta de l'ordinador, en valor real */
	float v_pal;	/* velocitat de la paleta del programa */
	float pal_ret;	/* percentatge de retard de la paleta */
	char id;
}		t_paleta;

typedef	struct s_timer
{
	char	sec;
	char	min;
}		t_timer;

typedef struct s_lock_data
{
	pthread_t 	id_user;
	pthread_t	id_ball;
	pthread_t	id_timer;
	pthread_t	id_computer[MAX_PAL];
}		t_lock_data;

typedef struct s_mem
{
	int		moviments_mem;
	int		*moviments_ptr;
	int		creation_failed_mem;
	char	*creation_failed_ptr;
	int		start_mem;
	char	*start_ptr;
	int		end_mem;
	char	*end_ptr;
	int		pause_game_mem;
	char	*pause_game_ptr;
	int		control_mem;
	int		*control_ptr;
	int		count_moves_mem;
	int		*count_moves_ptr;
}		t_mem;

extern int n_fil, n_col, m_por;	/* dimensions del taulell i porteries */
extern int l_pal;			/* longitud de les paletes */

extern int ipu_pf, ipu_pc;      	/* posicio del la paleta d'usuari */

extern t_paleta	paletes[MAX_PAL];	/* array de paletes */
extern t_timer	timer;

extern pthread_mutex_t	screen_control;		//Lock to control the resource screen
extern pthread_mutex_t	movement_control;	//Lock to control the moviments value
extern pthread_mutex_t	timer_pause_control;
extern pthread_mutex_t	ball_pause_control;
extern pthread_mutex_t	computer_pause_control[MAX_PAL];

extern char	creation_failed;

extern char	count_moves;
extern char	start;	//Var to initialize the execution of the threads
extern char	end;	//Var to stop the executation of the threads
extern char	pause_game;	//Var to pause the executation of the threads

extern int ipil_pf, ipil_pc;		/* posicio de la pilota, en valor enter */
extern float pil_pf, pil_pc;		/* posicio de la pilota, en valor real */
extern float pil_vf, pil_vc;		/* velocitat de la pilota, en valor real*/
extern float pil_ret;			/* percentatge de retard de la pilota */

extern int	total_moves;
extern int	n_paletes;
extern int	retard;		/* valor del retard de moviment, en mil.lisegons */
extern int	moviments;		/* numero max de moviments paletes per acabar el joc */

//custom globals used
extern int	tecla;
extern int  control;

void	*system_functionality();
void	*user_functionality();
void	*ball_functionality();
void	*timer_functionality();

#endif