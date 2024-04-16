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

# ifndef MAX_THREADS
#  define MAX_THREADS 12
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
	pthread_t	id_system[MAX_PAL];
	char		threads_created[MAX_THREADS];
}		t_lock_data;

extern int n_fil, n_col, m_por;	/* dimensions del taulell i porteries */
extern int l_pal;			/* longitud de les paletes */

extern int ipu_pf, ipu_pc;      	/* posicio del la paleta d'usuari */

extern t_paleta	paletes[MAX_PAL];	/* array de paletes */
extern t_timer	timer;

extern pthread_mutex_t	screen_control;	//Lock to control the resource screen
extern pthread_mutex_t	movement_control;	//Lock to control the moviments value

extern char	creation_failed;

extern char	start;	//Var to initialize the execution of the threads
extern char	end;	//Var to stop the executation of the threads

extern int ipil_pf, ipil_pc;		/* posicio de la pilota, en valor enter */
extern float pil_pf, pil_pc;		/* posicio de la pilota, en valor real */
extern float pil_vf, pil_vc;		/* velocitat de la pilota, en valor real*/
extern float pil_ret;			/* percentatge de retard de la pilota */

extern int retard;		/* valor del retard de moviment, en mil.lisegons */
extern int moviments;		/* numero max de moviments paletes per acabar el joc */

//custom globals used
extern int	tecla;
extern int  control;

void	*system_functionality();
void	*user_functionality();
void	*ball_functionality();
void	*timer_functionality();

#endif
