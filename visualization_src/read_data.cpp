#include "visualization.hpp"

static void		skip_map(int map_height)
{
	std::string line;
	for (int i = 0; i <= map_height; i++)
		std::getline(std::cin, line);
}

static void		announce_winner(t_filler &f, int p1)
{
	std::string	line;
	std::getline(std::cin, line);
	if (line.substr(line.size() - 8, line.size()) == "Segfault")
	{
		std::cout << line << std::endl;
		std::getline(std::cin, line);
	}
	int p2 = std::stoi(line.substr(10));

	std::cout << (p1 > p2 ? f.p1 : f.p2) << " has won\n";
}

static void		save_piece(t_piece &p, std::string line)
{
	p.piece_height = std::stoi(line);
	p.piece_width = std::stoi(line.substr(line.find_last_of(" ") + 1));

	p.piece = (int **)malloc(sizeof(int *) * p.piece_height);
	int *piece = (int *)malloc(sizeof(int) * p.piece_height * p.piece_width);
	for (int n = 0; n < p.piece_height; n++)
	{
		std::getline(std::cin, line);
		p.piece[n] = &piece[n * p.piece_width];
		for (int i = 0; i < p.piece_width; i++)
			p.piece[n][i] = line[i];
	}

	std::getline(std::cin, line);
	if (line.substr(0, 6) == "<got (")
	{
		p.player = line[6] == 'O' ? PLAYER1 : PLAYER2;
		line = line.substr(11);
		p.h = std::stoi(line);
		p.w = std::stoi(line.substr(line.find_last_of(" ") + 1));
	}
	else if (line.substr(0, 12) == "Player with ")
	{
		p.player = line[12] == 'O' ? PLAYER1 : PLAYER2;
		p.h = -1000;
		p.w = -1000;
	}
}

void	read_piece(t_filler &f)
{
	std::string line;
	std::getline(std::cin, line);
	if (line.substr(0, 7) == "Plateau")
	{
		skip_map(f.map_height);
		std::getline(std::cin, line);
	}
	else if (line.substr(0, 3) == "== ")
	{
		announce_winner(f, std::stoi(line.substr(10)));
		f.end_of_file = true;
		return ;
	}
	else if (line.substr(0, 6) != "Piece ")
		std::getline(std::cin, line);

	t_piece p;
	save_piece(p, line.substr(6));
	f.pieces.push_back(p);
}

static void		save_map(t_filler &f)
{
	f.map_size = f.map_height * f.map_width;
	f.map = (int *)malloc(sizeof(int) * f.map_size);
	for (int i = 0, n = 0; n < f.map_height; n++)
	{
		std::string line;
		std::getline(std::cin, line);
		for (int j = 0; j < f.map_width; i++, j++)
		{
			if (line[j + 4] == '.')
				f.map[i] = 0;
			else if (line[j + 4] == 'O')
				f.map[i] = f.colors[PLAYER1];
			else
				f.map[i] = f.colors[PLAYER2];
		}
	}
}

void	read_data(t_filler &f)
{
	std::string line;
	for (int i = 0; i < 5; i++)
		std::getline(std::cin, line);

	std::getline(std::cin, line);
	f.p1 = line.substr(line.find_last_of("/") + 1);
	std::getline(std::cin, line);

	std::getline(std::cin, line);
	f.p2 = line.substr(line.find_last_of("/") + 1);
	std::getline(std::cin, line);

	std::getline(std::cin, line);
	line = line.substr(8);
	f.map_height = std::stoi(line);
	f.map_width = std::stoi(line.substr(line.find_last_of(" ") + 1));
	std::getline(std::cin, line);

	save_map(f);
}
