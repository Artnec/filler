#ifndef VISUALIZATION_HPP
# define VISUALIZATION_HPP

# include <iostream>
# include <vector>
# include <math.h>

# include "tilemap.hpp"


enum	e_colors {black, dark_pink, orange, yellow, dark_green, light_blue, red, light_yellow, light_green};
enum	e_players {BACKGROUND, PLAYER1, PLAYER2};

typedef	struct				s_piece
{
	int						piece_height;
	int						piece_width;
	int						player;
	int						h;
	int						w;
	int						overlap;
	int						**piece;
	bool					fit;
}							t_piece;

typedef struct				s_filler
{
	int						map_width;
	int						map_height;
	int						map_size;
	int						*map;
	int						piece_cur;
	int						colors[3];
	sf::Font				font;
	sf::Text				keys;
	sf::Text				p1_name;
	sf::Text				p2_name;
	TileMap					tile_map;
	TileMap					tile_piece;
	bool					end_of_file;
	bool					enter;
	bool					r_shift;
	bool					reload_piece;
	bool					change_color;
	std::string				p1;
	std::string				p2;
	std::vector<t_piece>	pieces;
}							t_filler;

/*
**				read_data.cpp
*/
void			read_piece(t_filler &f);
void			read_data(t_filler &f);

/*
**				manipulate_piece.cpp
*/
void			put_piece(t_filler &f);
void			remove_piece(t_filler &f);

#endif
