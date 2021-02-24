#include "TerrainClass.h"

TerrainClass::TerrainClass()
{
	m_vertexBuffer = nullptr;
	m_indexBuffer = nullptr;
	m_terrainFilename = nullptr;
	m_heightMap = nullptr;
	m_terrainModel = nullptr;
}

TerrainClass::TerrainClass(const TerrainClass& other)
{
}

TerrainClass::~TerrainClass()
{
}

bool TerrainClass::Initialize(ID3D11Device* device, const char* setupFilename, TextureManagerClass* TextureManager)
{
	bool result;

	result = LoadSetupFile(setupFilename);
	if (!result)
	{
		return false;
	}

	TextureManager->InitializeTerrainTextures(m_texturesCount, m_textureIndexes);

	result = GenerateTerrain(device);
	if (!result)
	{
		return false;
	}

	return true;
}

void TerrainClass::Shutdown()
{
	ShutdownBuffers();

	ShutdownTerrainModel();

	ShutdownHeightMap();

	delete m_textureIndexes;
	delete m_biomHeights;
	delete m_amplitudes;
	delete m_generator;
	delete m_displacement;

	return;
}

void TerrainClass::ShutdownTerrainModel()
{
	// Release the terrain model data.
	if (m_terrainModel)
	{
		delete[] m_terrainModel;
		m_terrainModel = nullptr;
	}

	return;
}

void TerrainClass::ShutdownHeightMap()
{
	// Release the height map array.
	if (m_heightMap)
	{
		delete[] m_heightMap;
		m_heightMap = nullptr;
	}

	return;
}

bool TerrainClass::LoadSetupFile(const char* filename)
{
	std::ifstream fin;
	char input;
	float value;

	fin.open(filename);
	if (fin.fail())
	{
		return false;
	}

	// Terrain Height:
	fin.get(input);
	while (input != ':')
	{
		fin.get(input);
	}

	fin >> m_terrainHeight;

	// Terrain Width:
	fin.get(input);
	while (input != ':')
	{
		fin.get(input);
	}

	fin >> m_terrainWidth;

	// Terrain Height Scaling:
	fin.get(input);
	while (input != ':')
	{
		fin.get(input);
	}

	fin >> m_heightScale;

	// Textures Count:
	fin.get(input);
	while (input != ':')
	{
		fin.get(input);
	}

	fin >> m_texturesCount;

	m_textureIndexes = new int[m_texturesCount];
	m_biomHeights = new D3DXVECTOR4[m_texturesCount];

	// Texture Indexes[Textures Count]:
	fin.get(input);
	while (input != ':')
	{
		fin.get(input);
	}

	for (int i = 0; i < m_texturesCount; i++)
	{
		fin >> m_textureIndexes[i];
	}

	// Texture Biome Heights[Textures Count]:
	fin.get(input);
	while (input != ':')
	{
		fin.get(input);
	}

	for (int i = 0; i < m_texturesCount; i++)
	{
		fin >> value;
		m_biomHeights[i] = D3DXVECTOR4(value, value, value, value);
	}

	// Seed (default = -1):
	fin.get(input);
	while (input != ':')
	{
		fin.get(input);
	}

	fin >> m_seed;

	// Displacement[2]:
	fin.get(input);
	while (input != ':')
	{
		fin.get(input);
	}

	m_displacement = new float[2];

	fin >> m_displacement[0] >> m_displacement[1];

	// Frequency:
	fin.get(input);
	while (input != ':')
	{
		fin.get(input);
	}

	fin >> m_frequency;

	// Octaves/Layers:
	fin.get(input);
	while (input != ':')
	{
		fin.get(input);
	}

	fin >> m_octaves;

	m_amplitudes = new float[m_octaves];

	// Amplitudes[Octaves/Layers]:
	fin.get(input);
	while (input != ':')
	{
		fin.get(input);
	}

	for (int i = 0; i < m_octaves; i++)
	{
		fin >> m_amplitudes[i];
	}

	// Terrace Smoothness:
	fin.get(input);
	while (input != ':')
	{
		fin.get(input);
	}

	fin >> m_terraceSmoothnessFactor;

	// Exponent:
	fin.get(input);
	while (input != ':')
	{
		fin.get(input);
	}

	fin >> m_exponent;

	// Close the setup file.
	fin.close();

	return true;
}

D3DXVECTOR4* TerrainClass::GetBiomHeights()
{
	return m_biomHeights;
}

long TerrainClass::GetGenerationTime()
{
	return m_generationTime;
}

