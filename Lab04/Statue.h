#pragma once
#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "shader_m.h"
#include "camera.h"
#include "model.h"
#include "file_system.h"
#include <iostream>

#define SNOWMAN "textures/statue/statue.obj"
#define HAT "textures/statue/statue_hat.obj"

class Statue {
public:

	Model body;
	Model hat; 

	Statue(float x, float y, float z, GLfloat angle, float size) {
		this->posX = x; this->posY = y; this->posZ = z;
		this->angle = angle;
		this->body = Model(FileSystem::getPath(SNOWMAN));
		this->hat = Model(FileSystem::getPath(HAT));
		this->size = size;
	}
	void draw(Shader materialShader) {
		materialShader.setFloat("material.shininess", 4.0f);
		mBody = glm::mat4(1.0f);
		mBody = glm::translate(mBody, glm::vec3(posX, posY, posZ));
		mBody = glm::rotate(mBody, glm::radians(angle), glm::vec3(0, 1, 0));
		mBody = glm::scale(mBody, glm::vec3(size, size, size));
		materialShader.setMat4("model", mBody);
		body.Draw(materialShader);
		mHat = glm::mat4(1.0f);
		mHat = glm::translate(mHat, glm::vec3(0.0f, 0.0f, 0.0f));
		mHat = mBody * mHat;
		materialShader.setMat4("model", mHat);
		hat.Draw(materialShader);
	}

private:
	float posX, posY, posZ;
	float size;
	GLfloat angle;
	glm::mat4 mBody;
	glm::mat4 mHat;
};