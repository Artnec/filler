#include <stdio.h>
#include <fcntl.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <dirent.h>
#include <stdbool.h>

int		p1_points = 0;
int		p2_points = 0;
int		p1_wins = 0;
int		p2_wins = 0;
char	buf[256];

static void		get_score(char *player1, char *player2)
{
	int fd;

	if ((fd = open("filler.trace", O_RDONLY)) == -1)
		return ;
	int bytes_read = read(fd, buf, 255);
	close(fd);
	buf[bytes_read] = '\0';
	if (strstr(buf, player1))
	{
		printf("%s won\n", player1);
		p1_wins++;
	}
	else
	{
		printf("%s won\n", player2);
		p2_wins++;
	}
	char *ptr = strchr(buf, ':') + 2;
	p1_points += atoi(ptr);
	ptr = strchr(ptr, 'T') + 1;
	p2_points += atoi(ptr);
}

static bool		find_bot_in_directory(char *bot_name)
{
	DIR		*dir;
	if ((dir = opendir("players")) == NULL)
	{
		fprintf(stderr,"players directory not found\n");
		return 3;
	}
	struct dirent	*entry;
	while ((entry = readdir(dir)))
	{
		if (strncmp(entry->d_name, bot_name, strlen(entry->d_name) - 7) == 0
		&& strcmp(entry->d_name + strlen(bot_name), ".filler") == 0)
			return true;
	}
	fprintf(stderr,"%s not found in players directory\n", bot_name);
	return false;
}

static bool		parse_map_and_cycles(int *cycles, char *map, char *cycles_input)
{
	if (strcmp(map, "00") && strcmp(map, "01") && strcmp(map, "02") && strcmp(map, "03"))
	{
		fprintf(stderr, "map must be between 00 and 03\n");
		return false;
	}
	if ((*cycles = atoi(cycles_input)) < 1 || *cycles > 1000)
	{
		fprintf(stderr, "number_of_runs must be more then 0 and less than 1000\n");
		return false;
	}
	return true;
}

int		main(int argc, char **argv)
{
	if (argc != 5)
	{
		printf("usage: map[00-03] bot_name1 bot_name2 number_of_runs[1-1000]\n");
		return 0;
	}

	int cycles;
	if (parse_map_and_cycles(&cycles, argv[1], argv[4]) == false)
		return 1;

	if (find_bot_in_directory(argv[2]) == false || find_bot_in_directory(argv[3]) == false)
		return 2;

	for (int n = 0; n < cycles; n++)
	{
		snprintf(buf, sizeof(buf), "./filler_vm -f maps/map%s -p1 players/%s.filler \
									-p2 players/%s.filler > /dev/null\n", argv[1], argv[2], argv[3]);
		system(buf);
		printf("%d : ", n + 1);
		get_score(argv[2], argv[3]);
	}
	if (p1_wins > p2_wins || (p1_wins == p2_wins && p1_points > p2_points))
		printf("player1 %s won with a score %i to %i and total score %i %i\n",
						argv[2], p1_wins, p2_wins, p1_points, p2_points);
	else
		printf("player2 %s won with a score %i to %i and total score %i %i\n",
						argv[3], p2_wins, p1_wins, p2_points, p1_points);
	return 0;
}
