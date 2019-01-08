#include "tilemap.hpp"
#include <iostream>

TileMap::TileMap(void)
{
	this->m_vertices.setPrimitiveType(sf::Quads);
	this->m_tileSize = sf::Vector2u(100, 100);
}

TileMap::~TileMap(void) {}


void	TileMap::load_tilemap(const int *tiles, int width, int height)
{
	this->tiles = tiles;
	this->width = width;
	this->height = height;
	this->m_vertices.resize(width * height * 4);
}

void	TileMap::set_start_width(int start_width)
{
	this->start_width = start_width;
}

bool	TileMap::load_from_file(const std::string &tileset_file)
{
	return (m_tileset.loadFromFile(tileset_file));
}

void	TileMap::redraw(void)
{
	for (int i = 0; i < this->width; i++)
	{
		for (int j = 0; j < this->height; j++)
		{
			int tileNumber = tiles[i + j * this->width];

			int tu = tileNumber % (m_tileset.getSize().x / m_tileSize.x);
			int tv = tileNumber / (m_tileset.getSize().x / m_tileSize.x);

			sf::Vertex* quad = &m_vertices[(i + j * this->width) * 4];

			quad[0].position = sf::Vector2f(i * m_tileSize.x, j * m_tileSize.y);
			quad[1].position = sf::Vector2f((i + 1) * m_tileSize.x, j * m_tileSize.y);
			quad[2].position = sf::Vector2f((i + 1) * m_tileSize.x, (j + 1) * m_tileSize.y);
			quad[3].position = sf::Vector2f(i * m_tileSize.x, (j + 1) * m_tileSize.y);

			quad[0].texCoords = sf::Vector2f(tu * m_tileSize.x, tv * m_tileSize.y);
			quad[1].texCoords = sf::Vector2f((tu + 1) * m_tileSize.x, tv * m_tileSize.y);
			quad[2].texCoords = sf::Vector2f((tu + 1) * m_tileSize.x, (tv + 1) * m_tileSize.y);
			quad[3].texCoords = sf::Vector2f(tu * m_tileSize.x, (tv + 1) * m_tileSize.y);
		}
	}
}

void	TileMap::redraw_piece(int color)
{
	for (int i = this->start_width; i < this->width + this->start_width; i++)
	{
		for (int j = 0; j < this->height; j++)
		{
			int tileNumber = tiles[i - this->start_width + j * this->width];
			int tu = tileNumber == '.' ? grey : color;

			sf::Vertex* quad = &m_vertices[(i - this->start_width + j * this->width) * 4];

			quad[0].position = sf::Vector2f(i * m_tileSize.x, j * m_tileSize.y);
			quad[1].position = sf::Vector2f((i + 1) * m_tileSize.x, j * m_tileSize.y);
			quad[2].position = sf::Vector2f((i + 1) * m_tileSize.x, (j + 1) * m_tileSize.y);
			quad[3].position = sf::Vector2f(i * m_tileSize.x, (j + 1) * m_tileSize.y);

			quad[0].texCoords = sf::Vector2f(tu * m_tileSize.x, 0);
			quad[1].texCoords = sf::Vector2f((tu + 1) * m_tileSize.x, 0);
			quad[2].texCoords = sf::Vector2f((tu + 1) * m_tileSize.x, m_tileSize.y);
			quad[3].texCoords = sf::Vector2f(tu * m_tileSize.x, m_tileSize.y);
		}
	}
}

void	TileMap::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	states.transform *= getTransform();
	states.texture = &m_tileset;
	target.draw(m_vertices, states);
}
