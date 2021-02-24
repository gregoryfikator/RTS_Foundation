#pragma once

#ifndef _TEXTUREMANAGERCLASS_H_
#define _TEXTUREMANAGERCLASS_H_

#include "Models/TerrainTextureArrayClass.h"
#include "Models/TextureClass.h"

class TextureManagerClass
{
public:
	TextureManagerClass();
	TextureManagerClass(const TextureManagerClass&);
	~TextureManagerClass();

	bool Initialize(int);
	bool InitializeTerrainTextures(int, int*);
	void Shutdown();

	bool LoadTexture(ID3D11Device*, ID3D11DeviceContext*, const WCHAR*, int);

	ID3D11ShaderResourceView* GetTexture(int);
	ID3D11ShaderResourceView** GetTerrainTextures();

private:
	TextureClass* m_TextureArray;

	TerrainTextureArrayClass* m_TerrainTextureArray;

	int m_textureCount;
};

#endif