#include "OBJLoader.h"

OBJLoader::OBJLoader()
{
	Initialize();
}

OBJLoader::~OBJLoader()
{
}

unsigned long OBJLoader::GetIndiciesCount()
{
	return indiciesCount;
}

unsigned long OBJLoader::GetVerticesCount()
{
	return vertexCount;
}

void OBJLoader::Initialize()
{
	vertexCount = 0;
	textureCount = 0;
	normalCount = 0;
	faceCount = 0;
	indiciesCount = 0;
	vertexIndex = 0;
	texcoordIndex = 0;
	normalIndex = 0;
	faceIndex = 0;
	vIndex = 0;
	tIndex = 0;
	nIndex = 0;
}

bool OBJLoader::LoadObj(ModelType** model, const char* filename, bool conversionNeeded)
{
	bool result;

	Initialize();

	result = OpenFileStream(&fin, filename);
	if (!result)
	{
		return result;
	}

	result = InitDataStructures();
	if (!result)
	{
		return result;
	}

	result = FillDataStructures(conversionNeeded);
	if (!result)
	{
		return result;
	}

	result = SeedModel(model);
	if (!result)
	{
		return result;
	}

	result = ClearDataStructures();
	if (!result)
	{
		return result;
	}

	result = CloseFileStream(&fin);
	if (!result)
	{
		return result;
	}

	return result;
}

bool OBJLoader::OpenFileStream(std::ifstream* fin, const char* filename)
{
	fin->open(filename);

	// Check if it was successful in opening the file.
	if (fin->fail() == true)
	{
		return false;
	}

	return true;
}

bool OBJLoader::OpenFileStream(std::ofstream* fout, const char* filename)
{
	fout->open(filename);

	// Check if it was successful in opening the file.
	if (fout->fail() == true)
	{
		return false;
	}

	return true;
}

bool OBJLoader::CloseFileStream(std::ifstream* fin)
{
	fin->close();

	return true;
}

bool OBJLoader::CloseFileStream(std::ofstream* fout)
{
	fout->close();

	return true;
}

bool OBJLoader::InitDataStructures()
{
	char input;

	// Read from the file and continue to read until the end of the file is reached.
	fin.get(input);

	while (!fin.eof())
	{
		// If the line starts with 'v' then count either the vertex, the texture coordinates, or the normal vector.
		if (input == 'v')
		{
			fin.get(input);
			if (input == ' ') { vertexCount++; }
			if (input == 't') { textureCount++; }
			if (input == 'n') { normalCount++; }
		}

		// If the line starts with 'f' then increment the face count.
		if (input == 'f')
		{
			fin.get(input);
			if (input == ' ') { faceCount++; }
		}

		// Otherwise read in the remainder of the line.
		while (input != '\n')
		{
			fin.get(input);
		}

		// Start reading the beginning of the next line.
		fin.get(input);
	}

	fin.clear();                 // clear fail and eof bits
	fin.seekg(0, std::ios::beg); // back to the start!

	// Initialize the four data structures.
	vertices = new Float3Type[vertexCount]();
	if (!vertices)
	{
		return false;
	}

	texcoords = new Float3Type[textureCount]();
	if (!texcoords)
	{
		return false;
	}

	normals = new Float3Type[normalCount]();
	if (!normals)
	{
		return false;
	}

	faces = new FaceType[faceCount]();
	if (!faces)
	{
		return false;
	}

	indiciesCount = faceCount * 3;
}

