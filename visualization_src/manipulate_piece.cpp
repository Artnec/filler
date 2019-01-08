#include "visualization.hpp"

static bool		fit_piece(t_piece &p, t_filler &f)
{
	int overlap = 0;
	for (int n = 0; n < p.piece_height; n++)
	{
		for (int i = 0; i < p.piece_width; i++)
		{
			int j = (n + p.h) * f.map_width + i + p.w;
			if (p.piece[n][i] == '*' && (j < 0 || j >= f.map_size))
				return false;
			if (p.piece[n][i] == '*' && f.map[j] != 0)
			{
				if (overlap == 0 && f.map[j] == f.colors[p.player])
					overlap++;
				else
					return false;
			}
		}
	}
	return overlap == 1;
}

void	put_piece(t_filler &f)
{
	if (f.piece_cur + 3 > (int)f.pieces.size())
	{
		f.enter = false;
		return ;
	}

	f.reload_piece = true;
	t_piece &p = f.pieces[++f.piece_cur];
	if (fit_piece(p, f) == false)
	{
		f.pieces[f.piece_cur].fit = false;
		return ;
	}

	for (int n = 0; n < p.piece_height; n++)
	{
		for (int i = 0; i < p.piece_width; i++)
		{
			int j = (n + p.h) * f.map_width + i + p.w;
			if (p.piece[n][i] == '*')
			{
				if (f.map[j] != 0)
					p.overlap = j;
				f.map[j] = f.colors[p.player];
			}
		}
	}
	f.pieces[f.piece_cur].fit = true;
}

void	remove_piece(t_filler &f)
{
	if (f.piece_cur == -1)
	{
		f.r_shift = false;
		return ;
	}

	f.reload_piece = true;
	if (f.pieces[f.piece_cur--].fit == false)
		return ;

	t_piece &p = f.pieces[f.piece_cur + 1];
	for (int n = 0; n < p.piece_height; n++)
	{
		for (int i = 0; i < p.piece_width; i++)
		{
			if (p.piece[n][i] == '*')
			{
				int j = (n + p.h) * f.map_width + i + p.w;
				f.map[j] = 0;
			}
		}
	}
	f.map[p.overlap] = f.colors[p.player];
}
