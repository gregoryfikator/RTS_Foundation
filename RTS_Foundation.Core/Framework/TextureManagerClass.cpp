#include "TextureManagerClass.h"

TextureManagerClass::TextureManagerClass()
{
	m_TextureArray = nullptr;
}

TextureManagerClass::TextureManagerClass(const TextureManagerClass& other)
{
}

TextureManagerClass::~TextureManagerClass()
{
}

bool TextureManagerClass::Initialize(int count)
{
	m_textureCount = count;

	// Create the color texture object.
	m_TextureArray = new TextureClass[m_textureCount];
	if (!m_TextureArray)
	{
		return false;
	}

	return true;
}

bool TextureManagerClass::InitializeTerrainTextures(int count, int* textureIndexArray)
{
	// Create the color texture object.
	m_TerrainTextureArray = new TerrainTextureArrayClass(count);
	if (!m_TerrainTextureArray)
	{
		return false;
	}

	for (int i = 0; i < count; i++)
	{
		m_TerrainTextureArray->SetTexture(i, m_TextureArray[textureIndexArray[i]].GetTexture());
	}

	return true;
}

void TextureManagerClass::Shutdown()
{
	int i;

	if (m_TerrainTextureArray)
	{
		delete m_TerrainTextureArray;
		m_TerrainTextureArray = nullptr;
	}

	// Release the texture objects.
	if (m_TextureArray)
	{
		for (i = 0; i < m_textureCount; i++)
		{
			m_TextureArray[i].Shutdown();
		}
		delete[] m_TextureArray;
		m_TextureArray = nullptr;
	}

	return;
}

bool TextureManagerClass::LoadTexture(ID3D11Device* device, ID3D11DeviceContext* deviceContext, const WCHAR* filename, int location)
{
	bool result;

	// Initialize the color texture object.
	result = m_TextureArray[location].Initialize(device, filename);
	if (!result)
	{
		return false;
	}

	return true;
}

ID3D11ShaderResourceView* TextureManagerClass::GetTexture(int id)
{
	return m_TextureArray[id].GetTexture();
}

ID3D11ShaderResourceView** TextureManagerClass::GetTerrainTextures()
{
	return m_TerrainTextureArray->GetTextureArray();
}
