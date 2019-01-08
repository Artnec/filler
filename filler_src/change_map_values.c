#include "filler.h"

static void		prioritise_non_isolated(t_filler *f, int h, int l)
{
	if (h < 0 || h >= f->map_h || l < 0 || l >= f->map_l || f->map[h][l] % 10 != 0)
		return ;

	f->map[h][l] += -99;
	prioritise_non_isolated(f, h + 1, l);
	prioritise_non_isolated(f, h - 1, l);
	prioritise_non_isolated(f, h, l + 1);
	prioritise_non_isolated(f, h, l - 1);
}

static void		spread_from_map_borders(t_filler *f)
{
	for (int h = 0; h < f->map_h; h++)
	{
		prioritise_non_isolated(f, h, 0);
		prioritise_non_isolated(f, h, f->map_l - 1);
	}
	for (int l = 1; l < f->map_l - 1; l++)
	{
		prioritise_non_isolated(f, 0, l);
		prioritise_non_isolated(f, f->map_h - 1, l);
	}
}

static void		push_empty_neighbors(t_filler *f, int h, int l, t_neighbors *n)
{
	if (n->n > 49996)
		return ;
	if (h > 0 && f->map[h - 1][l] == '.')
	{
		n->h[n->n] = h - 1;
		n->l[n->n++] = l;
	}
	if (l > 0 && f->map[h][l - 1] == '.')
	{
		n->h[n->n] = h;
		n->l[n->n++] = l - 1;
	}
	if (h < f->map_h - 1 && f->map[h + 1][l] == '.')
	{
		n->h[n->n] = h + 1;
		n->l[n->n++] = l;
	}
	if (l < f->map_l - 1 && f->map[h][l + 1] == '.')
	{
		n->h[n->n] = h;
		n->l[n->n++] = l + 1;
	}
}

static void		change_values(t_filler *f, t_neighbors *n, t_neighbors *n_next, int val)
{
	for (int i = 0; i < n->n; i++)
	{
		if (f->map[n->h[i]][n->l[i]] > val)
		{
			f->map[n->h[i]][n->l[i]] = val;
			push_empty_neighbors(f, n->h[i], n->l[i], n_next);
		}
	}
	n->n = 0;
}

static void		swap(t_neighbors **a, t_neighbors **b)
{
	t_neighbors *tmp = *a;
	*a = *b;
	*b = tmp;
}

void	change_map_values(t_filler *f)
{
	t_neighbors n;
	t_neighbors n_next;

	n.n = 0;
	for (int h = 0; h < f->map_h; h++)
	{
		for (int l = 0; l < f->map_l; l++)
		{
			if (IS_ENEMY(f->map[h][l], f->enemy))
				push_empty_neighbors(f, h, l, &n);
		}
	}
	t_neighbors *p = &n;
	t_neighbors *p_next = &n_next;
	p_next->n = 0;
	for (int val = -10000; p->n > 0; val += 10)
	{
		change_values(f, p, p_next, val);
		swap(&p, &p_next);
	}
	spread_from_map_borders(f);
}
