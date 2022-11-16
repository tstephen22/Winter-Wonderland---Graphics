// Windows includes (For Time, IO, etc.)
#pragma region Imports
#include <windows.h>
#include <mmsystem.h>
#include <iostream>
#include <string>
#include <stdio.h>
#include <math.h>
#include <vector> // STL dynamic memory.

// OpenGL includes
#include <GL/glew.h>
#include <GL/freeglut.h>

// Assimp includes
#include <assimp/cimport.h> // scene importer
#include <assimp/scene.h> // collects data
#include <assimp/postprocess.h> // various extra operations


//glm includes 
#include <glm/gtc/matrix_transform.hpp>
#include <glm/glm.hpp>
// Project includes
#include "maths_funcs.h"
#include "camera.h"

// 
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "Mountains.h"
#pragma endregion Imports 


/*----------------------------------------------------------------------------
MESH TO LOAD
----------------------------------------------------------------------------*/
// this mesh is a dae file format but you should be able to use any other format too, obj is typically what is used
// put the mesh in your project directory, or provide a filepath for it here
#define SNOWMAN "snowman.dae"
#define HAT "hat.obj"
#define LEFT_ARM "ArmLeft.dae"
#define RIGHT_ARM "ArmRight.dae"
#define MOUNTAIN "mountains.dae"
/*----------------------------------------------------------------------------
----------------------------------------------------------------------------*/

using namespace std;
GLuint shaderProgramID;
MeshLoader meshLoader = MeshLoader();

ModelData snowman_data; 
unsigned int snowman_vn_vbo = 0;
unsigned int snowman_vp_vbo = 0;
unsigned int snowman_vao = 0;

vec3 snowman_pos = vec3(0.0f, 0.5f, 0.0f); 
int left_arm_distance = 0; 
int right_arm_distance = 0;


ModelData hat_data;
unsigned int hat_vn_vbo = 0;
unsigned int hat_vp_vbo = 0;
unsigned int hat_vao = 0;

ModelData leftArm_data;
unsigned int leftArm_vn_vbo = 0;
unsigned int leftArm_vp_vbo = 0;
unsigned int leftArm_vao = 0;

ModelData rightArm_data;
unsigned int rightArm_vn_vbo = 0;
unsigned int rightArm_vp_vbo = 0;
unsigned int rightArm_vao = 0;

Mountains mountains; 

int width = 1000;
int height = 800;

GLuint loc1, loc2, loc3;
GLfloat rotate_y = 0.0f;
GLfloat rotate_arm = 0.0f;

Camera camera(vec3(0.0f, 0.0f, 3.0f));
float lastX = width / 2.0f;
float lastY = height / 2.0f;
unsigned int snowman_texture; 
unsigned int arm_texture; 
unsigned int hat_texture; 
unsigned int ground_texture; 

bool firstMouse = true; 
// Shader Functions- click on + to expand
#pragma region SHADER_FUNCTIONS
char* readShaderSource(const char* shaderFile) {
	FILE* fp;
	fopen_s(&fp, shaderFile, "rb");

	if (fp == NULL) { return NULL; }

	fseek(fp, 0L, SEEK_END);
	long size = ftell(fp);

	fseek(fp, 0L, SEEK_SET);
	char* buf = new char[size + 1];
	fread(buf, 1, size, fp);
	buf[size] = '\0';

	fclose(fp);

	return buf;
}


