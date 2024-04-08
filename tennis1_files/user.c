// thread used by the user

#include "tennis.h"

/* funcio per moure la paleta de l'usuari en funcio de la tecla premuda */
static void	mou_paleta_usuari(int tecla)
{
	if ((tecla == TEC_AVALL) && (win_quincar(ipu_pf+l_pal,ipu_pc) == ' '))
	{
		win_escricar(ipu_pf,ipu_pc,' ',NO_INV);	   /* esborra primer bloc */
		ipu_pf++;					   /* actualitza posicio */
		win_escricar(ipu_pf+l_pal-1,ipu_pc,'0',INVERS); /* impri. ultim bloc */
		if (moviments > 0) moviments--;    /* he fet un moviment de la paleta */
	}
	if ((tecla == TEC_AMUNT) && (win_quincar(ipu_pf-1,ipu_pc) == ' '))
	{
		win_escricar(ipu_pf+l_pal-1,ipu_pc,' ',NO_INV); /* esborra ultim bloc */
		ipu_pf--;					    /* actualitza posicio */
		win_escricar(ipu_pf,ipu_pc,'0',INVERS);	    /* imprimeix primer bloc */
		if (moviments > 0) moviments--;    /* he fet un moviment de la paleta */
	}
	if (tecla == TEC_ESPAI) 
		win_escristr("ARA HAURIA D'ATURAR ELS ELEMENTS DEL JOC");
}

void	*user_functionality()
{
	while(!end)
	{
		tecla = win_gettec();
		if (tecla != 0)
			mou_paleta_usuari(tecla);
		win_retard(retard);
	}
	pthread_exit(0);
}
