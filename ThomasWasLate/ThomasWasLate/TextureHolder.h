#pragma once
#ifndef TEXTURE_HOLDER_H
#define TEXTURE_HOLDER_H

#include "stdafx.h"
#include <SFML/Graphics.hpp>
#include <map>

class TextureHolder {
private:
	std::map<std::string, sf::Texture> m_Textures;
	static TextureHolder *m_s_Instance;

public:
	TextureHolder();
	static sf::Texture& GetTexture(std::string const& filename);
};

#endif // !TEXTURE_HOLDER_H