static void AddShader(GLuint ShaderProgram, const char* pShaderText, GLenum ShaderType)
{
	// create a shader object
	GLuint ShaderObj = glCreateShader(ShaderType);

	if (ShaderObj == 0) {
		std::cerr << "Error creating shader..." << std::endl;
		std::cerr << "Press enter/return to exit..." << std::endl;
		std::cin.get();
		exit(1);
	}
	const char* pShaderSource = readShaderSource(pShaderText);

	// Bind the source code to the shader, this happens before compilation
	glShaderSource(ShaderObj, 1, (const GLchar**)&pShaderSource, NULL);
	// compile the shader and check for errors
	glCompileShader(ShaderObj);
	GLint success;
	// check for shader related errors using glGetShaderiv
	glGetShaderiv(ShaderObj, GL_COMPILE_STATUS, &success);
	if (!success) {
		GLchar InfoLog[1024] = { '\0' };
		glGetShaderInfoLog(ShaderObj, 1024, NULL, InfoLog);
		std::cerr << "Error compiling "
			<< (ShaderType == GL_VERTEX_SHADER ? "vertex" : "fragment")
			<< " shader program: " << InfoLog << std::endl;
		std::cerr << "Press enter/return to exit..." << std::endl;
		std::cin.get();
		exit(1);
	}
	// Attach the compiled shader object to the program object
	glAttachShader(ShaderProgram, ShaderObj);
}

GLuint CompileShaders()
{
	//Start the process of setting up our shaders by creating a program ID
	//Note: we will link all the shaders together into this ID
	shaderProgramID = glCreateProgram();
	if (shaderProgramID == 0) {
		std::cerr << "Error creating shader program..." << std::endl;
		std::cerr << "Press enter/return to exit..." << std::endl;
		std::cin.get();
		exit(1);
	}

	// Create two shader objects, one for the vertex, and one for the fragment shader
	AddShader(shaderProgramID, "simpleVertexShader.txt", GL_VERTEX_SHADER);
	AddShader(shaderProgramID, "simpleFragmentShader.txt", GL_FRAGMENT_SHADER);

	GLint Success = 0;
	GLchar ErrorLog[1024] = { '\0' };
	// After compiling all shader objects and attaching them to the program, we can finally link it
	glLinkProgram(shaderProgramID);
	// check for program related errors using glGetProgramiv
	glGetProgramiv(shaderProgramID, GL_LINK_STATUS, &Success);
	if (Success == 0) {
		glGetProgramInfoLog(shaderProgramID, sizeof(ErrorLog), NULL, ErrorLog);
		std::cerr << "Error linking shader program: " << ErrorLog << std::endl;
		std::cerr << "Press enter/return to exit..." << std::endl;
		std::cin.get();
		exit(1);
	}

	// program has been successfully linked but needs to be validated to check whether the program can execute given the current pipeline state
	glValidateProgram(shaderProgramID);
	// check for program related errors using glGetProgramiv
	glGetProgramiv(shaderProgramID, GL_VALIDATE_STATUS, &Success);
	if (!Success) {
		glGetProgramInfoLog(shaderProgramID, sizeof(ErrorLog), NULL, ErrorLog);
		std::cerr << "Invalid shader program: " << ErrorLog << std::endl;
		std::cerr << "Press enter/return to exit..." << std::endl;
		std::cin.get();
		exit(1);
	}
	// Finally, use the linked shader program
	// Note: this program will stay in effect for all draw calls until you replace it with another or explicitly disable its use
	glUseProgram(shaderProgramID);
	return shaderProgramID;
}
#pragma endregion SHADER_FUNCTIONS

