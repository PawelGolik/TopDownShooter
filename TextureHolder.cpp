#include "TextureHolder.h"
#include <assert.h>

TextureHolder* TextureHolder::m_s_TextureInstance = nullptr;

TextureHolder::TextureHolder()
{
	assert(m_s_TextureInstance == nullptr);
	m_s_TextureInstance = this;
}

Texture& TextureHolder::GetTexture(string const& filename)
{
	auto& textureMap = m_s_TextureInstance->m_Textures;
	auto keyValuePair = textureMap.find(filename);
	if (keyValuePair != textureMap.end())
	{
		return keyValuePair->second;
	}
	else
	{
		auto& texture = textureMap[filename];
		texture.loadFromFile(filename);
		return texture;
	}
}

TextureHolder::~TextureHolder()
{
}
