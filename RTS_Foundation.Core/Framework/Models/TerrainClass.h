#pragma once

#ifndef _TERRAINCLASS_H_
#define _TERRAINCLASS_H_

#include <d3d11.h>
#include <d3dx10math.h>
#include <fstream>
#include <stdio.h>
#include <chrono>

#include "../TextureManagerClass.h"

#include "RTS_Foundation.NoiseGenerator/PerlinNoiseGenerator.h"

class TerrainClass
{
private:

	struct VertexType
	{
		D3DXVECTOR3 position;
		D3DXVECTOR2 texture;
		D3DXVECTOR3 normal;
	};

	struct HeightMapType
	{
		float x, y, z;
		float nx, ny, nz;
	};

	struct ModelType
	{
		float x, y, z;
		float tu, tv;
		float nx, ny, nz;
	};

	struct VectorType
	{
		float x, y, z;
	};

public:
	TerrainClass();
	TerrainClass(const TerrainClass&);
	~TerrainClass();

	bool Initialize(ID3D11Device*, const char*, TextureManagerClass*);
	void Shutdown();
	void Render(ID3D11DeviceContext*);

	int GetIndexCount();
	D3DXVECTOR4* GetBiomHeights();

	// Generator related

	long GetGenerationTime();

	long GetSeed();
	void DisableSeed(ID3D11Device*);
	void IncreaseSeed(ID3D11Device*);
	void DecreaseSeed(ID3D11Device*);

	float* GetDisplacement();
	void DisableDisplacement(ID3D11Device*);
	void IncreaseDisplacementX(ID3D11Device*);
	void IncreaseDisplacementY(ID3D11Device*);
	void DecreaseDisplacementX(ID3D11Device*);
	void DecreaseDisplacementY(ID3D11Device*);

	float GetFrequency();
	void DisableFrequency(ID3D11Device*);
	void IncreaseFrequency(ID3D11Device*);
	void DecreaseFrequency(ID3D11Device*);

	float GetTerraceSmoothness();
	void DisableTerraceSmoothness(ID3D11Device*);
	void IncreaseTerraceSmoothness(ID3D11Device*);
	void DecreaseTerraceSmoothness(ID3D11Device*);

	float GetExponent();
	void DisableExponent(ID3D11Device* device);
	void IncreaseExponent(ID3D11Device* device);
	void DecreaseExponent(ID3D11Device* device);

private:
	bool LoadSetupFile(const char*);
	bool GenerateTerrain(ID3D11Device*);
	bool RegenerateTerrain(ID3D11Device*);
	void ShutdownHeightMap();
	void SetTerrainCoordinates();

	bool CalculateNormals();
	bool BuildTerrainModel();
	void ShutdownTerrainModel();

	bool InitializeBuffers(ID3D11Device*);
	void ShutdownBuffers();
	void RenderBuffers(ID3D11DeviceContext*);

private:
	ID3D11Buffer* m_vertexBuffer, *m_indexBuffer;
	int m_vertexCount, m_indexCount;

	int m_terrainHeight, m_terrainWidth;
	float m_heightScale;
	char* m_terrainFilename;
	HeightMapType* m_heightMap;
	ModelType* m_terrainModel;

	// Textures info
	int m_texturesCount;
	int* m_textureIndexes;
	D3DXVECTOR4* m_biomHeights;

	// Generator data
	PerlinNoiseGenerator* m_generator;
	long m_seed;

	float* m_displacement;
	float m_frequency;
	int m_octaves;
	float* m_amplitudes;
	float m_terraceSmoothnessFactor;
	float m_exponent;

	long m_generationTime;
};

#endif