// VBO Functions - click on + to expand
#pragma region VBO_FUNCTIONS
void generateObjectBufferMesh() {
	/*----------------------------------------------------------------------------
	LOAD MESH HERE AND COPY INTO BUFFERS
	----------------------------------------------------------------------------*/

	//Note: you may get an error "vector subscript out of range" if you are using this code for a mesh that doesnt have positions and normals
	//Might be an idea to do a check for that before generating and binding the buffer.
	//LOADING MOUNTAINS 
	mountains = Mountains(0.0f, 0.0f, 0.0f, shaderProgramID); 
	mountains.init(); 
	//LOADING SNOWMAN
	snowman_data = meshLoader.load_mesh(SNOWMAN);

	glGenBuffers(1, &snowman_vp_vbo);
	glBindBuffer(GL_ARRAY_BUFFER, snowman_vp_vbo);
	glBufferData(GL_ARRAY_BUFFER, snowman_data.mPointCount * sizeof(vec3), &snowman_data.mVertices[0], GL_STATIC_DRAW);
	glGenBuffers(1, &snowman_vn_vbo);
	glBindBuffer(GL_ARRAY_BUFFER, snowman_vn_vbo);
	glBufferData(GL_ARRAY_BUFFER, snowman_data.mPointCount * sizeof(vec3), &snowman_data.mNormals[0], GL_STATIC_DRAW);

	//	This is for texture coordinates which you don't currently need, so I have commented it out
	//	unsigned int vt_vbo = 0;
	//	glGenBuffers (1, &vt_vbo);
	//	glBindBuffer (GL_ARRAY_BUFFER, vt_vbo);
	//	glBufferData (GL_ARRAY_BUFFER, monkey_head_data.mTextureCoords * sizeof (vec2), &monkey_head_data.mTextureCoords[0], GL_STATIC_DRAW);


	//	This is for texture coordinates which you don't currently need, so I have commented it out
	//	glEnableVertexAttribArray (loc3);
	//	glBindBuffer (GL_ARRAY_BUFFER, vt_vbo);
	//	glVertexAttribPointer (loc3, 2, GL_FLOAT, GL_FALSE, 0, NULL);

	//LOADING LEFT HAND 
	leftArm_data = meshLoader.load_mesh(LEFT_ARM);

	glGenBuffers(1, &leftArm_vp_vbo);
	glBindBuffer(GL_ARRAY_BUFFER, leftArm_vp_vbo);
	glBufferData(GL_ARRAY_BUFFER, leftArm_data.mPointCount * sizeof(vec3), &leftArm_data.mVertices[0], GL_STATIC_DRAW);
	glGenBuffers(1, &leftArm_vn_vbo);
	glBindBuffer(GL_ARRAY_BUFFER, leftArm_vn_vbo);
	glBufferData(GL_ARRAY_BUFFER, leftArm_data.mPointCount * sizeof(vec3), &leftArm_data.mNormals[0], GL_STATIC_DRAW);

	//LOADING RIGHT HAND 
	rightArm_data = meshLoader.load_mesh(RIGHT_ARM);

	glGenBuffers(1, &rightArm_vp_vbo);
	glBindBuffer(GL_ARRAY_BUFFER, rightArm_vp_vbo);
	glBufferData(GL_ARRAY_BUFFER, rightArm_data.mPointCount * sizeof(vec3), &rightArm_data.mVertices[0], GL_STATIC_DRAW);
	glGenBuffers(1, &rightArm_vn_vbo);
	glBindBuffer(GL_ARRAY_BUFFER, rightArm_vn_vbo);
	glBufferData(GL_ARRAY_BUFFER, rightArm_data.mPointCount * sizeof(vec3), &rightArm_data.mNormals[0], GL_STATIC_DRAW);

	//LOADING HAT ----------------------------------------------------------------------------
	hat_data = meshLoader.load_mesh(HAT);

	glGenBuffers(1, &hat_vp_vbo);
	glBindBuffer(GL_ARRAY_BUFFER, hat_vp_vbo);
	glBufferData(GL_ARRAY_BUFFER, hat_data.mPointCount * sizeof(vec3), &hat_data.mVertices[0], GL_STATIC_DRAW);
	glGenBuffers(1, &hat_vn_vbo);
	glBindBuffer(GL_ARRAY_BUFFER, hat_vn_vbo);
	glBufferData(GL_ARRAY_BUFFER, hat_data.mPointCount * sizeof(vec3), &hat_data.mNormals[0], GL_STATIC_DRAW);

	//	This is for texture coordinates which you don't currently need, so I have commented it out
	//	unsigned int vt_vbo = 0;
	//	glGenBuffers (1, &vt_vbo);
	//	glBindBuffer (GL_ARRAY_BUFFER, vt_vbo);
	//	glBufferData (GL_ARRAY_BUFFER, monkey_head_data.mTextureCoords * sizeof (vec2), &monkey_head_data.mTextureCoords[0], GL_STATIC_DRAW);

}
#pragma endregion VBO_FUNCTIONS


