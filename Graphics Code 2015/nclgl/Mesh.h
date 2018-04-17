#pragma once

#include "OGLRenderer.h"
 enum MeshBuffer {VERTEX_BUFFER, COLOUR_BUFFER,TEXTURE_BUFFER ,NORMAL_BUFFER, TANGENT_BUFFER, INDEX_BUFFER, MAX_BUFFER};
class Mesh {
public:
	Mesh(void);
	~Mesh(void);
		
	virtual void Draw();
	static Mesh * GenerateTriangle();
	void SetTexture(GLuint tex, int pos) { texture[pos] = tex; }
	GLuint GetTexture(int pos) { return texture[pos]; }
	static Mesh* GenerateQuad();

	void SetBumpMap(GLuint tex) { bumpTexture = tex; }
	GLuint GetBumpMap() { return bumpTexture; }

protected:
		void BufferData();
		void GenerateNormals();
			
		GLuint arrayObject;
		GLuint bufferObject[MAX_BUFFER];
		GLuint numVertices;
		GLuint type;
		GLuint texture[2];
		GLuint numIndices;

		unsigned int *indices;
			
		Vector2* textureCoords;
		Vector3* vertices;
		Vector3* normals;
		Vector4* colours;

		void GenerateTangents();
		Vector3 GenerateTangent(const Vector3 &a, const Vector3 &b,const Vector3 &c, const Vector2 & ta,const Vector2 & tb, const Vector2 & tc);

		Vector3 * tangents;
		GLuint bumpTexture;

};

