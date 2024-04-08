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

extern int n_fil, n_col, m_por;	/* dimensions del taulell i porteries */
extern int l_pal;			/* longitud de les paletes */
extern float v_pal;			/* velocitat de la paleta del programa */
extern float pal_ret;			/* percentatge de retard de la paleta */

extern int ipu_pf, ipu_pc;      	/* posicio del la paleta d'usuari */
extern int ipo_pf, ipo_pc;      	/* posicio del la paleta de l'ordinador */
extern float po_pf;	/* pos. vertical de la paleta de l'ordinador, en valor real */

extern int ipil_pf, ipil_pc;		/* posicio de la pilota, en valor enter */
extern float pil_pf, pil_pc;		/* posicio de la pilota, en valor real */
extern float pil_vf, pil_vc;		/* velocitat de la pilota, en valor real*/
extern float pil_ret;			/* percentatge de retard de la pilota */

extern int retard;		/* valor del retard de moviment, en mil.lisegons */
extern int moviments;		/* numero max de moviments paletes per acabar el joc */


void	mou_paleta_ordinador(void);
int		moure_pilota(void);

void	*user_functionality();

#endif
