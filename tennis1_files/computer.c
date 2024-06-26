// thread used by the computer

#include "tennis.h"

/* funcio per moure la paleta de l'ordinador autonomament, en funcio de la */
/* velocitat de la paleta (variable global v_pal) */
static void mou_paleta_ordinador(t_paleta *paleta)
{
	int f_h;

	f_h = paleta->po_pf + paleta->v_pal;		/* posicio hipotetica de la paleta */
	if (f_h != paleta->ipo_pf)	/* si pos. hipotetica no coincideix amb pos. actual */
	{
		if (paleta->v_pal > 0.0)			/* verificar moviment cap avall */
		{
			if (win_quincar(f_h+l_pal-1,paleta->ipo_pc) == ' ')   /* si no hi ha obstacle */
			{
				win_escricar(paleta->ipo_pf,paleta->ipo_pc,' ',NO_INV);      /* esborra primer bloc */
				paleta->po_pf += paleta->v_pal; paleta->ipo_pf = paleta->po_pf;		/* actualitza posicio */
				win_escricar(paleta->ipo_pf+l_pal-1,paleta->ipo_pc,paleta->id,INVERS); /* impr. ultim bloc */
				moviments++;    /* he fet un moviment de la paleta */
			}
			else		/* si hi ha obstacle, canvia el sentit del moviment */
				paleta->v_pal = -paleta->v_pal;
		}
		else			/* verificar moviment cap amunt */
		{
			if (win_quincar(f_h,paleta->ipo_pc) == ' ')        /* si no hi ha obstacle */
				{
				win_escricar(paleta->ipo_pf+l_pal-1,paleta->ipo_pc,' ',NO_INV); /* esbo. ultim bloc */
				paleta->po_pf += paleta->v_pal; paleta->ipo_pf = paleta->po_pf;		/* actualitza posicio */
				win_escricar(paleta->ipo_pf,paleta->ipo_pc,paleta->id,INVERS);	 /* impr. primer bloc */
				moviments++;    /* he fet un moviment de la paleta */
				}
			else		/* si hi ha obstacle, canvia el sentit del moviment */
				paleta->v_pal = -paleta->v_pal;
		}
	}
	else
		paleta->po_pf += paleta->v_pal; 	/* actualitza posicio vertical real de la paleta */
}

void	*system_functionality(void *paleta_ptr)
{
	t_paleta	*paleta;

	paleta = (t_paleta*)paleta_ptr;
	while(!end)
	{
		mou_paleta_ordinador(paleta);
		win_retard(retard);
	}
	pthread_exit(0);
}