bool OBJLoader::FillDataStructures(bool switchToLHSystem)
{
	char input, input2;

	// Check if it was successful in opening the file.
	if (fin.fail() == true)
	{
		return false;
	}

	// Read in the vertices, texture coordinates, and normals into the data structures.
	// Important: Also convert to left hand coordinate system since Maya uses right hand coordinate system.
	fin.get(input);
	while (!fin.eof())
	{
		if (input == 'v')
		{
			fin.get(input);

			// Read in the vertices.
			if (input == ' ')
			{
				fin >> vertices[vertexIndex].x >> vertices[vertexIndex].y >> vertices[vertexIndex].z;

				// Invert the Z vertex to change to left hand system.
				if (switchToLHSystem)
					vertices[vertexIndex].z = vertices[vertexIndex].z * -1.0f;

				vertexIndex++;
			}

			// Read in the texture uv coordinates.
			if (input == 't')
			{
				fin >> texcoords[texcoordIndex].x >> texcoords[texcoordIndex].y;

				// Invert the V texture coordinates to left hand system.
				if (switchToLHSystem)
					texcoords[texcoordIndex].y = 1.0f - texcoords[texcoordIndex].y;

				texcoordIndex++;
			}

			// Read in the normals.
			if (input == 'n')
			{
				fin >> normals[normalIndex].x >> normals[normalIndex].y >> normals[normalIndex].z;

				// Invert the Z normal to change to left hand system.
				if (switchToLHSystem)
					normals[normalIndex].z = normals[normalIndex].z * -1.0f;

				normalIndex++;
			}
		}

		// Read in the faces.
		if (input == 'f')
		{
			fin.get(input);
			if (input == ' ')
			{

				// Read the face data in backwards to convert it to a left hand system from right hand system.
				if (switchToLHSystem)
					fin >> faces[faceIndex].vIndex3 >> input2 >> faces[faceIndex].tIndex3 >> input2 >> faces[faceIndex].nIndex3
						>> faces[faceIndex].vIndex2 >> input2 >> faces[faceIndex].tIndex2 >> input2 >> faces[faceIndex].nIndex2
						>> faces[faceIndex].vIndex1 >> input2 >> faces[faceIndex].tIndex1 >> input2 >> faces[faceIndex].nIndex1;
				else
					fin >> faces[faceIndex].vIndex1 >> input2 >> faces[faceIndex].tIndex1 >> input2 >> faces[faceIndex].nIndex1
						>> faces[faceIndex].vIndex2 >> input2 >> faces[faceIndex].tIndex2 >> input2 >> faces[faceIndex].nIndex2
						>> faces[faceIndex].vIndex3 >> input2 >> faces[faceIndex].tIndex3 >> input2 >> faces[faceIndex].nIndex3;

				faceIndex++;
			}
		}

		// Read in the remainder of the line.
		while (input != '\n')
		{
			fin.get(input);
		}

		// Start reading the beginning of the next line.
		fin.get(input);
	}

	return true;
}

bool OBJLoader::SeedModel(ModelType** model)
{
	if (*model != nullptr)
	{
		delete[] * model;
		*model = nullptr;
	}

	ModelType* modelCopy = new ModelType[indiciesCount];
	memset(modelCopy, 0, indiciesCount);

	// Now loop through all the faces and output the three vertices for each face.
	for (int i = 0, j = 0; i < faceCount; i++, j++)
	{
		vIndex = faces[i].vIndex1 - 1;
		tIndex = faces[i].tIndex1 - 1;
		nIndex = faces[i].nIndex1 - 1;

		(modelCopy + j)->x = vertices[vIndex].x;
		(modelCopy + j)->y = vertices[vIndex].y;
		(modelCopy + j)->z = vertices[vIndex].z;
		(modelCopy + j)->tu = texcoords[tIndex].x;
		(modelCopy + j)->tv = texcoords[tIndex].y;
		(modelCopy + j)->nx = normals[nIndex].x;
		(modelCopy + j)->ny = normals[nIndex].y;
		(modelCopy + j)->nz = normals[nIndex].z;

		j++;

		vIndex = faces[i].vIndex2 - 1;
		tIndex = faces[i].tIndex2 - 1;
		nIndex = faces[i].nIndex2 - 1;

		(modelCopy + j)->x = vertices[vIndex].x;
		(modelCopy + j)->y = vertices[vIndex].y;
		(modelCopy + j)->z = vertices[vIndex].z;
		(modelCopy + j)->tu = texcoords[tIndex].x;
		(modelCopy + j)->tv = texcoords[tIndex].y;
		(modelCopy + j)->nx = normals[nIndex].x;
		(modelCopy + j)->ny = normals[nIndex].y;
		(modelCopy + j)->nz = normals[nIndex].z;

		j++;

		vIndex = faces[i].vIndex3 - 1;
		tIndex = faces[i].tIndex3 - 1;
		nIndex = faces[i].nIndex3 - 1;

		(modelCopy + j)->x = vertices[vIndex].x;
		(modelCopy + j)->y = vertices[vIndex].y;
		(modelCopy + j)->z = vertices[vIndex].z;
		(modelCopy + j)->tu = texcoords[tIndex].x;
		(modelCopy + j)->tv = texcoords[tIndex].y;
		(modelCopy + j)->nx = normals[nIndex].x;
		(modelCopy + j)->ny = normals[nIndex].y;
		(modelCopy + j)->nz = normals[nIndex].z;
	}

	*model = modelCopy;

	return true;
}

