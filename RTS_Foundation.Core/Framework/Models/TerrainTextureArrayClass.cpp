#include "TerrainTextureArrayClass.h"

TerrainTextureArrayClass::TerrainTextureArrayClass(int textureCount)
{
	m_textures = new ID3D11ShaderResourceView*[textureCount];
	m_textureCount = textureCount;

	for (int i = 0; i < textureCount; i++)
	{
		m_textures[i] = nullptr;
	}
}

TerrainTextureArrayClass::~TerrainTextureArrayClass()
{
}

void TerrainTextureArrayClass::Shutdown()
{
	for (int i = 0; i < m_textureCount; i++)
	{
		m_textures[i]->Release();
		m_textures[i] = nullptr;
	}

	return;
}

ID3D11ShaderResourceView** TerrainTextureArrayClass::GetTextureArray()
{
	return m_textures;
}

int TerrainTextureArrayClass::GetTextureCount()
{
	return m_textureCount;
}

void TerrainTextureArrayClass::SetTexture(int index, ID3D11ShaderResourceView* texture)
{
	m_textures[index] = texture;
}