void display() {

	// tell GL to only draw onto a pixel if the shape is closer to the viewer
	glEnable(GL_DEPTH_TEST); // enable depth-testing
	glDepthFunc(GL_LESS); // depth-testing interprets a smaller value as "closer"
	glClearColor(0.6f, 0.6f, 0.6f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glUseProgram(shaderProgramID);


	//Declare your uniform variables that will be used in your shader
	int matrix_location = glGetUniformLocation(shaderProgramID, "model");
	int view_mat_location = glGetUniformLocation(shaderProgramID, "view");
	int proj_mat_location = glGetUniformLocation(shaderProgramID, "proj");

	loc1 = glGetAttribLocation(shaderProgramID, "vertex_position");
	loc2 = glGetAttribLocation(shaderProgramID, "vertex_normal");
	loc3 = glGetAttribLocation(shaderProgramID, "vertex_texture");

	// Root of the Hierarchy
	mat4 view = camera.GetViewMatrix(); 
	mat4 persp_proj = perspective(camera.Zoom, (float)width / (float)height, 0.1f, 1000.0f);
	view = translate(view, vec3(0.0, 0.0, -10.0f));

	// update uniforms & draw
	glUniformMatrix4fv(proj_mat_location, 1, GL_FALSE, persp_proj.m);
	glUniformMatrix4fv(view_mat_location, 1, GL_FALSE, view.m);
	
	mountains.draw(); 
	

	mat4 snowman = identity_mat4();
	snowman = translate(snowman, snowman_pos);
	snowman = rotate_y_deg(snowman, -90);
	glEnableVertexAttribArray(loc1);
	glBindBuffer(GL_ARRAY_BUFFER, snowman_vp_vbo);
	glVertexAttribPointer(loc1, 3, GL_FLOAT, GL_FALSE, 0, NULL);
	glEnableVertexAttribArray(loc2);
	glBindBuffer(GL_ARRAY_BUFFER, snowman_vn_vbo);
	glVertexAttribPointer(loc2, 3, GL_FLOAT, GL_FALSE, 0, NULL);

	glUniformMatrix4fv(matrix_location, 1, GL_FALSE, snowman.m);
	glDrawArrays(GL_TRIANGLES, 3, snowman_data.mPointCount);

	mat4 hat = identity_mat4();
	hat = translate(hat, vec3(0.0f, 3.5f, 0.0f));
	hat = rotate_y_deg(hat, rotate_y);
	hat = snowman * hat;
	glEnableVertexAttribArray(loc1);
	glBindBuffer(GL_ARRAY_BUFFER, hat_vp_vbo);
	glVertexAttribPointer(loc1, 3, GL_FLOAT, GL_FALSE, 0, NULL);
	glEnableVertexAttribArray(loc2);
	glBindBuffer(GL_ARRAY_BUFFER, hat_vn_vbo);
	glVertexAttribPointer(loc2, 3, GL_FLOAT, GL_FALSE, 0, NULL);

	glUniformMatrix4fv(matrix_location, 1, GL_FALSE, hat.m);
	glDrawArrays(GL_TRIANGLES, 3, hat_data.mPointCount);

	mat4 leftArm = identity_mat4();
	leftArm = translate(leftArm, left_arm_pos);
	leftArm = snowman * leftArm;
	glEnableVertexAttribArray(loc1);
	glBindBuffer(GL_ARRAY_BUFFER, leftArm_vp_vbo);
	glVertexAttribPointer(loc1, 3, GL_FLOAT, GL_FALSE, 0, NULL);
	glEnableVertexAttribArray(loc2);
	glBindBuffer(GL_ARRAY_BUFFER, leftArm_vn_vbo);
	glVertexAttribPointer(loc2, 3, GL_FLOAT, GL_FALSE, 0, NULL);

	glUniformMatrix4fv(matrix_location, 1, GL_FALSE, leftArm.m);
	glDrawArrays(GL_TRIANGLES, 3, leftArm_data.mPointCount);

	mat4 rightArm = identity_mat4();
	rightArm = translate(rightArm, right_arm_pos);
	
	rightArm = snowman * rightArm;
	glEnableVertexAttribArray(loc1);
	glBindBuffer(GL_ARRAY_BUFFER, rightArm_vp_vbo);
	glVertexAttribPointer(loc1, 3, GL_FLOAT, GL_FALSE, 0, NULL);
	glEnableVertexAttribArray(loc2);
	glBindBuffer(GL_ARRAY_BUFFER, rightArm_vn_vbo);
	glVertexAttribPointer(loc2, 3, GL_FLOAT, GL_FALSE, 0, NULL);

	glUniformMatrix4fv(matrix_location, 1, GL_FALSE, rightArm.m);
	glDrawArrays(GL_TRIANGLES, 3, rightArm_data.mPointCount);

	//// Set up the child matrix
	//mat4 modelChild = identity_mat4();
	//modelChild = rotate_z_deg(modelChild, 180);
	//modelChild = rotate_y_deg(modelChild, rotate_y);
	//modelChild = translate(modelChild, vec3(0.0f, 1.9f, 0.0f));

	//// Apply the root matrix to the child matrix
	//modelChild = model * modelChild;

	//// Update the appropriate uniform and draw the mesh again
	//glUniformMatrix4fv(matrix_location, 1, GL_FALSE, modelChild.m);
	//glDrawArrays(GL_TRIANGLES, 0, mesh_data.mPointCount);
	
	
	//glDrawArrays(GL_TRIANGLES, 0, snowman_data.mPointCount);
	glutSwapBuffers();
}


void updateScene() {

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

	// Draw the next frame
	glutPostRedisplay();
}


void init()
{
	// Set up the shaders
	shaderProgramID = CompileShaders();
	// load mesh into a vertex buffer array
	generateObjectBufferMesh();

}

void keypress(unsigned char key, int x, int y) {
	if (key == 'w') {
		camera.ProcessKeyboard(FORWARD, 0.2);
	}
	else if (key == 'a') {
		camera.ProcessKeyboard(LEFT, 0.2);
	}
	else if (key == 'd') {
		camera.ProcessKeyboard(RIGHT, 0.2);
	}
	else if (key == 's') {
		camera.ProcessKeyboard(BACKWARD, 0.2);
	}
	else if (key == 'm') {
		rotate_y += 10.0f;
		rotate_y = fmodf(rotate_y, 360.0f);
	}
}
void mouseCallback(int x, int y) {

	float xoffset = x - lastX;
	float yoffset = lastY - y; // reversed since y-coordinates go from bottom to top

	lastX = x;
	lastY = y;

	camera.ProcessMouseMovement(xoffset, yoffset);

	glutPostRedisplay();

	//this is the main thing that keeps it from leaving the screen
	if (x < 100 || x > width - 100) {  //you can use values other than 100 for the screen edges if you like, kind of seems to depend on your mouse sensitivity for what ends up working best
		lastX = width / 2;   //centers the last known position, this way there isn't an odd jump with your cam as it resets
		lastY = height / 2;
		glutWarpPointer(width / 2, height / 2);  //centers the cursor
	}
	else if (y < 100 || y > height - 100) {
		lastX = width / 2;
		lastY = height / 2;
		glutWarpPointer(width / 2, height / 2);
	}
}
void mouse(int button, int state, int x, int y)
{
	if (button == 4) {
		camera.ProcessMouseScroll(2.0f);
	}
	if (button == 3) {
		camera.ProcessMouseScroll(-2.0f);
	}
}


int main(int argc, char** argv) {

	// Set up the window
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
	glutInitWindowSize(width, height);
	glutCreateWindow("Winter Wonderland");

	// Tell glut where the display function is
	glutDisplayFunc(display);
	glutIdleFunc(updateScene);
	glutKeyboardFunc(keypress);
	glutPassiveMotionFunc(mouseCallback);
	glutMouseFunc(mouse);
	glutSetCursor(GLUT_CURSOR_NONE);
	// A call to glewInit() must be done after glut is initialized!
	GLenum res = glewInit();
	// Check for any errors
	if (res != GLEW_OK) {
		fprintf(stderr, "Error: '%s'\n", glewGetErrorString(res));
		return 1;
	}
	// Set up your objects and shaders
	init();
	// Begin infinite event loop
	glutMainLoop();
	return 0;
}