long TerrainClass::GetSeed()
{
	return m_seed;
}

void TerrainClass::DisableSeed(ID3D11Device* device)
{
	if (m_seed == -1)
		return;

	m_seed = -1;

	RegenerateTerrain(device);
}

void TerrainClass::IncreaseSeed(ID3D11Device* device)
{
	m_seed += 1;

	RegenerateTerrain(device);
}

void TerrainClass::DecreaseSeed(ID3D11Device* device)
{
	if (m_seed == -1)
		return;

	m_seed -= 1;

	RegenerateTerrain(device);
}

float* TerrainClass::GetDisplacement()
{
	return m_displacement;
}

void TerrainClass::DisableDisplacement(ID3D11Device* device)
{
	m_displacement[0] = 0.0f;
	m_displacement[1] = 0.0f;

	RegenerateTerrain(device);
}

void TerrainClass::IncreaseDisplacementX(ID3D11Device* device)
{
	m_displacement[0] += 0.1f;

	RegenerateTerrain(device);
}

void TerrainClass::IncreaseDisplacementY(ID3D11Device* device)
{
	m_displacement[1] += 0.1f;

	RegenerateTerrain(device);
}

void TerrainClass::DecreaseDisplacementX(ID3D11Device* device)
{
	m_displacement[0] -= 0.1f;

	RegenerateTerrain(device);
}

void TerrainClass::DecreaseDisplacementY(ID3D11Device* device)
{
	m_displacement[1] -= 0.1f;

	RegenerateTerrain(device);
}

float TerrainClass::GetFrequency()
{
	return m_frequency;
}

void TerrainClass::DisableFrequency(ID3D11Device* device)
{
	m_frequency = 2.0f;

	RegenerateTerrain(device);
}

void TerrainClass::IncreaseFrequency(ID3D11Device* device)
{
	m_frequency += 0.25f;

	RegenerateTerrain(device);
}

void TerrainClass::DecreaseFrequency(ID3D11Device* device)
{
	m_frequency -= 0.25f;

	RegenerateTerrain(device);
}

float TerrainClass::GetTerraceSmoothness()
{
	return m_terraceSmoothnessFactor;
}

void TerrainClass::DisableTerraceSmoothness(ID3D11Device* device)
{
	m_terraceSmoothnessFactor = 5.0f;

	RegenerateTerrain(device);
}

void TerrainClass::IncreaseTerraceSmoothness(ID3D11Device* device)
{
	m_terraceSmoothnessFactor += 0.1f;

	RegenerateTerrain(device);
}

void TerrainClass::DecreaseTerraceSmoothness(ID3D11Device* device)
{
	m_terraceSmoothnessFactor -= 0.1f;

	if (m_terraceSmoothnessFactor < 0.05f)
		m_terraceSmoothnessFactor = 0.1f;
	else
		RegenerateTerrain(device);
}

float TerrainClass::GetExponent()
{
	return m_exponent;
}

void TerrainClass::DisableExponent(ID3D11Device* device)
{
	m_exponent = 2.75f;

	RegenerateTerrain(device);
}

void TerrainClass::IncreaseExponent(ID3D11Device* device)
{
	m_exponent += 0.25f;

	RegenerateTerrain(device);
}

void TerrainClass::DecreaseExponent(ID3D11Device* device)
{
	m_exponent -= 0.25f;

	RegenerateTerrain(device);
}

bool TerrainClass::RegenerateTerrain(ID3D11Device* device)
{
	delete m_generator;

	ShutdownTerrainModel();
	ShutdownHeightMap();

	return GenerateTerrain(device);
}

