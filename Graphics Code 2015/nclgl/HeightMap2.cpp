#include "HeightMap2.h"

HeightMap2::HeightMap2(std::string name)
{
	std::ifstream file(name.c_str(), ios::binary);
	if (!file) {
		return;

	}
	numVertices = RAW_WIDTH2 * RAW_HEIGHT2;
	numIndices = (RAW_WIDTH2 - 1)*(RAW_HEIGHT2 - 1) * 6;
	vertices = new Vector3[numVertices];
	textureCoords = new Vector2[numVertices];
	indices = new GLuint[numIndices];

	unsigned char * data = new unsigned char[numVertices];
	file.read((char *)data, numVertices * sizeof(unsigned char));
	file.close();

	for (int x = 0; x < RAW_WIDTH2; ++x) {
		for (int z = 0; z < RAW_HEIGHT2; ++z) {
			int offset = (x * RAW_WIDTH2) + z;

			vertices[offset] = Vector3(x * HEIGHTMAP2_X, data[offset] * HEIGHTMAP2_Y, z * HEIGHTMAP2_Z);

			textureCoords[offset] = Vector2(x * HEIGHTMAP2_TEX_X, z * HEIGHTMAP2_TEX_Z);
		}
	}
	delete data;

	numIndices = 0;

	for (int x = 0; x < RAW_WIDTH2 - 1; ++x) {
		for (int z = 0; z < RAW_HEIGHT2 - 1; ++z) {
			int a = (x * (RAW_WIDTH2)) + z;
			int b = ((x + 1) * (RAW_WIDTH2)) + z;
			int c = ((x + 1) * (RAW_WIDTH2)) + (z + 1);
			int d = (x * (RAW_WIDTH2)) + (z + 1);

			indices[numIndices++] = c;
			indices[numIndices++] = b;
			indices[numIndices++] = a;

			indices[numIndices++] = a;
			indices[numIndices++] = d;
			indices[numIndices++] = c;
		}
	}
	GenerateNormals();
	GenerateTangents();
	BufferData();

}

