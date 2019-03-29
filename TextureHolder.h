#pragma once
#ifndef TEXTURE_HOLDER_H
#define TEXTURE_HOLDER_H

#include<map>
#include<SFML\Graphics.hpp>
#include<iostream>


using namespace std;
using namespace sf;

class TextureHolder
{
private:
	map<string, Texture> m_Textures;
	static TextureHolder* m_s_TextureInstance;
public:
	TextureHolder();
	static Texture& GetTexture(string const& filename);
	~TextureHolder();
};

#endif // !1