bool TerrainClass::GenerateTerrain(ID3D11Device* device)
{

	auto t_start = std::chrono::high_resolution_clock::now();

	bool result;

	int i, j, k;

	float minHeight = 9999.0f;
	float maxHeight = 0.0f;
	float terraceFactor = 100 * m_terraceSmoothnessFactor;

	m_heightMap = new HeightMapType[m_terrainWidth * m_terrainHeight];
	if (!m_heightMap)
	{
		return false;
	}

	m_generator = m_seed < 0 ? new PerlinNoiseGenerator() : new PerlinNoiseGenerator(m_seed);

	//float amplitudes[4] = { 1.0f, 0.5f, 0.25f, 0.5f };
	//float amplitudes[4] = { 1.0f, 0.5f, 0.25f, 0.125f };

	k = 0;

	// Visit every pixel of the image and assign a color generated with Perlin noise
	for (i = 0; i < m_terrainHeight; ++i) {     // y
		for (j = 0; j < m_terrainWidth; ++j) {  // x

			double x = (double)j / ((double)m_terrainWidth) + m_displacement[0];
			double y = (double)i / ((double)m_terrainHeight) + m_displacement[1];

			double elevation = 0;
			double value = 0;
			float factor = 1.0f;
			float amplitude = 4.0f;
			float divider = 1.0f;

			for (int layer = 0; layer < m_octaves; layer++)
			{
				factor = pow(2, layer) * m_frequency;
				elevation += m_amplitudes[layer] * m_generator->Generate(factor * x, factor * y);
				divider += m_amplitudes[layer];
			}

			elevation /= divider;

			elevation = floor(elevation * terraceFactor) / terraceFactor;

			value = pow(elevation, m_exponent);

			//value = elevation * 10;
			//value = floor(10 * elevation);
			//// Typical Perlin noise
			//double n1 = m_generator->Generate(3 * x, 6 * y);
			//double n2 = m_generator->Generate(2 * x, 2 * y);
			//double n3 = m_generator->Generate(4 * x, 4 * y);
			//double n4 = m_generator->Generate(8 * x, 4 * y);

			////double n = (n1 + n2 + n3 + n4) / 4;// *sin(x)* sin(x) / 4;

			////double n = m_generator->Generate(10 * x, 10 * y, 0.8);

			//// Wood like structure
			//double n = 20 * m_generator->Generate(frequency * x, frequency * y);
			//n = n - floor(n);

			//// Map the values to the [0, 255] interval, for simplicity we use 
			//// tones of grey

			//double currentHeight = 255 * n;
			//double value;

			//if (currentHeight < 80)
			//{
			//	n = 8 * floor(n * 99.0f) / 100.0f;
			//	//n = floor(abs(sin(n * n) * 10.0f)) / abs(sin(n) * 15.0f);
			//	value = floor(255 * n);
			//}
			//else
			//{
			//}
			//value = floor(currentHeight);		

			if (value < minHeight)
				minHeight = value;
			if (value > maxHeight)
				maxHeight = value;

			//m_heightMap[k].x = value;
			m_heightMap[k].y = value;
			//m_heightMap[k].z = value;
			k++;
		}
	}

	int newMinHeight = 0;
	int newMaxHeight = 255;
	float value;

	k = 0;

	for (i = 0; i < m_terrainHeight; ++i)
	{     // y
		for (j = 0; j < m_terrainWidth; ++j)
		{  // x
			value = m_heightMap[k].y;

			// skalowanie wysokosci
			double percent = (value - minHeight) / (maxHeight - minHeight);
			int diff = (newMaxHeight - newMinHeight);
			value = floor((percent * diff) + newMinHeight);
			
			m_heightMap[k].y = value;
			k++;
		}
	}

	SetTerrainCoordinates();

	result = CalculateNormals();
	if (!result)
	{
		return false;
	}

	result = BuildTerrainModel();
	if (!result)
	{
		return false;
	}

	result = InitializeBuffers(device);
	if (!result)
	{
		return false;
	}

	auto t_end = std::chrono::high_resolution_clock::now();

	m_generationTime = std::chrono::duration_cast<std::chrono::milliseconds>(t_end - t_start).count();

	return true;
}

void TerrainClass::SetTerrainCoordinates()
{
	int i, j, index;

	// Loop through all the elements in the height map array and adjust their coordinates correctly.
	for (j = 0; j < m_terrainHeight; j++)
	{
		for (i = 0; i < m_terrainWidth; i++)
		{
			index = (m_terrainWidth * j) + i;

			// Set the X and Z coordinates.
			m_heightMap[index].x = (float)i;
			m_heightMap[index].z = -(float)j;

			// Move the terrain depth into the positive range.  For example from (0, -256) to (256, 0).
			m_heightMap[index].z += (float)(m_terrainHeight - 1);

			// Scale the height.
			m_heightMap[index].y /= m_heightScale;
		}
	}

	return;
}

