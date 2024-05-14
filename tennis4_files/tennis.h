/***********************************************************/
/*                                                         */
/*                      tennis.h                           */
/*                                                         */
/* 				blablabla			                       */
/*														   */
/***********************************************************/

#ifndef TENNIS_H
# define TENNIS_H

# include "winsuport2.h"
# include "memoria.h"
# include "missatge.h"
# include "semafor.h"
# include <sys/wait.h>
# include <pthread.h>
# include <unistd.h>
# include <stdio.h>
# include <stdlib.h>

# ifndef MAX_PROCS
#  define MAX_PROCS 9
# endif

# ifndef N_ARGS
#  define N_ARGS 24
# endif

# ifndef ARGS_LEN
#  define ARGS_LEN 400
# endif

# ifndef PAL_ORD_EXE
#  define PAL_ORD_EXE "./tennis3_ord"
# endif

# ifndef PAL_ORD
#  define PAL_ORD "tennis3_ord"
# endif

#define MIN_FIL 7		/* definir limits de variables globals */
#define MAX_FIL 25
#define MIN_COL 10
#define MAX_COL 80
#define MIN_PAL 3
#define MIN_VEL -1.0
#define MAX_VEL 1.0
#define MIN_RET 0.0
#define MAX_RET 5.0
#define MAX_PAL 9

#define TEC_ESPAI   ' '  	/* tecla de barra espaiadora */

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
	int		timer_sec_mem;
	char	*timer_sec_ptr;
	int		timer_min_mem;
	char	*timer_min_ptr;
	int		camp_mem;
	void	*camp_ptr;
	int		mailbox_id;
	void	*mailbox_ptr;
}		t_mem;

extern int n_fil, n_col, m_por;	/* dimensions del taulell i porteries */
extern int l_pal;			/* longitud de les paletes */

extern int ipu_pf, ipu_pc;      	/* posicio del la paleta d'usuari */

extern t_paleta	paletes[MAX_PROCS];	/* array de paletes */
extern t_mem	shared_mem;

extern int		screen_id_sem;
extern int		move_id_sem;
extern int		pause_id_sem;

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

void	init_threads(t_lock_data *lock_data, t_mem *mem);
void	end_threads(t_lock_data lock_data);
void	end_program(int process_created, t_lock_data *lock_data, t_mem *mem);
void	remove_mem(t_mem *mem);

#endif
