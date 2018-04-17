# include "Renderer.h"

Renderer::Renderer(Window & parent) : OGLRenderer(parent) {
	scene = 1;

	//Create our objects
	CubeRobot::CreateCube(); // Important !

	camera = new Camera();
	camera2 = new Camera();
	camera3 = new Camera();

	quad = Mesh::GenerateQuad();
	quad2 = Mesh::GenerateQuad();
	quad3 = Mesh::GenerateQuad();

	heightMap = new HeightMap(TEXTUREDIR "HeightMap.raw");
	heightMap1 = new HeightMap2(TEXTUREDIR "terrain.raw");
	heightMap2 = new HeightMap3(TEXTUREDIR "HeightMap.raw");

	emitter = new ParticleEmitter();
	emitter2 = new ParticleEmitter();
	
	//Set camera position
	camera->SetPosition(Vector3(RAW_WIDTH * HEIGHTMAP_X / 2.0f - 200.0f, 3000.0f, RAW_WIDTH * HEIGHTMAP_X));
	camera->SetPitch(-30);
	camera2->SetPosition(Vector3(RAW_WIDTH2 * HEIGHTMAP2_X / 2.0f - 200.0f, 3000.0f, RAW_WIDTH2 * HEIGHTMAP2_X));
	camera2->SetPitch(-30);
	camera3->SetPosition(Vector3(RAW_WIDTH3 * HEIGHTMAP3_X / 2.0f + 11000 , 13000.0f, RAW_WIDTH3 * HEIGHTMAP3_X / 2.0f - 800.0f));
	camera3->SetPitch(-90);
	camera3->SetYaw(90);
	//Set light position
	light = new Light(Vector3((RAW_HEIGHT * HEIGHTMAP_X / 2.0f), 1000.0f, (RAW_HEIGHT * HEIGHTMAP_Z / 2.0f)), Vector4(0.9f, 0.9f, 1.0f, 1), (RAW_WIDTH * HEIGHTMAP_X) / 2.0f);
	light2 = new Light(Vector3((RAW_HEIGHT * HEIGHTMAP_X / 2.0f) - 250.0f, 600.0f, (RAW_HEIGHT2 * HEIGHTMAP2_Z / 2.0f)), Vector4(0.9f, 0.9f, 1.0f, 1), (RAW_WIDTH2 * HEIGHTMAP2_X) / 2.0f);
	light3 = new Light(Vector3((RAW_HEIGHT * HEIGHTMAP_X / 2.0f), 1000.0f, (RAW_HEIGHT3 * HEIGHTMAP3_Z / 2.0f)), Vector4(0.9f, 0.9f, 1.0f, 1), (RAW_WIDTH3* HEIGHTMAP3_X) / 2.0f);


	//Create our shaders
	reflectShader = new Shader(SHADERDIR "PerPixelVertex.glsl", SHADERDIR "reflectFragment.glsl");
	skyboxShader = new Shader(SHADERDIR "skyboxVertex.glsl", SHADERDIR "skyboxFragment.glsl");
	lightShader = new Shader(SHADERDIR "PerPixelVertex.glsl", SHADERDIR"PerPixelFragment.glsl");
	lightShader2 = new Shader(SHADERDIR "PerPixelVertex.glsl", SHADERDIR"PerPixelFragment2.glsl");
	fontShader = new Shader(SHADERDIR "TexturedVertex.glsl", SHADERDIR"TexturedFragment.glsl");	//Font shader
	cubeShader = new Shader(SHADERDIR "SceneVertex.glsl", SHADERDIR "SceneFragment.glsl");	//Cube shader
	particleShader = new Shader(SHADERDIR "vertex.glsl", SHADERDIR "fragment.glsl", SHADERDIR "geometry.glsl");

	hellData = new MD5FileData("../../Meshes/hellknight.md5mesh");
	hellNode = new MD5Node(*hellData);

	hellData->AddAnim("../../Meshes/idle2.md5anim");
	hellNode->PlayAnim("../../Meshes/idle2.md5anim");

	sceneShader = new Shader(SHADERDIR "shadowscenevert.glsl", SHADERDIR "shadowscenefrag.glsl");
	shadowShader = new Shader(SHADERDIR "shadowVert.glsl", SHADERDIR "shadowFrag.glsl");

	glGenTextures(1, &shadowTex);
	glBindTexture(GL_TEXTURE_2D, shadowTex);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, SHADOWSIZE, SHADOWSIZE, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_MODE, GL_COMPARE_R_TO_TEXTURE);

	glBindTexture(GL_TEXTURE_2D, 0);

	glGenFramebuffers(1, &shadowFBO);

	glBindFramebuffer(GL_FRAMEBUFFER, shadowFBO);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT,	GL_TEXTURE_2D, shadowTex, 0);
	glDrawBuffer(GL_NONE);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	//Link our shaders
	if (!reflectShader->LinkProgram() || !lightShader->LinkProgram() || !lightShader2->LinkProgram() || !skyboxShader->LinkProgram() || !fontShader->LinkProgram() 
		|| !cubeShader->LinkProgram() || !particleShader->LinkProgram() || !sceneShader->LinkProgram() || !shadowShader->LinkProgram())
	{
		return;
	}
	//Load textures to our objects
	quad->SetTexture(SOIL_load_OGL_texture(TEXTUREDIR "water.JPG", SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS), 0);
	quad2->SetTexture(SOIL_load_OGL_texture(TEXTUREDIR "FrozenWater.JPG", SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS), 0);
	quad3->SetTexture(SOIL_load_OGL_texture(TEXTUREDIR "lava.JPG", SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS), 0);

	heightMap->SetTexture(SOIL_load_OGL_texture(TEXTUREDIR "sand.JPG", SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS), 0);
	heightMap->SetTexture(SOIL_load_OGL_texture(TEXTUREDIR "grass.JPG", SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS), 1);
	heightMap1->SetTexture(SOIL_load_OGL_texture(TEXTUREDIR "SnowGrass.JPG", SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS), 0);
	heightMap2->SetTexture(SOIL_load_OGL_texture(TEXTUREDIR "lavaGround.JPG", SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS), 0);
	

	heightMap->SetBumpMap(SOIL_load_OGL_texture(TEXTUREDIR "sandNormal.JPG", SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS));
	heightMap1->SetBumpMap(SOIL_load_OGL_texture(TEXTUREDIR "GrassBump.JPG", SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS));
	heightMap2->SetBumpMap(SOIL_load_OGL_texture(TEXTUREDIR "lavaBump.JPG", SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS));

	basicFont = new Font(SOIL_load_OGL_texture(TEXTUREDIR"tahoma.tga", SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_COMPRESS_TO_DXT), 16, 16);

	cubeMap = SOIL_load_OGL_cubemap(
		TEXTUREDIR "rusted_west.JPG", TEXTUREDIR "rusted_east.JPG",
		TEXTUREDIR "rusted_up.JPG", TEXTUREDIR "rusted_down.JPG",
		TEXTUREDIR "rusted_south.JPG", TEXTUREDIR "rusted_north.JPG",
		SOIL_LOAD_RGB, SOIL_CREATE_NEW_ID, 0);

	cubeMap2 = SOIL_load_OGL_cubemap(
		TEXTUREDIR "cloudy/graycloud_ft.JPG", TEXTUREDIR "cloudy/graycloud_bk.JPG",
		TEXTUREDIR "cloudy/graycloud_up.JPG", TEXTUREDIR "cloudy/graycloud_dn.JPG",
		TEXTUREDIR "cloudy/graycloud_rt.JPG", TEXTUREDIR "cloudy/graycloud_lf.JPG",
		SOIL_LOAD_RGB, SOIL_CREATE_NEW_ID, 0);

	cubeMap3 = SOIL_load_OGL_cubemap(
		TEXTUREDIR "Nebula/BlueGreenNebula_left.JPG", TEXTUREDIR "Nebula/BlueGreenNebula_right.JPG",
		TEXTUREDIR "Nebula/BlueGreenNebula_top.JPG", TEXTUREDIR "Nebula/BlueGreenNebula_bottom.JPG",
		TEXTUREDIR "Nebula/BlueGreenNebula_front.JPG", TEXTUREDIR "Nebula/BlueGreenNebula_back.JPG",
		SOIL_LOAD_RGB, SOIL_CREATE_NEW_ID, 0);

	if (!cubeMap || !cubeMap2 || !cubeMap3 || !quad->GetTexture(0) || !quad2->GetTexture(0) || !quad3->GetTexture(0) || !heightMap->GetTexture(0)
		|| !heightMap1->GetTexture(0) || !heightMap2->GetTexture(0) || !heightMap->GetTexture(1)
		|| !heightMap->GetBumpMap() || !heightMap1->GetBumpMap() || !heightMap2->GetBumpMap())
	{
		return;
	}

	//Repeat the texture
	SetTextureRepeating(quad->GetTexture(0), true);
	SetTextureRepeating(quad2->GetTexture(0), true);
	SetTextureRepeating(quad3->GetTexture(0), true);
	SetTextureRepeating(heightMap->GetTexture(0), true);
	SetTextureRepeating(heightMap1->GetTexture(0), true);
	SetTextureRepeating(heightMap2->GetTexture(0), true);
	SetTextureRepeating(heightMap->GetTexture(1), true);
	SetTextureRepeating(heightMap->GetBumpMap(), true);
	SetTextureRepeating(heightMap1->GetBumpMap(), true);
	SetTextureRepeating(heightMap2->GetBumpMap(), true);


	init = true;
	waterRotate = 0.0f;

	projMatrix = Matrix4::Perspective(1.0f, 15000.0f, (float)width / (float)height, 45.0f);

	//Create the spaceship and give it a position
	root = new SceneNode();
	CubeRobot* c = new CubeRobot();
	c->SetTransform(Matrix4::Translation(Vector3(RAW_WIDTH3 * HEIGHTMAP3_X / 2.0f + 10000.0f, 2000.0f, RAW_HEIGHT3 * HEIGHTMAP3_Z / 2.0f)));
	
	root->AddChild(c);


	glEnable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_TEXTURE_CUBE_MAP_SEAMLESS);
	
}
//Destructor: Clean everything
	Renderer ::~Renderer(void) {
		delete camera, camera2, camera3;
		delete heightMap, heightMap1, heightMap2;
		delete quad, quad2, quad3;
		delete reflectShader;
		delete skyboxShader;
		delete lightShader;
		delete lightShader2;
		delete particleShader;
		delete light, light2, light3;
		delete basicFont;
		delete root;
		CubeRobot::DeleteCube();
		delete emitter, emitter2;
		glDeleteTextures(1, &shadowTex);
		glDeleteFramebuffers(1, &shadowFBO);
		delete hellData;
		delete hellNode;
		delete sceneShader;
		delete shadowShader;
		
		currentShader = 0;
	}

	void Renderer::UpdateScene(float msec) {
		
		if (scene == 1)
		{
			camera->UpdateCamera(msec);
			viewMatrix = camera->BuildViewMatrix();
			waterRotate += msec / 2500.0f;
		}
		if (scene == 2)
		{
			camera2->UpdateCamera(msec);
			viewMatrix = camera2->BuildViewMatrix();
			waterRotate += msec / 8000.0f;
			hellNode->Update(msec);
		}
		if (scene == 3)
		{
			ms += 6;
			emitter->Update(msec);
			emitter2->Update(msec);
			camera3->UpdateCamera(msec);
			viewMatrix = camera3->BuildViewMatrix();
			waterRotate += msec / 3500.0f;
			root->Update(msec);		

		}
	}

	//We render our scenes
	void Renderer::RenderScene() {
		if (scene == 1)
		{
			glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);

			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_CUBE_MAP, cubeMap);

			DrawSkybox();

			DrawHeightmap();
			DrawWater();
		
			SetCurrentShader(fontShader);
			
			glUniform1i(glGetUniformLocation(currentShader->GetProgram(), "diffuseTex"), 0);

			//Render function to encapsulate our font rendering!
			DrawText("FPS: " + to_string(1000/ msec2), Vector3(0, 0, 0), 16.0f);
			DrawText("Sunset", Vector3(0, 800, 0), 16.0f);
			glUseProgram(0);

			SwapBuffers();
		}

		if (scene == 2)
		{
			glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
			
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_CUBE_MAP, cubeMap2);

			DrawSkybox();
			DrawShadowScene();
			DrawCombinedScene();
			SetCurrentShader(fontShader);
												
			glUniform1i(glGetUniformLocation(currentShader->GetProgram(), "diffuseTex"), 0);

			//Render function to encapsulate our font rendering!
			DrawText("FPS: " + to_string(1000 / msec2), Vector3(0, 0, 0), 16.0f);
			DrawText("HellKnight", Vector3(0, 800, 0), 16.0f);
			glUseProgram(0);	

			SwapBuffers();
		}

		if (scene == 3)
		{
			glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);


			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_CUBE_MAP, cubeMap3);

			DrawSkybox();

			SetCurrentShader(particleShader);
			glUniform1i(glGetUniformLocation(currentShader->GetProgram(), "diffuseTex"), 0);

			SetShaderParticleSize(emitter->GetParticleSize());
			emitter->SetParticleSize(150.0f);
			emitter->SetParticleVariance(1.0f);
			emitter->SetLaunchParticles(16.0f);
			emitter->SetParticleLifetime(2000.0f);
			emitter->SetParticleSpeed(0.1f);
			modelMatrix.ToIdentity();
			modelMatrix = Matrix4::Translation(Vector3(RAW_WIDTH3 * HEIGHTMAP3_X / 2.0f + 12000.0f - ms, 2400.0f, RAW_HEIGHT3 * HEIGHTMAP3_Z / 2.0f + 1300.0f)) * Matrix4::Scale(Vector3(10.0f,10.0f,10.0f));
			UpdateShaderMatrices();

			emitter->Draw();

			SetShaderParticleSize(emitter->GetParticleSize());
			emitter2->SetParticleSize(150.0f);
			emitter2->SetParticleVariance(1.0f);
			emitter2->SetLaunchParticles(16.0f);
			emitter2->SetParticleLifetime(2000.0f);
			emitter2->SetParticleSpeed(0.1f);
			modelMatrix.ToIdentity();
			modelMatrix = Matrix4::Translation(Vector3(RAW_WIDTH3 * HEIGHTMAP3_X / 2.0f + 12000.0f - ms, 2400.0f , RAW_HEIGHT3 * HEIGHTMAP3_Z / 2.0f - 1300.0f)) * Matrix4::Scale(Vector3(10.0f, 10.0f, 10.0f));
			UpdateShaderMatrices();

			emitter2->Draw();

			glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

			SetCurrentShader(cubeShader);
			glUniform1i(glGetUniformLocation(currentShader->GetProgram(), "diffuseTex"), 1);
			UpdateShaderMatrices();
			DrawNode(root);

			DrawHeightmap();


			DrawWater();

			SetCurrentShader(fontShader);
												
			glUniform1i(glGetUniformLocation(currentShader->GetProgram(), "diffuseTex"), 0);

			//Render function to encapsulate our font rendering!
			DrawText("FPS: " + to_string(1000 / msec2), Vector3(0, 0, 0), 16.0f);
			DrawText("Spaceship", Vector3(0, 800, 0), 16.0f);

			glUseProgram(0);

			SwapBuffers();
		}
	}

	//We draw the skybox and enable its shader
	void Renderer::DrawSkybox() {
		if (scene == 1)
		{
			glDepthMask(GL_FALSE);
			SetCurrentShader(skyboxShader);
			
			UpdateShaderMatrices();
			quad->Draw();

			glUseProgram(0);
			glDepthMask(GL_TRUE);
		}

		if (scene == 2)
		{
			glDepthMask(GL_FALSE);
			SetCurrentShader(skyboxShader);

			UpdateShaderMatrices();
			quad2->Draw();

			glUseProgram(0);
			glDepthMask(GL_TRUE);
		}

		if (scene == 3)
		{
			glDepthMask(GL_FALSE);
			SetCurrentShader(skyboxShader);

			UpdateShaderMatrices();
			quad3->Draw();

			glUseProgram(0);
			glDepthMask(GL_TRUE);
		}
	}
	//We draw the heightmap and enable its shader
	void Renderer::DrawHeightmap() {
		if (scene == 1)
		{
			SetCurrentShader(lightShader);
			SetShaderLight(*light);

			glUniform3fv(glGetUniformLocation(currentShader->GetProgram(), "cameraPos"), 1, (float *)& camera->GetPosition());

			glUniform1i(glGetUniformLocation(currentShader->GetProgram(), "diffuseTex"), 0);
			glUniform1i(glGetUniformLocation(currentShader -> GetProgram(),"diffuseTex1"), 2);
			glUniform1i(glGetUniformLocation(currentShader->GetProgram(), "bumpTex"), 1);

			modelMatrix.ToIdentity();
			textureMatrix.ToIdentity();

			UpdateShaderMatrices();

			heightMap->Draw();

			glUseProgram(0);
		}
		
		if (scene == 2)
		{
			SetCurrentShader(lightShader2);
			SetShaderLight(*light2);

			glUniform3fv(glGetUniformLocation(currentShader->GetProgram(), "cameraPos"), 1, (float *)& camera->GetPosition());

			glUniform1i(glGetUniformLocation(currentShader->GetProgram(), "diffuseTex"), 0);
			glUniform1i(glGetUniformLocation(currentShader->GetProgram(), "bumpTex"), 1);

			modelMatrix.ToIdentity();
			textureMatrix.ToIdentity();

			UpdateShaderMatrices();

			heightMap1->Draw();

			glUseProgram(0);
		}
		
		if (scene == 3)
		{
			SetCurrentShader(lightShader2);
			SetShaderLight(*light3);

			glUniform3fv(glGetUniformLocation(currentShader->GetProgram(), "cameraPos"), 1, (float *)& camera->GetPosition());

			glUniform1i(glGetUniformLocation(currentShader->GetProgram(), "diffuseTex"), 0);
			glUniform1i(glGetUniformLocation(currentShader->GetProgram(), "bumpTex"), 1);

			modelMatrix.ToIdentity();
			textureMatrix.ToIdentity();

			UpdateShaderMatrices();

			heightMap2->Draw();

			glUseProgram(0);
		}
	}
	//We draw the water, enable its shader, give it a position and we make the texture of the water rotate.
	void Renderer::DrawWater() {
		if (scene == 1)
		{
			SetCurrentShader(reflectShader);
			SetShaderLight(*light);
			glUniform3fv(glGetUniformLocation(currentShader->GetProgram(), "cameraPos"), 1, (float *)& camera->GetPosition());

			glUniform1i(glGetUniformLocation(currentShader->GetProgram(), "diffuseTex"), 0);

			glUniform1i(glGetUniformLocation(currentShader->GetProgram(), "cubeTex"), 2);

			glActiveTexture(GL_TEXTURE2);
			glBindTexture(GL_TEXTURE_CUBE_MAP, cubeMap);

			float heightX = (RAW_WIDTH * HEIGHTMAP_X / 2.0f);

			float heightY = 256 * HEIGHTMAP_Y / 4.0f;

			float heightZ = (RAW_HEIGHT * HEIGHTMAP_Z / 2.0f);
			Matrix4 MM = modelMatrix;
			Matrix4 TM = textureMatrix;
			modelMatrix =
				Matrix4::Translation(Vector3(heightX, heightY, heightZ)) *
				Matrix4::Scale(Vector3(heightX, 1, heightZ)) *
				Matrix4::Rotation(90, Vector3(1.0f, 0.0f, 0.0f));

			textureMatrix = Matrix4::Scale(Vector3(10.0f, 10.0f, 10.0f)) *
				Matrix4::Rotation(waterRotate, Vector3(0.0f, 0.0f, 1.0f));


			UpdateShaderMatrices();

			quad->Draw();
			modelMatrix = MM;
			textureMatrix = TM;
			glUseProgram(0);
		}
		
		if (scene == 2)
		{
			SetCurrentShader(reflectShader);
			SetShaderLight(*light2);
			glUniform3fv(glGetUniformLocation(currentShader->GetProgram(), "cameraPos"), 1, (float *)& camera->GetPosition());

			glUniform1i(glGetUniformLocation(currentShader->GetProgram(), "diffuseTex"), 0);

			glUniform1i(glGetUniformLocation(currentShader->GetProgram(), "cubeTex"), 2);

			glActiveTexture(GL_TEXTURE2);
			glBindTexture(GL_TEXTURE_CUBE_MAP, cubeMap2);

			float heightX = (RAW_WIDTH2 * HEIGHTMAP2_X / 2.0f);

			float heightY = 256 * HEIGHTMAP2_Y / 5.0f;

			float heightZ = (RAW_HEIGHT2 * HEIGHTMAP2_Z / 2.0f);
			Matrix4 MM = modelMatrix;
			Matrix4 TM = textureMatrix;
			modelMatrix =
				Matrix4::Translation(Vector3(heightX, heightY, heightZ)) *
				Matrix4::Scale(Vector3(heightX, 1, heightZ)) *
				Matrix4::Rotation(90, Vector3(1.0f, 0.0f, 0.0f));

			textureMatrix = Matrix4::Scale(Vector3(10.0f, 10.0f, 10.0f)) *
				Matrix4::Rotation(waterRotate, Vector3(0.0f, 0.0f, 1.0f));


			UpdateShaderMatrices();

			quad2->Draw();
			modelMatrix = MM;
			textureMatrix = TM;
			glUseProgram(0);
		}

		if (scene == 3)
		{
			SetCurrentShader(reflectShader);
			SetShaderLight(*light);
			glUniform3fv(glGetUniformLocation(currentShader->GetProgram(), "cameraPos"), 1, (float *)& camera->GetPosition());

			glUniform1i(glGetUniformLocation(currentShader->GetProgram(), "diffuseTex"), 0);

			glUniform1i(glGetUniformLocation(currentShader->GetProgram(), "cubeTex"), 2);

			glActiveTexture(GL_TEXTURE2);
			glBindTexture(GL_TEXTURE_CUBE_MAP, cubeMap3);

			float heightX = (RAW_WIDTH3 * HEIGHTMAP3_X / 2.0f);

			float heightY = 256 * HEIGHTMAP3_Y / 2.5f;

			float heightZ = (RAW_HEIGHT3 * HEIGHTMAP3_Z / 2.0f);
			Matrix4 MM = modelMatrix;
			Matrix4 TM = textureMatrix;
			modelMatrix =
				Matrix4::Translation(Vector3(heightX, heightY, heightZ)) *
				Matrix4::Scale(Vector3(heightX, 1, heightZ)) *
				Matrix4::Rotation(90, Vector3(1.0f, 0.0f, 0.0f));

			textureMatrix = Matrix4::Scale(Vector3(10.0f, 10.0f, 10.0f)) *
				Matrix4::Rotation(waterRotate, Vector3(0.0f, 0.0f, 1.0f));


			UpdateShaderMatrices();

			quad3->Draw();
			modelMatrix = MM;
			textureMatrix = TM;
			glUseProgram(0);
		}
	}

	void Renderer::DrawText(const std::string &text, const Vector3 &position, const float size, const bool perspective) {
		//Create a new temporary TextMesh, using our line of text and our font
		TextMesh* mesh = new TextMesh(text, *basicFont);
		Matrix4 MM = modelMatrix;
		Matrix4 VM = viewMatrix;
		Matrix4 PM = projMatrix;
		//This just does simple matrix setup to render in either perspective or
		//orthographic mode, there's nothing here that's particularly tricky.
		if (perspective) {
			modelMatrix = Matrix4::Translation(position) * Matrix4::Scale(Vector3(size, size, 1));
			viewMatrix = camera->BuildViewMatrix();
			projMatrix = Matrix4::Perspective(1.0f, 10000.0f, (float)width / (float)height, 45.0f);
		}
		else {
			//In ortho mode, we subtract the y from the height, so that a height of 0
			//is at the top left of the screen, which is more intuitive
			modelMatrix = Matrix4::Translation(Vector3(position.x, height - position.y, position.z)) * Matrix4::Scale(Vector3(size, size, 1));
			viewMatrix.ToIdentity();
			projMatrix = Matrix4::Orthographic(-1.0f, 1.0f, (float)width, 0.0f, (float)height, 0.0f);
		}
		//Either way, we update the matrices, and draw the mesh
		UpdateShaderMatrices();
		mesh->Draw();

		modelMatrix = MM;
		viewMatrix = VM;
		projMatrix = PM;

		delete mesh;
	}

	void Renderer::SetMsec(float msec)
	{
		msec2 = msec;
	}

	//We iterate over a node's children drawing each of those too
	void Renderer::DrawNode(SceneNode * n) {
		if (n->GetMesh()) {
			Matrix4 transform = n->GetWorldTransform() *
				Matrix4::Scale(n->GetModelScale());
			glUniformMatrix4fv(glGetUniformLocation(currentShader->GetProgram(), "modelMatrix"), 1, false, (float *)& transform);

			glUniform4fv(glGetUniformLocation(currentShader->GetProgram(), "nodeColour"), 1, (float *)& n->GetColour());
			glUniform1i(glGetUniformLocation(currentShader->GetProgram(), "useTexture"), (int)n->GetMesh()->GetTexture(0));
			n->Draw(*this);

		}

		for (vector < SceneNode * >::const_iterator
			i = n->GetChildIteratorStart();
			i != n->GetChildIteratorEnd(); ++i) {
			DrawNode(*i);

		}
	}

	void	Renderer::SetShaderParticleSize(float f) {
		glUniform1f(glGetUniformLocation(currentShader->GetProgram(), "particleSize"), f);
	}

	//We make the camera move on the x axis by holding the x button
	void Renderer::CameraForward()
	{
		camera3->SetPosition(Vector3(camera3->GetPosition().x - 6, camera3->GetPosition().y, camera3->GetPosition().z));
	}

	//Simple function to constantly change scenes by pressing the PAUSE button
	void Renderer::CycleScenes(float msec)
	{
		
		counter+=msec;
		if (counter > 100) {
			if (scene == 1)
			scene = 2;
			if (scene == 2)
			scene = 3;
			if (scene == 3)
			scene = 1;

			counter = 0;
			}
				
			
		}

	//We draw the shadows of our hellknight on the heightmap
	void Renderer::DrawShadowScene() {
		glBindFramebuffer(GL_FRAMEBUFFER, shadowFBO);

		glClear(GL_DEPTH_BUFFER_BIT);

		glViewport(0, 0, SHADOWSIZE, SHADOWSIZE);

		glColorMask(GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE);

		SetCurrentShader(shadowShader);

		//viewMatrix = Matrix4::BuildViewMatrix(light2 -> GetPosition(), Vector3(0, 0, 0)); 
		//shadowMatrix = biasMatrix *(projMatrix * viewMatrix);

		UpdateShaderMatrices();
		DrawHeightmap();
		DrawWater();
		DrawMesh();

		glUseProgram(0);
		glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);
		glViewport(0, 0, width, height);

		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}

	void Renderer::DrawCombinedScene() {
		SetCurrentShader(sceneShader);
		glUniform1i(glGetUniformLocation(currentShader->GetProgram(), "diffuseTex"), 0);
		glUniform1i(glGetUniformLocation(currentShader->GetProgram(), "bumpTex"), 1);
		glUniform1i(glGetUniformLocation(currentShader->GetProgram(), "shadowTex"), 5);

		glUniform3fv(glGetUniformLocation(currentShader->GetProgram(), "cameraPos"), 1, (float *)& camera2->GetPosition());

		SetShaderLight(*light2);

		glActiveTexture(GL_TEXTURE5);
		glBindTexture(GL_TEXTURE_2D, shadowTex);

		viewMatrix = camera2 -> BuildViewMatrix();

		UpdateShaderMatrices();

		DrawMesh();
		DrawHeightmap();
		DrawWater();
		glUseProgram(0);

	}

//We draw our hellknight and we give it a position
	void Renderer::DrawMesh() {
		modelMatrix.ToIdentity();

		Matrix4 tempMatrix = shadowMatrix * modelMatrix;
		modelMatrix = Matrix4::Translation(Vector3(RAW_WIDTH2 * HEIGHTMAP2_X / 2.0f, 420.0f, RAW_HEIGHT2 * HEIGHTMAP2_Z / 2.0f)) * Matrix4::Scale(Vector3(10.0f, 10.0f, 10.0f));
		glUniformMatrix4fv(glGetUniformLocation(currentShader->GetProgram(), "shadowMatrix"), 1, false, *& tempMatrix.values);

		glUniformMatrix4fv(glGetUniformLocation(currentShader->GetProgram(), "modelMatrix"), 1, false, *& modelMatrix.values);

		hellNode->Draw(*this);

	}