bool TerrainClass::CalculateNormals()
{
	int i, j, index1, index2, index3, index;
	float vertex1[3], vertex2[3], vertex3[3], vector1[3], vector2[3], sum[3], length;
	VectorType* normals;

	// Create a temporary array to hold the face normal vectors.
	normals = new VectorType[(m_terrainHeight - 1) * (m_terrainWidth - 1)];
	if (!normals)
	{
		return false;
	}

	// Go through all the faces in the mesh and calculate their normals.
	for (j = 0; j < (m_terrainHeight - 1); j++)
	{
		for (i = 0; i < (m_terrainWidth - 1); i++)
		{
			index1 = ((j + 1) * m_terrainWidth) + i;      // Bottom left vertex.
			index2 = ((j + 1) * m_terrainWidth) + (i + 1);  // Bottom right vertex.
			index3 = (j * m_terrainWidth) + i;          // Upper left vertex.

			// Get three vertices from the face.
			vertex1[0] = m_heightMap[index1].x;
			vertex1[1] = m_heightMap[index1].y;
			vertex1[2] = m_heightMap[index1].z;

			vertex2[0] = m_heightMap[index2].x;
			vertex2[1] = m_heightMap[index2].y;
			vertex2[2] = m_heightMap[index2].z;

			vertex3[0] = m_heightMap[index3].x;
			vertex3[1] = m_heightMap[index3].y;
			vertex3[2] = m_heightMap[index3].z;

			// Calculate the two vectors for this face.
			vector1[0] = vertex1[0] - vertex3[0];
			vector1[1] = vertex1[1] - vertex3[1];
			vector1[2] = vertex1[2] - vertex3[2];
			vector2[0] = vertex3[0] - vertex2[0];
			vector2[1] = vertex3[1] - vertex2[1];
			vector2[2] = vertex3[2] - vertex2[2];

			index = (j * (m_terrainWidth - 1)) + i;

			// Calculate the cross product of those two vectors to get the un-normalized value for this face normal.
			normals[index].x = (vector1[1] * vector2[2]) - (vector1[2] * vector2[1]);
			normals[index].y = (vector1[2] * vector2[0]) - (vector1[0] * vector2[2]);
			normals[index].z = (vector1[0] * vector2[1]) - (vector1[1] * vector2[0]);

			// Calculate the length.
			length = (float)sqrt((normals[index].x * normals[index].x) + (normals[index].y * normals[index].y) +
				(normals[index].z * normals[index].z));

			// Normalize the final value for this face using the length.
			normals[index].x = (normals[index].x / length);
			normals[index].y = (normals[index].y / length);
			normals[index].z = (normals[index].z / length);
		}
	}

	// Now go through all the vertices and take a sum of the face normals that touch this vertex.
	for (j = 0; j < m_terrainHeight; j++)
	{
		for (i = 0; i < m_terrainWidth; i++)
		{
			// Initialize the sum.
			sum[0] = 0.0f;
			sum[1] = 0.0f;
			sum[2] = 0.0f;

			// Bottom left face.
			if (((i - 1) >= 0) && ((j - 1) >= 0))
			{
				index = ((j - 1) * (m_terrainWidth - 1)) + (i - 1);

				sum[0] += normals[index].x;
				sum[1] += normals[index].y;
				sum[2] += normals[index].z;
			}

			// Bottom right face.
			if ((i < (m_terrainWidth - 1)) && ((j - 1) >= 0))
			{
				index = ((j - 1) * (m_terrainWidth - 1)) + i;

				sum[0] += normals[index].x;
				sum[1] += normals[index].y;
				sum[2] += normals[index].z;
			}

			// Upper left face.
			if (((i - 1) >= 0) && (j < (m_terrainHeight - 1)))
			{
				index = (j * (m_terrainWidth - 1)) + (i - 1);

				sum[0] += normals[index].x;
				sum[1] += normals[index].y;
				sum[2] += normals[index].z;
			}

			// Upper right face.
			if ((i < (m_terrainWidth - 1)) && (j < (m_terrainHeight - 1)))
			{
				index = (j * (m_terrainWidth - 1)) + i;

				sum[0] += normals[index].x;
				sum[1] += normals[index].y;
				sum[2] += normals[index].z;
			}

			// Calculate the length of this normal.
			length = (float)sqrt((sum[0] * sum[0]) + (sum[1] * sum[1]) + (sum[2] * sum[2]));

			// Get an index to the vertex location in the height map array.
			index = (j * m_terrainWidth) + i;

			// Normalize the final shared normal for this vertex and store it in the height map array.
			m_heightMap[index].nx = (sum[0] / length);
			m_heightMap[index].ny = (sum[1] / length);
			m_heightMap[index].nz = (sum[2] / length);
		}
	}

	// Release the temporary normals.
	delete[] normals;
	normals = nullptr;

	return true;
}

