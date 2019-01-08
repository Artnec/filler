#ifndef FILLER_H
# define FILLER_H

# include <stdlib.h>
# include <stdbool.h>

# define BUF_SIZE				1024
# define MAX_PIECE_SIZE			64
# define IS_ENEMY(map, enemy)	(map == enemy || map == enemy + 'a' - 'A')

typedef struct	s_filler
{
	int			**map;
	int			map_h;
	int			map_l;
	int			piece[MAX_PIECE_SIZE][2];
	int			piece_size;
	int			piece_h;
	int			piece_l;
	int			final_h;
	int			final_l;
	int			sum;
	char		player;
	char		enemy;
}				t_filler;

typedef struct	s_neighbors
{
	int			h[50000];
	int			l[50000];
	int			n;
	int			i;
}				t_neighbors;


/*
**		read_data.c
*/
char	find_player(void);
void	memalloc_and_read_map(t_filler *f);
int		read_map(t_filler *f);
void	read_piece(t_filler *f);

/*
**		change_map_values.c
*/
void	change_map_values(t_filler *f);


#endif
