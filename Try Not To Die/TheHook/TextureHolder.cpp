#include "stdafx.h"
#include "TextureHolder.h"
#include <assert.h>

TextureHolder* TextureHolder::m_s_Instance = nullptr;

TextureHolder::TextureHolder() {
	assert(m_s_Instance == nullptr); //make sure that m_s_Instance = nullptr. if not exit execution
	m_s_Instance = this;
}

sf::Texture& TextureHolder::GetTexture(std::string const& filename) {
	//get a reference to m_textures using m_s_instance
	auto& m = m_s_Instance->m_Textures;

	//create an iterator to hold a key-value pair(kvp) and search for the required kvp
	auto keyValuePair = m.find(filename);
	//auto = map<string, Texture>::iterator

	if (keyValuePair != m.end()) {
		//return the texture, the second part of kvp
		return keyValuePair->second;
	}
	else {
		//find not found
		//create a new key value pair using the file name
		auto& texture = m[filename];
		texture.loadFromFile(filename);
		return texture;
	}
}