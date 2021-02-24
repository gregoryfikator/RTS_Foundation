#pragma once

#ifndef _MODELCLASS_H_
#define _MODELCLASS_H_

#include <d3d11.h>
#include <d3dx10math.h>

#include "TextureArrayClass.h"
#include "../TextureManagerClass.h"

#include "RTS_Foundation.OBJLoader/OBJLoader.h"

class ModelClass
{
private:
	typedef struct
	{
		D3DXVECTOR3 position;
		D3DXVECTOR2 texture;
		D3DXVECTOR3 normal;
	} VertexType;

public:
	ModelClass();
	ModelClass(const ModelClass&);
	~ModelClass();

	bool Initialize(ID3D11Device*, OBJLoader*, const char*, TextureManagerClass*);
	void Shutdown();
	void Render(ID3D11DeviceContext*);

	int GetIndexCount();

	ID3D11ShaderResourceView** GetTextureArray();

private:
	bool InitializeBuffers(ID3D11Device*);
	void ShutdownBuffers();
	void RenderBuffers(ID3D11DeviceContext*);

	bool LoadTextures(ID3D11Device*, TextureManagerClass*, int*, int);
	void ReleaseTextures();

	bool LoadConvertedModel(OBJLoader*, const char*);
	void ReleaseModel();

private:
	ID3D11Buffer * m_vertexBuffer, * m_indexBuffer;
	int m_vertexCount, m_indexCount;

	TextureArrayClass* m_TextureArray;

	ModelType* m_Model;
};

#endif
