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

#define SNOWMAN "snowman_body.obj"
#define HAT "hat.obj"
#define LEFT_ARM "ArmLeft.obj"
#define RIGHT_ARM "ArmRight.obj"
class Snowman {
public: 

	Model body; 
	Model rightArm; 
	Model leftArm; 
	Model hat; 

	Snowman(float x, float y, float z, GLfloat angle, float size) {
		this->posX = x; this->posY = y; this->posZ = z; 
		this->angle = angle; 
		this->body = Model(FileSystem::getPath(SNOWMAN));
		this->hat = Model(FileSystem::getPath(HAT));
		this->rightArm = Model(FileSystem::getPath(RIGHT_ARM));
		this->leftArm = Model(FileSystem::getPath(LEFT_ARM));
		this->size = size; 
	}
	Snowman() {

	}
	void draw(Shader shaderProgram) {
		mBody = glm::mat4(1.0f);
		mBody = glm::translate(mBody, glm::vec3(posX, posY, posZ)); 
		mBody = glm::rotate(mBody, glm::radians(angle), glm::vec3(0, 1, 0));
		mBody = glm::rotate(mBody, glm::radians(bodyRotate), glm::vec3(1, 0, 0));
		mBody = glm::scale(mBody, glm::vec3(size, size, size));
		shaderProgram.setMat4("model", mBody);
		body.Draw(shaderProgram); 
		mHat = glm::mat4(1.0f);
		mHat = glm::translate(mHat, glm::vec3(0.0f, 0.0f, 0.0f)); 
		mHat = mBody * mHat;
		shaderProgram.setMat4("model", mHat);
		hat.Draw(shaderProgram); 
		mRightArm = glm::mat4(1.0f); 
		mRightArm = mRightArm * glm::rotate(glm::mat4(1.0f), glm::radians(armRotate), glm::vec3(1, 0, 0));
		mRightArm = mBody * mRightArm; 
		shaderProgram.setMat4("model", mRightArm);
		rightArm.Draw(shaderProgram); 
		mLeftArm = glm::mat4(1.0f);
		mLeftArm = glm::rotate(mLeftArm, glm::radians(armRotate), glm::vec3(1, 0, 0));
		mLeftArm = mBody * mLeftArm;
		shaderProgram.setMat4("model", mLeftArm);
		leftArm.Draw(shaderProgram);
	}
	void runInCircle() {
		
	}
	void bow() {
		if (armDown) {
			armRotate += 0.1f;
			bodyRotate += 0.1f; 
			armRotate = fmodf(armRotate, 360.0f);
			bodyRotate = fmodf(bodyRotate, 360.0f);
		}
		else {
			armRotate -= 0.1f;
			bodyRotate -= 0.1f; 
			armRotate = fmodf(armRotate, 360.0f);
			bodyRotate = fmodf(bodyRotate, 360.0f); 
		}
		if (armRotate > 20.0f) armDown = false;
		else if (armRotate < -5.0f) armDown = true;
	}

private: 

	float posX, posY, posZ; 
	float size;
	GLfloat angle; 
	GLfloat armRotate = 0.0f; 
	GLfloat bodyRotate = 0.0f;
	bool armDown = true;
	glm::mat4 mBody; 
	glm::mat4 mHat; 
	glm::mat4 mLeftArm; 
	glm::mat4 mRightArm; 
};