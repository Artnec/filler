#include "visualization.hpp"

static void		change_color(t_filler &f, e_players player)
{
	int	other_player_color = (player == PLAYER1 ? f.colors[PLAYER2] : f.colors[PLAYER1]);
	int next_color = (f.colors[player] == 8 ? 1 : f.colors[player] + 1);
	if (next_color == other_player_color)
		next_color = (next_color == 8 ? 1 : next_color + 1);

	for (int i = 0; i < f.map_size; i++)
	{
		if (f.map[i] == f.colors[player])
			f.map[i] = next_color;
	}
	f.colors[player] = next_color;
	f.change_color = true;
}

static void		pool_events(sf::RenderWindow &window, t_filler &f)
{
	sf::Event event;
	while (window.pollEvent(event))
	{
		if (event.type == sf::Event::Closed)
			window.close();
		if (event.type == sf::Event::KeyPressed)
		{
			if (event.key.code == sf::Keyboard::Escape)
				window.close();
			else if (event.key.code == sf::Keyboard::Enter)
			{
				f.enter = !f.enter;
				f.r_shift = false;
			}
			else if (event.key.code == sf::Keyboard::RShift)
			{
				f.r_shift = !f.r_shift;
				f.enter = false;
			}
			else if (event.key.code == sf::Keyboard::Right)
				put_piece(f);
			else if (event.key.code == sf::Keyboard::Left)
				remove_piece(f);
			else if (event.key.code == sf::Keyboard::Num1)
				change_color(f, PLAYER1);
			else if (event.key.code == sf::Keyboard::Num2)
				change_color(f, PLAYER2);
		}
	}
}

static void		set_text(t_filler &f, float zoom, int window_width, int window_height, float info_start)
{
	f.keys.setFont(f.font);
	f.keys.setCharacterSize(55 * zoom);
	f.keys.setPosition((window_width * info_start + 10) * zoom, (window_height - 320) * zoom);
	f.keys.setString("next: >\nprev: <\nauto next: Enter\nauto prev: RShift\nchange color: 1 2");

	f.p1_name.setFont(f.font);
	f.p1_name.setCharacterSize(55 * zoom);
	f.p1_name.setPosition((window_width * info_start + 10) * zoom, (window_height - 500) * zoom);
	f.p1_name.setString("p1: " + f.p1);

	f.p2_name.setFont(f.font);
	f.p2_name.setCharacterSize(55 * zoom);
	f.p2_name.setPosition((window_width * info_start + 10) * zoom, (window_height - 430) * zoom);
	f.p2_name.setString("p2: " + f.p2);
}

void	display_data(t_filler &f)
{
	float zoom = 1.0;
	while (f.map_height / zoom > 26 || f.map_width / zoom > 38)
		zoom += 0.05;

	int info_width = (f.map_width / 3.5f > 8 ? f.map_width / 3.5f : 8) * 100 / sqrt(zoom);
	int window_height = f.map_height * 100 / zoom;
	int window_width = f.map_width * 100 / zoom + info_width;

	sf::RenderWindow window(sf::VideoMode(window_width, window_height), "filler");
	sf::View view = window.getDefaultView();
	view.zoom(zoom);
	view.setCenter(window_width * (zoom / 2), window_height * (zoom / 2));
	window.setView(view);

	sf::VertexArray split_line(sf::LinesStrip, 2);
	split_line[0].position = sf::Vector2f((window_width - info_width) * zoom, 0);
	split_line[1].position = sf::Vector2f((window_width - info_width) * zoom, window_height * zoom);

	set_text(f, zoom, window_width, window_height, 1.0f - (float)info_width / window_width);
	sf::Color text_colors[9] = {{0x00, 0x00, 0x00}, {0x7f, 0x18, 0x56}, {0xfb, 0x57, 0x0b},
							{0xfe, 0xd4, 0x6b}, {0x00, 0x77, 0x44}, {0x52, 0xd0, 0xc8},
							{0xff, 0x00, 0x44}, {0xfe, 0xf1, 0xb2}, {0x8c, 0xf6, 0xae}};

	while (window.isOpen())
	{
		if (f.end_of_file == false)
			read_piece(f);
		pool_events(window, f);
		if (f.enter == true)
			put_piece(f);
		else if (f.r_shift == true)
			remove_piece(f);

		window.clear();
		if (f.reload_piece == true || f.change_color == true)
		{
			f.p1_name.setFillColor(text_colors[f.colors[PLAYER1]]);
			f.p2_name.setFillColor(text_colors[f.colors[PLAYER2]]);

			t_piece &cp = f.pieces[f.piece_cur + 1];
			f.tile_piece.load_tilemap(cp.piece[0], cp.piece_width, cp.piece_height);
			f.tile_piece.redraw_piece(f.colors[f.pieces[f.piece_cur + 1].player]);
			f.reload_piece = false;
			f.change_color = false;
			f.tile_map.redraw();
		}
		window.draw(f.tile_piece);
		window.draw(f.tile_map);

		window.draw(f.keys);
		window.draw(f.p1_name);
		window.draw(f.p2_name);
		window.draw(split_line);
		window.display();
	}
}

static bool		load_resources(t_filler &f)
{
	if (!f.font.loadFromFile("resources/arial.ttf"))
	{
		std::cout << "error: font not found\n";
		return false;
	}
	if (!f.tile_map.load_from_file("resources/10squares.png")
	|| !f.tile_piece.load_from_file("resources/10squares.png"))
	{
		std::cout << "error: resources not found\n";
		return false;
	}
	f.tile_map.load_tilemap(f.map, f.map_width, f.map_height);
	f.tile_piece.set_start_width(f.map_width);
	return true;
}

static void		initiate_structure(t_filler &f)
{
	f.colors[BACKGROUND] = black;
	f.colors[PLAYER1] = light_yellow;
	f.colors[PLAYER2] = dark_green;
	f.piece_cur = -1;
	f.end_of_file = false;
	f.enter = false;
	f.r_shift = false;
	f.change_color = false;
	f.reload_piece = true;
}

int		main(void)
{
	t_filler f;
	initiate_structure(f);
	read_data(f);
	if (load_resources(f) == false)
		return 1;
	display_data(f);
	return 0;
}
