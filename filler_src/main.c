#include "filler.h"
#include <stdio.h>

static bool		fit_piece(t_filler *f, int p[][2], int h, int l)
{
	int count = 0;
	int sum = 0;

	for (int i = 0; i < f->piece_size; i++)
	{
		if (h + p[i][0] >= f->map_h || h + p[i][0] < 0
		||  l + p[i][1] >= f->map_l || l + p[i][1] < 0)
			return false;
		else if (IS_ENEMY(f->map[h + p[i][0]][l + p[i][1]], f->enemy))
			return false;
		else if (f->map[h + p[i][0]][l + p[i][1]] == f->player)
		{
			if (++count > 1)
				return false;
		}
		else
			sum += f->map[h + p[i][0]][l + p[i][1]];
	}
	if (count == 1 && f->sum > sum)
	{
		f->sum = sum;
		return true;
	}
	return false;
}

static void		fit_any_piece_point(t_filler *f, int h, int l)
{
	for (int i = 0; i < f->piece_size; i++)
	{
		int h_dif = h - f->piece[i][0];
		int l_dif = l - f->piece[i][1];

		if (fit_piece(f, f->piece, h_dif, l_dif) == true)
		{
			f->final_h = h_dif;
			f->final_l = l_dif;
		}
	}
}

static void		make_move(t_filler *f)
{
	change_map_values(f);
	f->sum = 10000;
	f->final_h = 0;
	f->final_l = 0;
	for (int h = 0; h < f->map_h; h++)
	{
		for (int l = 0; l < f->map_l; l++)
		{
			if (f->map[h][l] == f->player)
				fit_any_piece_point(f, h, l);
		}
	}
	printf("%d %d\n", f->final_h, f->final_l);
	fflush(stdout); // filler_vm doesn't work with printf buffering
}

static void		make_first_move(t_filler *f)
{
	memalloc_and_read_map(f);
	read_piece(f);
	make_move(f);
}

static void		free_map(int **map)
{
	free(*map);
	free(map);
}

int		main(void)
{
	t_filler	f;

	f.player = find_player();
	f.enemy = (f.player == 'O' ? 'X' : 'O');
	make_first_move(&f);
	while (read_map(&f) == true)
	{
		read_piece(&f);
		make_move(&f);
	}
	free_map(f.map);
	return 0;
}
