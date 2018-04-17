#pragma once

#include <string>
#include <iostream>
#include <fstream>

#include "../nclgl/Mesh.h"

#define RAW_WIDTH3	257
#define RAW_HEIGHT3	257

#define HEIGHTMAP3_X	50.0f
#define HEIGHTMAP3_Z	50.0f
#define HEIGHTMAP3_Y	6.5f
#define HEIGHTMAP3_TEX_X 1.0f / 16.0f
#define HEIGHTMAP3_TEX_Z 1.0f / 16.0f

class HeightMap3 : public Mesh
{
public:
	HeightMap3(std::string name);
	~HeightMap3(void) {};
};

