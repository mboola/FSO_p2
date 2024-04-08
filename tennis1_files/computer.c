// thread used by the computer

#include "tennis.h"

/* funcio per moure la paleta de l'ordinador autonomament, en funcio de la */
/* velocitat de la paleta (variable global v_pal) */
static void mou_paleta_ordinador(void)
{
	int f_h;
	/* char rh,rv,rd; */

	f_h = po_pf + v_pal;		/* posicio hipotetica de la paleta */
	if (f_h != ipo_pf)	/* si pos. hipotetica no coincideix amb pos. actual */
	{
		if (v_pal > 0.0)			/* verificar moviment cap avall */
		{
			if (win_quincar(f_h+l_pal-1,ipo_pc) == ' ')   /* si no hi ha obstacle */
			{
				win_escricar(ipo_pf,ipo_pc,' ',NO_INV);      /* esborra primer bloc */
				po_pf += v_pal; ipo_pf = po_pf;		/* actualitza posicio */
				win_escricar(ipo_pf+l_pal-1,ipo_pc,'1',INVERS); /* impr. ultim bloc */
					if (moviments > 0) moviments--;    /* he fet un moviment de la paleta */
			}
			else		/* si hi ha obstacle, canvia el sentit del moviment */
				v_pal = -v_pal;
		}
		else			/* verificar moviment cap amunt */
		{
			if (win_quincar(f_h,ipo_pc) == ' ')        /* si no hi ha obstacle */
				{
				win_escricar(ipo_pf+l_pal-1,ipo_pc,' ',NO_INV); /* esbo. ultim bloc */
				po_pf += v_pal; ipo_pf = po_pf;		/* actualitza posicio */
				win_escricar(ipo_pf,ipo_pc,'1',INVERS);	/* impr. primer bloc */
					if (moviments > 0) moviments--;    /* he fet un moviment de la paleta */
				}
			else		/* si hi ha obstacle, canvia el sentit del moviment */
				v_pal = -v_pal;
		}
	}
	else po_pf += v_pal; 	/* actualitza posicio vertical real de la paleta */
}

void	*system_functionality()
{
	while(!end)
	{
		mou_paleta_ordinador();
		win_retard(retard);
	}
	pthread_exit(0);
}
