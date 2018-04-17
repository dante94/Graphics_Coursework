#include "HeightMap3.h"

HeightMap3::HeightMap3(std::string name)
{
	std::ifstream file(name.c_str(), ios::binary);
	if (!file) {
		return;

	}
	numVertices = RAW_WIDTH3 * RAW_HEIGHT3;
	numIndices = (RAW_WIDTH3 - 1)*(RAW_HEIGHT3 - 1) * 6;
	vertices = new Vector3[numVertices];
	textureCoords = new Vector2[numVertices];
	indices = new GLuint[numIndices];

	unsigned char * data = new unsigned char[numVertices];
	file.read((char *)data, numVertices * sizeof(unsigned char));
	file.close();

	for (int x = 0; x < RAW_WIDTH3; ++x) {
		for (int z = 0; z < RAW_HEIGHT3; ++z) {
			int offset = (x * RAW_WIDTH3) + z;

			vertices[offset] = Vector3(x * HEIGHTMAP3_X, data[offset] * HEIGHTMAP3_Y, z * HEIGHTMAP3_Z);

			textureCoords[offset] = Vector2(x * HEIGHTMAP3_TEX_X, z * HEIGHTMAP3_TEX_Z);
		}
	}
	delete data;

	numIndices = 0;

	for (int x = 0; x < RAW_WIDTH3 - 1; ++x) {
		for (int z = 0; z < RAW_HEIGHT3 - 1; ++z) {
			int a = (x * (RAW_WIDTH3)) + z;
			int b = ((x + 1) * (RAW_WIDTH3)) + z;
			int c = ((x + 1) * (RAW_WIDTH3)) + (z + 1);
			int d = (x * (RAW_WIDTH3)) + (z + 1);

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


