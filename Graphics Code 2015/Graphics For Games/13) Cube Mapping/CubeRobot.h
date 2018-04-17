#pragma once
#include "../../nclgl/SceneNode.h"
#include "../../nclgl/OBJMesh.h"

//We overload the Update function to handle simple animation and we load the cube
//as a mesh geometry
class CubeRobot : public SceneNode {
public:
	CubeRobot(void);
	~CubeRobot(void) {};
	virtual void Update(float msec);
	static void CreateCube() {
	OBJMesh * m = new OBJMesh();
	m -> LoadOBJMesh(MESHDIR "cube.obj");
	cube = m;
	}

	static void DeleteCube() { delete cube; }

protected:
	static Mesh * cube;
	static Mesh * particle;
	SceneNode * body2;
	SceneNode * body3;
	SceneNode * tower;
};