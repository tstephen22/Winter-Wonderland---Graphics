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
		this->discoShader = Shader("pointLightVert.vs", "pointLightFrag.fs");
		this->size = size;
	}
	Statue(){

	}
	void draw(Shader materialShader, glm::mat4& view, glm::mat4& projection) {
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
		if (!disco) {
			//Point light 
			materialShader.setFloat("pointLights[0].constant", 0.0f);
			materialShader.setMat4("model", mHat);
			hat.Draw(materialShader);
		}
		else {
			//Point light 
			materialShader.setFloat("pointLights[0].constant", 1.0f);
			materialShader.setFloat("pointLights[0].linear", 0.09f);
			materialShader.setFloat("pointLights[0].quadratic", 0.032f);
			materialShader.setVec3("pointLights[0].position", glm::vec3(0.0f, 3.0f, 0.0f));
			glm::vec3 lightColor;
			lightColor.x = sin(glfwGetTime() * 2.0f);
			lightColor.y = sin(glfwGetTime() * 0.7f);
			lightColor.z = sin(glfwGetTime() * 1.3f);

			glm::vec3 diffuseColor = lightColor * glm::vec3(0.5f);
			glm::vec3 ambientColor = diffuseColor * glm::vec3(0.2f);

			materialShader.setVec3("pointLights[0].ambient", ambientColor);
			materialShader.setVec3("pointLights[0].diffuse", diffuseColor);
			materialShader.setVec3("pointLights[0].specular", 1.0f, 1.0f, 1.0f);

			discoShader.use();
			discoShader.setVec3("inputColor", lightColor);
			discoShader.setMat4("view", view);
			discoShader.setMat4("projection", projection);
			discoShader.setMat4("model", mHat);
			hat.Draw(discoShader);
		}
	}
	void toggleDisco() {
		this->disco = !this->disco;
	}

private:
	Shader discoShader;
	bool disco = false;
	float posX, posY, posZ;
	float size;
	GLfloat angle;
	glm::mat4 mBody;
	glm::mat4 mHat;
};