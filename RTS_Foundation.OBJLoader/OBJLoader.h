#pragma once

#include <D3DX10math.h>
#include <fstream>

#ifndef _OBJLOADER_H_
#define _OBJLOADER_H_

typedef struct 
{
	float x, y, z;
	float tu, tv;
	float nx, ny, nz;
} ModelType;

class OBJLoader
{

private:
	typedef struct
	{
		float x, y, z;
	} Float3Type;

	typedef struct
	{
		int vIndex1, vIndex2, vIndex3;
		int tIndex1, tIndex2, tIndex3;
		int nIndex1, nIndex2, nIndex3;
	} FaceType;

public:
	OBJLoader();
	OBJLoader(const OBJLoader&);
	~OBJLoader();

	bool LoadObj(ModelType**, const char*, bool = false);
	bool ConvertObjToModel(const char*, const char*, bool = false);
	bool LoadConvertedModel(ModelType**, const char*);

	unsigned long GetIndiciesCount();
	unsigned long GetVerticesCount();

private:
	bool OpenFileStream(std::ifstream*, const char*);
	bool OpenFileStream(std::ofstream*, const char*);
	bool CloseFileStream(std::ifstream*);
	bool CloseFileStream(std::ofstream*);
	bool InitDataStructures();
	bool FillDataStructures(bool);
	bool SeedModel(ModelType**);
	void Initialize();
	bool WriteModelData();
	bool LoadModelData(ModelType**);
	bool ClearDataStructures();

private:
	std::ifstream fin;
	std::ofstream fout;

	unsigned long vertexCount, textureCount, normalCount, faceCount, indiciesCount;
	unsigned long vertexIndex, texcoordIndex, normalIndex, faceIndex;
	unsigned long vIndex, tIndex, nIndex;

	Float3Type* vertices;
	Float3Type* texcoords;
	Float3Type* normals;
	FaceType* faces;
};

#endif