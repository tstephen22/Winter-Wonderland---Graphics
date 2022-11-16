#include "MeshLoader.h"
#pragma once

#define SNOWMAN "snowman.dae"
#define HAT "hat.obj"
#define LEFT_ARM "ArmLeft.dae"
#define RIGHT_ARM "ArmRight.dae"

class Snowman
{
public:
	ModelData body_model; 
	ModelData hat_model; 
	ModelData left_arm_model; 
	ModelData right_arm_model; 
	Snowman() {

	}
	Snowman(float x, float y, float z, GLuint shaderProgramID) {

	}
private: 
	MeshLoader meshLoader = MeshLoader();
	float posX, posY, posZ;

	unsigned int body_vn_vbo = 0;
	unsigned int body_vp_vbo = 0;
	unsigned int body_vao = 0;

	unsigned int hat_vn_vbo = 0;
	unsigned int hat_vp_vbo = 0;
	unsigned int hat_vao = 0;

	unsigned int leftArm_vn_vbo = 0;
	unsigned int leftArm_vp_vbo = 0;
	unsigned int leftArm_vao = 0;

	unsigned int rightArm_vn_vbo = 0;
	unsigned int rightArm_vp_vbo = 0;
	unsigned int rightArm_vao = 0;

	GLuint loc1, loc2, loc3;

	mat4 body;
	mat4 hat; 
	mat4 left_arm; 
	mat4 right_arm;

	int left_arm_distance = 0;
	int right_arm_distance = 0;

	GLuint shaderProgramID;
	vec3 left_arm_pos = vec3(0.0f, 0.0f, 0.0f);
	vec3 right_arm_pos = vec3(0.0f, 0.0f, 0.0f);
	bool left_arm_up = false;
	bool right_arm_up = false;
};

