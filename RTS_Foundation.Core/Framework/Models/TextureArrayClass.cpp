#include "TextureArrayClass.h"

TextureArrayClass::TextureArrayClass()
{
}

TextureArrayClass::~TextureArrayClass()
{
}

bool TextureArrayClass::Initialize(ID3D11Device* device, TextureManagerClass* TextureManager, int* textureIndexes, int texturesCount)
{
	HRESULT result;

	m_texturesCount = texturesCount;

	m_textures = new ID3D11ShaderResourceView * [m_texturesCount];

	for (int i = 0; i < m_texturesCount; i++)
	{
		m_textures[i] = TextureManager->GetTexture(textureIndexes[i]);
		if (m_textures[i] == nullptr)
		{
			return false;
		}
	}

	return true;
}

void TextureArrayClass::Shutdown()
{
	//for (int i = 0; i < m_texturesCount; i++)
	//{
	//	m_textures[i]->Release();
	//	m_textures[i] = nullptr;
	//}

	delete m_textures;

	return;
}

ID3D11ShaderResourceView** TextureArrayClass::GetTextureArray()
{
	return m_textures;
}