bool TerrainClass::BuildTerrainModel()
{
	int i, j, index, index1, index2, index3, index4;

	// Calculate the number of vertices in the 3D terrain model.
	m_vertexCount = (m_terrainHeight - 1) * (m_terrainWidth - 1) * 6;

	// Create the 3D terrain model array.
	m_terrainModel = new ModelType[m_vertexCount];
	if (!m_terrainModel)
	{
		return false;
	}

	// Initialize the index into the height map array.
	index = 0;

	// Load the 3D terrain model with the height map terrain data.
	// We will be creating 2 triangles for each of the four points in a quad.
	for (j = 0; j < (m_terrainHeight - 1); j++)
	{
		for (i = 0; i < (m_terrainWidth - 1); i++)
		{
			// Get the indexes to the four points of the quad.
			index1 = (m_terrainWidth * j) + i;          // Upper left.
			index2 = (m_terrainWidth * j) + (i + 1);      // Upper right.
			index3 = (m_terrainWidth * (j + 1)) + i;      // Bottom left.
			index4 = (m_terrainWidth * (j + 1)) + (i + 1);  // Bottom right.

			// Now create two triangles for that quad.
			// Triangle 1 - Upper left.
			m_terrainModel[index].x = m_heightMap[index1].x;
			m_terrainModel[index].y = m_heightMap[index1].y;
			m_terrainModel[index].z = m_heightMap[index1].z;
			m_terrainModel[index].tu = 0.0f;
			m_terrainModel[index].tv = 0.0f;
			m_terrainModel[index].nx = m_heightMap[index1].nx;
			m_terrainModel[index].ny = m_heightMap[index1].ny;
			m_terrainModel[index].nz = m_heightMap[index1].nz;
			index++;

			// Triangle 1 - Upper right.
			m_terrainModel[index].x = m_heightMap[index2].x;
			m_terrainModel[index].y = m_heightMap[index2].y;
			m_terrainModel[index].z = m_heightMap[index2].z;
			m_terrainModel[index].tu = 1.0f;
			m_terrainModel[index].tv = 0.0f;
			m_terrainModel[index].nx = m_heightMap[index2].nx;
			m_terrainModel[index].ny = m_heightMap[index2].ny;
			m_terrainModel[index].nz = m_heightMap[index2].nz;
			index++;

			// Triangle 1 - Bottom left.
			m_terrainModel[index].x = m_heightMap[index3].x;
			m_terrainModel[index].y = m_heightMap[index3].y;
			m_terrainModel[index].z = m_heightMap[index3].z;
			m_terrainModel[index].tu = 0.0f;
			m_terrainModel[index].tv = 1.0f;
			m_terrainModel[index].nx = m_heightMap[index3].nx;
			m_terrainModel[index].ny = m_heightMap[index3].ny;
			m_terrainModel[index].nz = m_heightMap[index3].nz;
			index++;

			// Triangle 2 - Bottom left.
			m_terrainModel[index].x = m_heightMap[index3].x;
			m_terrainModel[index].y = m_heightMap[index3].y;
			m_terrainModel[index].z = m_heightMap[index3].z;
			m_terrainModel[index].tu = 0.0f;
			m_terrainModel[index].tv = 1.0f;
			m_terrainModel[index].nx = m_heightMap[index3].nx;
			m_terrainModel[index].ny = m_heightMap[index3].ny;
			m_terrainModel[index].nz = m_heightMap[index3].nz;
			index++;

			// Triangle 2 - Upper right.
			m_terrainModel[index].x = m_heightMap[index2].x;
			m_terrainModel[index].y = m_heightMap[index2].y;
			m_terrainModel[index].z = m_heightMap[index2].z;
			m_terrainModel[index].tu = 1.0f;
			m_terrainModel[index].tv = 0.0f;
			m_terrainModel[index].nx = m_heightMap[index2].nx;
			m_terrainModel[index].ny = m_heightMap[index2].ny;
			m_terrainModel[index].nz = m_heightMap[index2].nz;
			index++;

			// Triangle 2 - Bottom right.
			m_terrainModel[index].x = m_heightMap[index4].x;
			m_terrainModel[index].y = m_heightMap[index4].y;
			m_terrainModel[index].z = m_heightMap[index4].z;
			m_terrainModel[index].tu = 1.0f;
			m_terrainModel[index].tv = 1.0f;
			m_terrainModel[index].nx = m_heightMap[index4].nx;
			m_terrainModel[index].ny = m_heightMap[index4].ny;
			m_terrainModel[index].nz = m_heightMap[index4].nz;
			index++;
		}
	}

	return true;
}

