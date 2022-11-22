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
	ModelData leftArm_model; 
	ModelData rightArm_model; 
	Snowman() {

	}
	Snowman(float x, float y, float z, GLfloat angle, GLuint shaderProgramID) {
		this->snowman_pos = vec3(x, y, x); this->angle = angle;
		loc1 = glGetAttribLocation(shaderProgramID, "vertex_position");
		loc2 = glGetAttribLocation(shaderProgramID, "vertex_normal");
		loc3 = glGetAttribLocation(shaderProgramID, "vertex_texture");
	}
	void init() {
		//Loading body 
		body_model = meshLoader.load_mesh(SNOWMAN);
		glGenBuffers(1, &body_vp_vbo);
		glBindBuffer(GL_ARRAY_BUFFER, body_vp_vbo);
		glBufferData(GL_ARRAY_BUFFER, body_model.mPointCount * sizeof(vec3), &body_model.mVertices[0], GL_STATIC_DRAW);
		glGenBuffers(1, &body_vn_vbo);
		glBindBuffer(GL_ARRAY_BUFFER, body_vn_vbo);
		glBufferData(GL_ARRAY_BUFFER, body_model.mPointCount * sizeof(vec3), &body_model.mNormals[0], GL_STATIC_DRAW);
		//Loading hat
		hat_model = meshLoader.load_mesh(HAT);
		glGenBuffers(1, &hat_vp_vbo);
		glBindBuffer(GL_ARRAY_BUFFER, hat_vp_vbo);
		glBufferData(GL_ARRAY_BUFFER, hat_model.mPointCount * sizeof(vec3), &hat_model.mVertices[0], GL_STATIC_DRAW);
		glGenBuffers(1, &hat_vn_vbo);
		glBindBuffer(GL_ARRAY_BUFFER, hat_vn_vbo);
		glBufferData(GL_ARRAY_BUFFER, hat_model.mPointCount * sizeof(vec3), &hat_model.mNormals[0], GL_STATIC_DRAW);
		//Loading right arm 
		rightArm_model = meshLoader.load_mesh(RIGHT_ARM);
		glGenBuffers(1, &rightArm_vp_vbo);
		glBindBuffer(GL_ARRAY_BUFFER, rightArm_vp_vbo);
		glBufferData(GL_ARRAY_BUFFER, rightArm_model.mPointCount * sizeof(vec3), &rightArm_model.mVertices[0], GL_STATIC_DRAW);
		glGenBuffers(1, &rightArm_vn_vbo);
		glBindBuffer(GL_ARRAY_BUFFER, rightArm_vn_vbo);
		glBufferData(GL_ARRAY_BUFFER, rightArm_model.mPointCount * sizeof(vec3), &rightArm_model.mNormals[0], GL_STATIC_DRAW);
		//Loading left arm 
		leftArm_model = meshLoader.load_mesh(LEFT_ARM);
		glGenBuffers(1, &leftArm_vp_vbo);
		glBindBuffer(GL_ARRAY_BUFFER, leftArm_vp_vbo);
		glBufferData(GL_ARRAY_BUFFER, leftArm_model.mPointCount * sizeof(vec3), &leftArm_model.mVertices[0], GL_STATIC_DRAW);
		glGenBuffers(1, &leftArm_vn_vbo);
		glBindBuffer(GL_ARRAY_BUFFER, leftArm_vn_vbo);
		glBufferData(GL_ARRAY_BUFFER, leftArm_model.mPointCount * sizeof(vec3), &leftArm_model.mNormals[0], GL_STATIC_DRAW);
	}

	void draw() {
		int matrix_location = glGetUniformLocation(shaderProgramID, "model");
		int view_mat_location = glGetUniformLocation(shaderProgramID, "view");
		int proj_mat_location = glGetUniformLocation(shaderProgramID, "proj");

	    snowman = identity_mat4();
		snowman = translate(snowman, snowman_pos);
		snowman = rotate_y_deg(snowman, angle);
		glEnableVertexAttribArray(loc1);
		glBindBuffer(GL_ARRAY_BUFFER, body_vp_vbo);
		glVertexAttribPointer(loc1, 3, GL_FLOAT, GL_FALSE, 0, NULL);
		glEnableVertexAttribArray(loc2);
		glBindBuffer(GL_ARRAY_BUFFER, body_vn_vbo);
		glVertexAttribPointer(loc2, 3, GL_FLOAT, GL_FALSE, 0, NULL);

		glUniformMatrix4fv(matrix_location, 1, GL_FALSE, snowman.m);
		glDrawArrays(GL_TRIANGLES, 3, body_model.mPointCount);

		hat = identity_mat4();
		hat = translate(hat, vec3(0.0f, 0.0f, 0.0f));
		hat = rotate_y_deg(hat, rotate_y);
		hat = snowman * hat;
		glEnableVertexAttribArray(loc1);
		glBindBuffer(GL_ARRAY_BUFFER, hat_vp_vbo);
		glVertexAttribPointer(loc1, 3, GL_FLOAT, GL_FALSE, 0, NULL);
		glEnableVertexAttribArray(loc2);
		glBindBuffer(GL_ARRAY_BUFFER, hat_vn_vbo);
		glVertexAttribPointer(loc2, 3, GL_FLOAT, GL_FALSE, 0, NULL);

		glUniformMatrix4fv(matrix_location, 1, GL_FALSE, hat.m);
		glDrawArrays(GL_TRIANGLES, 3, hat_model.mPointCount);

		leftArm = identity_mat4();
		leftArm = translate(leftArm, left_arm_pos);
		leftArm = snowman * leftArm;
		glEnableVertexAttribArray(loc1);
		glBindBuffer(GL_ARRAY_BUFFER, leftArm_vp_vbo);
		glVertexAttribPointer(loc1, 3, GL_FLOAT, GL_FALSE, 0, NULL);
		glEnableVertexAttribArray(loc2);
		glBindBuffer(GL_ARRAY_BUFFER, leftArm_vn_vbo);
		glVertexAttribPointer(loc2, 3, GL_FLOAT, GL_FALSE, 0, NULL);

		glUniformMatrix4fv(matrix_location, 1, GL_FALSE, leftArm.m);
		glDrawArrays(GL_TRIANGLES, 3, leftArm_model.mPointCount);

		rightArm = identity_mat4();
		rightArm = translate(rightArm, right_arm_pos);

		rightArm = snowman * rightArm;
		glEnableVertexAttribArray(loc1);
		glBindBuffer(GL_ARRAY_BUFFER, rightArm_vp_vbo);
		glVertexAttribPointer(loc1, 3, GL_FLOAT, GL_FALSE, 0, NULL);
		glEnableVertexAttribArray(loc2);
		glBindBuffer(GL_ARRAY_BUFFER, rightArm_vn_vbo);
		glVertexAttribPointer(loc2, 3, GL_FLOAT, GL_FALSE, 0, NULL);

		glUniformMatrix4fv(matrix_location, 1, GL_FALSE, rightArm.m);
		glDrawArrays(GL_TRIANGLES, 3, rightArm_model.mPointCount);
	}

	void update() {
		static DWORD last_time = 0;
		DWORD curr_time = timeGetTime();
		if (last_time == 0)
			last_time = curr_time;
		float delta = (curr_time - last_time) * 0.001f;
		last_time = curr_time;
		// Rotate the model slowly around the y axis at 20 degrees per second
		rotate_y += 80.0f * delta;
		rotate_y = fmodf(rotate_y, 360.0f);

		if (left_arm_distance < 700) {
			left_arm_distance++;
		}
		else {
			left_arm_distance = 0;
			left_arm_up = !left_arm_up;
		}

		if (left_arm_up) left_arm_pos.v[1] += 0.0001f;
		else left_arm_pos.v[1] -= 0.0001f;

		if (right_arm_distance < 400) {
			right_arm_distance++;
		}
		else {
			right_arm_distance = 0;
			right_arm_up = !right_arm_up;
		}

		if (right_arm_up) right_arm_pos.v[1] += 0.0001f;
		else right_arm_pos.v[1] -= 0.0001f;

		snowman_pos.v[2] += 0.0001f;
	}

private: 
	MeshLoader meshLoader = MeshLoader();
	float posX, posY, posZ; 
	GLfloat angle; 

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

	mat4 snowman;
	mat4 hat; 
	mat4 leftArm; 
	mat4 rightArm;

	int left_arm_distance = 0;
	int right_arm_distance = 0;
	GLfloat rotate_y = 0.0f;
	GLfloat rotate_arm = 0.0f;

	GLuint shaderProgramID;
	vec3 snowman_pos;
	vec3 left_arm_pos = vec3(0.0f, 0.0f, 0.0f);
	vec3 right_arm_pos = vec3(0.0f, 0.0f, 0.0f);
	bool left_arm_up = false;
	bool right_arm_up = false;
};

