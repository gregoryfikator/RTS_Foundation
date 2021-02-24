#pragma once

#ifndef _TEXTUREARRAYCLASS_H_
#define _TEXTUREARRAYCLASS_H_

#include <d3d11.h>
#include <d3dx11tex.h>

#include "../TextureManagerClass.h"

class TextureArrayClass
{
public:
	TextureArrayClass();
	~TextureArrayClass();

	bool Initialize(ID3D11Device*, TextureManagerClass*, int*, int);
	void Shutdown();

	ID3D11ShaderResourceView** GetTextureArray();

private:
	ID3D11ShaderResourceView** m_textures;
	int m_texturesCount;
};

#endif