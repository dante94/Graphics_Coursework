#pragma once

#include "../../nclgl/OGLRenderer.h"
#include "../../nclgl/Camera.h"
#include "../../nclgl/HeightMap.h"
#include "../../nclgl/HeightMap2.h"
#include "../../nclgl/HeightMap3.h"
#include "../../nclgl/SceneNode.h"
#include "../../nclgl/MD5Mesh.h"
#include "../../nclgl/MD5Node.h"
#include "CubeRobot.h"
#include "Textmesh.h"
#include "ParticleEmitter.h"

# define SHADOWSIZE 2048 // New !

class Renderer : public OGLRenderer {
public:
	Renderer(Window & parent);
	virtual ~Renderer(void);
		
	virtual void RenderScene();
	virtual void UpdateScene(float msec);

	int scene;

	void DrawText(const std::string &text, const Vector3 &position, const float size = 10.0f, const bool perspective = false);
	void CameraForward();
	void CycleScenes(float msec);

	void Renderer::SetMsec(float msec);
protected:
	int counter;
	void DrawHeightmap();
	void DrawWater();
	void DrawSkybox();
	void DrawNode(SceneNode * n);

	void DrawMesh(); // New !
	void DrawShadowScene(); // New !
	void DrawCombinedScene(); // New !

	Shader * sceneShader;
	Shader * shadowShader;

	GLuint shadowTex;
	GLuint shadowFBO;

	MD5FileData * hellData;
	MD5Node * hellNode;

	

	void	SetShaderParticleSize(float f);

	ParticleEmitter* emitter;
	ParticleEmitter* emitter2;
				
	Shader * lightShader;
	Shader * lightShader2;
	Shader * reflectShader;
	Shader * skyboxShader;
	Shader * fontShader;
	Shader * particleShader;
	Shader * cubeShader;
				
	HeightMap * heightMap;
	HeightMap2 * heightMap1;
	HeightMap3 * heightMap2;

	Mesh * quad;
	Mesh * quad2;
	Mesh * quad3;
				
	Light * light;
	Light * light2;
	Light * light3;

	Camera * camera;
	Camera * camera2;
	Camera * camera3;
				
	GLuint cubeMap;
	GLuint cubeMap2;
	GLuint cubeMap3;

	Font*	basicFont;
	SceneNode * root;

	float waterRotate;
	float msec2;
	
	float ms=0;
};