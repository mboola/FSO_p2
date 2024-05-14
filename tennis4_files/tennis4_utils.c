
#include "tennis.h"

void	init_args(char args[N_ARGS][ARGS_LEN], t_mem shared_mem)
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
	sprintf(args[24], "%d", shared_mem.mailbox_id);
}

void	update_args(char args[N_ARGS][ARGS_LEN], int i)
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

t_mem	create_shared_mem(int mida_camp)
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

	shared_mem.mailbox_id = ini_mem(sizeof(int) * (n_paletes));	//mailbox for each process
	shared_mem.mailbox_ptr = map_mem(shared_mem.mailbox_id);
	return (shared_mem);
}

void	remove_mem(t_mem *mem)
{
	elim_mem(mem->moviments_mem);
	elim_mem(mem->creation_failed_mem);
	elim_mem(mem->start_mem);
	elim_mem(mem->end_mem);
	elim_mem(mem->pause_game_mem);
	elim_mem(mem->control_mem);
	elim_mem(mem->count_moves_mem);
	elim_mem(mem->timer_sec_mem);
	elim_mem(mem->timer_min_mem);
	elim_mem(mem->camp_mem);
	elim_mem(mem->mailbox_id);
	exit (2);
}