bool OBJLoader::ConvertObjToModel(const char* objFileName, const char* modelFileName, bool switchSystem)
{
	bool result;

	Initialize();

	result = OpenFileStream(&fin, objFileName);
	if (!result)
	{
		return result;
	}

	result = InitDataStructures();
	if (!result)
	{
		return result;
	}

	result = FillDataStructures(switchSystem);
	if (!result)
	{
		return result;
	}

	result = CloseFileStream(&fin);
	if (!result)
	{
		return result;
	}

	result = OpenFileStream(&fout, modelFileName);
	if (!result)
	{
		return result;
	}

	result = WriteModelData();
	if (!result)
	{
		return result;
	}

	result = CloseFileStream(&fout);
	if (!result)
	{
		return result;
	}

	return true;
}

bool OBJLoader::WriteModelData()
{
	// Write out the file header that our model format uses.
	fout << "Vertex Count: " << (faceCount * 3) << std::endl;
	fout << std::endl;
	fout << "Data:" << std::endl;
	fout << std::endl;

	// Now loop through all the faces and output the three vertices for each face.
	for (int i = 0; i < faceIndex; i++)
	{
		vIndex = faces[i].vIndex1 - 1;
		tIndex = faces[i].tIndex1 - 1;
		nIndex = faces[i].nIndex1 - 1;

		fout << vertices[vIndex].x << ' ' << vertices[vIndex].y << ' ' << vertices[vIndex].z << ' '
			<< texcoords[tIndex].x << ' ' << texcoords[tIndex].y << ' '
			<< normals[nIndex].x << ' ' << normals[nIndex].y << ' ' << normals[nIndex].z << std::endl;

		vIndex = faces[i].vIndex2 - 1;
		tIndex = faces[i].tIndex2 - 1;
		nIndex = faces[i].nIndex2 - 1;

		fout << vertices[vIndex].x << ' ' << vertices[vIndex].y << ' ' << vertices[vIndex].z << ' '
			<< texcoords[tIndex].x << ' ' << texcoords[tIndex].y << ' '
			<< normals[nIndex].x << ' ' << normals[nIndex].y << ' ' << normals[nIndex].z << std::endl;

		vIndex = faces[i].vIndex3 - 1;
		tIndex = faces[i].tIndex3 - 1;
		nIndex = faces[i].nIndex3 - 1;

		fout << vertices[vIndex].x << ' ' << vertices[vIndex].y << ' ' << vertices[vIndex].z << ' '
			<< texcoords[tIndex].x << ' ' << texcoords[tIndex].y << ' '
			<< normals[nIndex].x << ' ' << normals[nIndex].y << ' ' << normals[nIndex].z << std::endl;
	}

	return true;
}

bool OBJLoader::LoadConvertedModel(ModelType** model, const char* filename)
{
	bool result;

	Initialize();

	result = OpenFileStream(&fin, filename);
	if (!result)
	{
		return result;
	}

	result = LoadModelData(model);
	if (!result)
	{
		return result;
	}

	result = ClearDataStructures();
	if (!result)
	{
		return result;
	}

	result = CloseFileStream(&fin);
	if (!result)
	{
		return result;
	}

	return true;
}

bool OBJLoader::LoadModelData(ModelType** model)
{
	char input;

	// Read up to the value of vertex count.
	fin.get(input);
	while (input != ':')
	{
		fin.get(input);
	}

	// Read in the vertex count.
	fin >> vertexCount;

	// Set the number of indices to be the same as the vertex count.
	indiciesCount = vertexCount;
	faceCount = indiciesCount / 3;

	// Create the model using the vertex count that was read in.
	if (*model != nullptr)
	{
		delete[] * model;
		*model = nullptr;
	}

	ModelType* modelCopy = new ModelType[indiciesCount];
	memset(modelCopy, 0, indiciesCount);

	// Read up to the beginning of the data.
	fin.get(input);

	while (input != ':')
	{
		fin.get(input);
	}

	fin.get(input);
	fin.get(input);

	// Read in the vertex data.
	for (int i = 0; i < indiciesCount; i++)
	{
		fin >> modelCopy[i].x >> modelCopy[i].y >> modelCopy[i].z;
		fin >> modelCopy[i].tu >> modelCopy[i].tv;
		fin >> modelCopy[i].nx >> modelCopy[i].ny >> modelCopy[i].nz;
	}

	*model = modelCopy;

	return true;
}

bool OBJLoader::ClearDataStructures()
{
	// Release the four data structures.
	if (vertices)
	{
		delete[] vertices;
		vertices = nullptr;
	}

	if (texcoords)
	{
		delete[] texcoords;
		texcoords = nullptr;
	}

	if (normals)
	{
		delete[] normals;
		normals = nullptr;
	}

	if (faces)
	{
		delete[] faces;
		faces = nullptr;
	}

	return true;
}