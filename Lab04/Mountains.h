#pragma once
#include "MeshLoader.h"
#define MOUNTAIN "mountains.dae"

class Mountains
{
public: 
	ModelData model; 
	Mountains() {

	}
	Mountains(float x, float y, float z, GLuint shaderProgramID) {
		this->shaderProgramID = shaderProgramID; 
		this->posX = x; this->posY = y; this->posZ = z; 
		loc1 = glGetAttribLocation(shaderProgramID, "vertex_position");
		loc2 = glGetAttribLocation(shaderProgramID, "vertex_normal");
		loc3 = glGetAttribLocation(shaderProgramID, "vertex_texture");
	}

	void init() {
		model = meshLoader.load_mesh(MOUNTAIN);

		glGenBuffers(1, &mountains_vp_vbo);
		glBindBuffer(GL_ARRAY_BUFFER, mountains_vp_vbo);
		glBufferData(GL_ARRAY_BUFFER, model.mPointCount * sizeof(vec3), &model.mVertices[0], GL_STATIC_DRAW);
		glGenBuffers(1, &mountains_vn_vbo);
		glBindBuffer(GL_ARRAY_BUFFER, mountains_vn_vbo);
		glBufferData(GL_ARRAY_BUFFER, model.mPointCount * sizeof(vec3), &model.mNormals[0], GL_STATIC_DRAW);
	}

	void draw() {
		int matrix_location = glGetUniformLocation(shaderProgramID, "model");
		int view_mat_location = glGetUniformLocation(shaderProgramID, "view");
		int proj_mat_location = glGetUniformLocation(shaderProgramID, "proj");
		mountains = identity_mat4();
		mountains = translate(mountains, vec3(posX, posY, posZ));
		glEnableVertexAttribArray(loc1);
		glBindBuffer(GL_ARRAY_BUFFER, mountains_vp_vbo);
		glVertexAttribPointer(loc1, 3, GL_FLOAT, GL_FALSE, 0, NULL);
		glEnableVertexAttribArray(loc2);
		glBindBuffer(GL_ARRAY_BUFFER, mountains_vn_vbo);
		glVertexAttribPointer(loc2, 3, GL_FLOAT, GL_FALSE, 0, NULL);

		glUniformMatrix4fv(matrix_location, 1, GL_FALSE, mountains.m);
		glDrawArrays(GL_TRIANGLES, 3, model.mPointCount);
	}

private:
	MeshLoader meshLoader = MeshLoader(); 
	float posX, posY, posZ;
	unsigned int mountains_vn_vbo = 0;
	unsigned int mountains_vp_vbo = 0;
	unsigned int mountains_vao = 0;
	GLuint loc1, loc2, loc3;
	mat4 mountains; 
	GLuint shaderProgramID;
};

