#pragma once

#include <string>
#include <iostream>
#include <fstream>

#include "../nclgl/Mesh.h"

#define RAW_WIDTH2	257
#define RAW_HEIGHT2	257

#define HEIGHTMAP2_X	50.0f
#define HEIGHTMAP2_Z	50.0f
#define HEIGHTMAP2_Y	3.5f
#define HEIGHTMAP2_TEX_X 1.0f / 16.0f
#define HEIGHTMAP2_TEX_Z 1.0f / 16.0f

class HeightMap2 : public Mesh
{
public:
	HeightMap2(std::string name);
	~HeightMap2(void) {};
};

