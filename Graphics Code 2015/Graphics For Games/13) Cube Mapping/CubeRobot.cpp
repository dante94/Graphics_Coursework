#include "CubeRobot.h"

//The CubeRobot constructor builds up the spaceship out of cube Meshes, and child SceneNodes, using
//the SceneNode scale variable to set the size and shape of the cube, and the local transform to set the
//position offset of the limbs

Mesh* CubeRobot::cube = NULL;

CubeRobot::CubeRobot(void)
{
	//Optianal, uncomment if you want a local origin marker!
	//SetMesh(cube);

	SceneNode*body = new SceneNode(cube, Vector4(0, 0, 0, 1));	
	body->SetModelScale(Vector3(2000, 350, 800));
	body->SetTransform(Matrix4::Translation(Vector3(0, 0, 0)));
	body->SetMesh(cube);
	AddChild(body);

	tower = new SceneNode(cube, Vector4(0, 0, 1, 1));
	tower->SetModelScale(Vector3(500, 200, 700));
	tower->SetTransform(Matrix4::Translation(Vector3(600, 700, 0)));
	tower->SetMesh(cube);
	body->AddChild(tower);

	body2 = new SceneNode(cube, Vector4(0, 0, 0, 1));
	body2->SetModelScale(Vector3(500, 350, 500));
	body2->SetTransform(Matrix4::Translation(Vector3(1500, 0, 1310)));
	body2->SetMesh(cube);
	body->AddChild(body2);

	body3 = new SceneNode(cube, Vector4(0, 0, 0, 1));	
	body3->SetModelScale(Vector3(500, 350, 500));
	body3->SetTransform(Matrix4::Translation(Vector3(1500, 0, -1310)));
	body3->SetMesh(cube);
	body->AddChild(body3);

	SceneNode* rightTurbine = new SceneNode(cube, Vector4(1, 0, 0, 1));
	rightTurbine->SetModelScale(Vector3(125, 200, 300));
	rightTurbine->SetTransform(Matrix4::Translation(Vector3(2100, 200, -1300)));
	rightTurbine->SetMesh(cube);
	body->AddChild(rightTurbine);

	SceneNode* leftTurbine = new SceneNode(cube, Vector4(1, 0, 0, 1));
	leftTurbine->SetModelScale(Vector3(125, 200, 300));
	leftTurbine->SetTransform(Matrix4::Translation(Vector3(2100, 200, 1300)));
	leftTurbine->SetMesh(cube);
	body->AddChild(leftTurbine);

	SceneNode* rightGun = new SceneNode(cube, Vector4(0, 1, 0, 1));
	rightGun->SetModelScale(Vector3(600, 150, 250));
	rightGun->SetTransform(Matrix4::Translation(Vector3(395, 200, -1300)));
	rightGun->SetMesh(cube);
	body->AddChild(rightGun);

	SceneNode* leftGun = new SceneNode(cube, Vector4(0, 1, 0, 1));
	leftGun->SetModelScale(Vector3(600, 150, 250));
	leftGun->SetTransform(Matrix4::Translation(Vector3(395, 200, 1300)));
	leftGun->SetMesh(cube);
	body->AddChild(leftGun);
}

// We make our spaceship move on constantly the x axis
void CubeRobot::Update(float msec)
{
	transform = transform * Matrix4::Translation(Vector3(-6, 0, 0));
	SceneNode::Update(msec);
}
