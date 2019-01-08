#include "filler.h"
#include <unistd.h>
#include <ctype.h>

static bool	find_nl(char *line, char **buf_cur, int *i)
{
	char *cur = *buf_cur;
	while (*cur != '\n' && *cur != '\0')
		line[(*i)++] = *cur++;
	if (*cur == '\n')
	{
		*buf_cur = (cur + 1);
		line[*i] = '\0';
		return true;
	}
	return false;
}

static int		get_next_line(char *line)
{
	static char		buf[BUF_SIZE];
	static char		*buf_cur;
	int				bytes_read;
	int				i;

	if (buf[0] == '\0')
		buf_cur = buf;
	i = 0;
	while (find_nl(line, &buf_cur, &i) == false)
	{
		if ((bytes_read = read(0, buf, BUF_SIZE - 1)) == 0)
			return 0;
		buf[bytes_read] = '\0';
		buf_cur = buf;
	}
	return 1;
}

static void	get_hight_and_lenght(int *h, int *l)
{
	char line[BUF_SIZE];
	get_next_line(line);

	int i = 0;
	while (isdigit(line[i]) == false)
		i++;

	*h = 0;
	while (isdigit(line[i]) == true)
		*h = *h * 10 + line[i++] - '0';

	i++;
	*l = 0;
	while (isdigit(line[i]) == true)
		*l = *l * 10 + line[i++] - '0';
}

void	memalloc_and_read_map(t_filler *f)
{
	char line[BUF_SIZE];

	get_hight_and_lenght(&f->map_h, &f->map_l);
	get_next_line(line);
	f->map = (int **)malloc(sizeof(int *) * f->map_h);
	int *map = (int *)malloc(sizeof(int) * f->map_h * f->map_l);
	for (int n = 0; n < f->map_h; n++)
	{
		get_next_line(line);
		f->map[n] = map + n * f->map_l;
		for (int i = 0; i < f->map_l; i++)
			f->map[n][i] = line[i + 4];
	}
}

int		read_map(t_filler *f)
{
	char line[BUF_SIZE];

	if (get_next_line(line) == 0)
		return 0;
	get_next_line(line);
	for (int n = 0; n < f->map_h; n++)
	{
		get_next_line(line);
		for (int i = 0; i < f->map_l; i++)
			f->map[n][i] = line[i + 4];
	}
	return 1;
}

void	read_piece(t_filler *f)
{
	char	line[BUF_SIZE];

	get_hight_and_lenght(&f->piece_h, &f->piece_l);
	f->piece_size = 0;
	for (int n = 0; n < f->piece_h; n++)
	{
		get_next_line(line);
		for (int i = 0; i < f->piece_l; i++)
		{
			if (line[i] == '*')
			{
				f->piece[f->piece_size][0] = n;
				f->piece[f->piece_size][1] = i;
				if (++f->piece_size == MAX_PIECE_SIZE)
					return ;
			}
		}
	}
}

char	find_player(void)
{
	char first_line_buf[BUF_SIZE];

	get_next_line(first_line_buf);
	return first_line_buf[10] == '1' ? 'O' : 'X';
}
