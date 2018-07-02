#pragma once
#ifndef TEXTURE_HOLDER_H
#define TEXTURE_HOLDER_H

#include <SFML/Graphics.hpp>
#include <map>

using namespace sf;
using namespace std;

class TextureHolder {
private:
	//a map container from the STL that holds related pairs of string and texture
	std::map<std::string, Texture> m_Textures;

	//a pointer of the same type as the class itself 
	static TextureHolder* m_s_Instance;

public:
	TextureHolder();
	static Texture& GetTexture(std::string const& filename);
};
#endif // !TEXTURE_HOLDER_H

