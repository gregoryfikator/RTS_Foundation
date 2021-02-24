#pragma once

#ifndef _TERRAINTEXTUREARRAYCLASS_H_
#define _TERRAINTEXTUREARRAYCLASS_H_

#include <d3d11.h>
#include <d3dx11tex.h>
#include <vector>

class TerrainTextureArrayClass
{
public:
	TerrainTextureArrayClass(int);
	~TerrainTextureArrayClass();

	void Shutdown();

	ID3D11ShaderResourceView** GetTextureArray();
	void SetTexture(int, ID3D11ShaderResourceView*);

	int GetTextureCount();

private:
	ID3D11ShaderResourceView** m_textures;
	int m_textureCount;
};

#endif