void TerrainClass::Render(ID3D11DeviceContext* deviceContext)
{
	// Put the vertex and index buffers on the graphics pipeline to prepare them for drawing.
	RenderBuffers(deviceContext);

	return;
}

int TerrainClass::GetIndexCount()
{
	return m_indexCount;
}

bool TerrainClass::InitializeBuffers(ID3D11Device* device)
{
	VertexType* vertices;
	unsigned long* indices;
	D3D11_BUFFER_DESC vertexBufferDesc, indexBufferDesc;
	D3D11_SUBRESOURCE_DATA vertexData, indexData;
	HRESULT result;
	int i;
	D3DXVECTOR4 color;

	color = D3DXVECTOR4(1.0f, 1.0f, 1.0f, 1.0f);

	// Calculate the number of vertices in the terrain mesh.
	m_vertexCount = (m_terrainWidth - 1) * (m_terrainHeight - 1) * 6;

	// Set the index count to the same as the vertex count.
	m_indexCount = m_vertexCount;

	// Create the vertex array.
	vertices = new VertexType[m_vertexCount];
	if (!vertices)
	{
		return false;
	}

	// Create the index array.
	indices = new unsigned long[m_indexCount];
	if (!indices)
	{
		return false;
	}

	// Load the vertex array and index array with 3D terrain model data.
	for (i = 0; i < m_vertexCount; i++)
	{
		vertices[i].position = D3DXVECTOR3(m_terrainModel[i].x, m_terrainModel[i].y, m_terrainModel[i].z);
		vertices[i].texture = D3DXVECTOR2(m_terrainModel[i].tu, m_terrainModel[i].tv);
		vertices[i].normal = D3DXVECTOR3(m_terrainModel[i].nx, m_terrainModel[i].ny, m_terrainModel[i].nz);
		indices[i] = i;
	}

	// Set up the description of the static vertex buffer.
	vertexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	vertexBufferDesc.ByteWidth = sizeof(VertexType) * m_vertexCount;
	vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vertexBufferDesc.CPUAccessFlags = 0;
	vertexBufferDesc.MiscFlags = 0;
	vertexBufferDesc.StructureByteStride = 0;

	// Give the subresource structure a pointer to the vertex data.
	vertexData.pSysMem = vertices;
	vertexData.SysMemPitch = 0;
	vertexData.SysMemSlicePitch = 0;

	// Now create the vertex buffer.
	result = device->CreateBuffer(&vertexBufferDesc, &vertexData, &m_vertexBuffer);
	if (FAILED(result))
	{
		return false;
	}

	// Set up the description of the static index buffer.
	indexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	indexBufferDesc.ByteWidth = sizeof(unsigned long) * m_indexCount;
	indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	indexBufferDesc.CPUAccessFlags = 0;
	indexBufferDesc.MiscFlags = 0;
	indexBufferDesc.StructureByteStride = 0;

	// Give the subresource structure a pointer to the index data.
	indexData.pSysMem = indices;
	indexData.SysMemPitch = 0;
	indexData.SysMemSlicePitch = 0;

	// Create the index buffer.
	result = device->CreateBuffer(&indexBufferDesc, &indexData, &m_indexBuffer);
	if (FAILED(result))
	{
		return false;
	}

	// Release the arrays now that the buffers have been created and loaded.
	delete[] vertices;
	vertices = nullptr;

	delete[] indices;
	indices = nullptr;

	return true;
}

void TerrainClass::ShutdownBuffers()
{
	// Release the index buffer.
	if (m_indexBuffer)
	{
		m_indexBuffer->Release();
		m_indexBuffer = nullptr;
	}

	// Release the vertex buffer.
	if (m_vertexBuffer)
	{
		m_vertexBuffer->Release();
		m_vertexBuffer = nullptr;
	}

	return;
}

void TerrainClass::RenderBuffers(ID3D11DeviceContext* deviceContext)
{
	unsigned int stride;
	unsigned int offset;

	// Set vertex buffer stride and offset.
	stride = sizeof(VertexType);
	offset = 0;

	// Set the vertex buffer to active in the input assembler so it can be rendered.
	deviceContext->IASetVertexBuffers(0, 1, &m_vertexBuffer, &stride, &offset);

	// Set the index buffer to active in the input assembler so it can be rendered.
	deviceContext->IASetIndexBuffer(m_indexBuffer, DXGI_FORMAT_R32_UINT, 0);

	// Set the type of primitive that should be rendered from this vertex buffer, in this case a line list.
	deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	return;
}
