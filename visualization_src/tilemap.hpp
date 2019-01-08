#ifndef TILEMAP_HPP
# define TILEMAP_HPP

# include <SFML/Graphics.hpp>

# define grey	9

class TileMap : public sf::Drawable, public sf::Transformable
{

public:

	TileMap(void);
	~TileMap(void);

	bool	load_from_file(const std::string &tileset);
	void	redraw(void);
	void	load_tilemap(const int *tiles, int width, int height);
	void	set_start_width(int start_width);
	void	redraw_piece(int color);

private:

	virtual void	draw(sf::RenderTarget& target, sf::RenderStates states) const;

	const int			*tiles;
	int					width;
	int					height;
	int					start_width;
	sf::VertexArray		m_vertices;
	sf::Texture			m_tileset;
	sf::Vector2u		m_tileSize;
	
};

#endif
