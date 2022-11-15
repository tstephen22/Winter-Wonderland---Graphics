#pragma once
// Windows includes (For Time, IO, etc.)
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
#include <fstream>
#include <iostream>
#include <sstream>


// Assimp includes
#include <assimp/cimport.h> // scene importer
#include <assimp/scene.h> // collects data
#include <assimp/postprocess.h> // various extra operations

// Project includes
#include "maths_funcs.h"

class Mesh {
public: 
	Mesh(const char* file_name) {
		ModelData modelData;

		/* Use assimp to read the model file, forcing it to be read as    */
		/* triangles. The second flag (aiProcess_PreTransformVertices) is */
		/* relevant if there are multiple meshes in the model file that   */
		/* are offset from the origin. This is pre-transform them so      */
		/* they're in the right position.                                 */
		const aiScene* scene = aiImportFile(
			file_name,
			aiProcess_Triangulate | aiProcess_PreTransformVertices
		);

		if (!scene) {
			fprintf(stderr, "ERROR: reading mesh %s\n", file_name);
		}

		printf("  %i materials\n", scene->mNumMaterials);
		printf("  %i meshes\n", scene->mNumMeshes);
		printf("  %i textures\n", scene->mNumTextures);

		for (unsigned int m_i = 0; m_i < scene->mNumMeshes; m_i++) {
			const aiMesh* mesh = scene->mMeshes[m_i];
			printf("    %i vertices in mesh\n", mesh->mNumVertices);
			modelData.mPointCount += mesh->mNumVertices;
			for (unsigned int v_i = 0; v_i < mesh->mNumVertices; v_i++) {
				if (mesh->HasPositions()) {
					const aiVector3D* vp = &(mesh->mVertices[v_i]);
					modelData.mVertices.push_back(vec3(vp->x, vp->y, vp->z));
				}
				if (mesh->HasNormals()) {
					const aiVector3D* vn = &(mesh->mNormals[v_i]);
					modelData.mNormals.push_back(vec3(vn->x, vn->y, vn->z));
				}
				if (mesh->HasTextureCoords(0)) {
					const aiVector3D* vt = &(mesh->mTextureCoords[0][v_i]);
					modelData.mTextureCoords.push_back(vec2(vt->x, vt->y));
				}
				//if (mesh->HasFaces()) {
				//	for (unsigned int i = 0; i < mesh->mNumFaces; i++) {
				//		aiFace face = mesh->mFaces[i];
				//		for (unsigned int j = 0; j < face.mNumIndices; j++) {
				//			modelData.mIndices.push_back(face.mIndices[j]);
				//		}
				//	}
				//}
				if (mesh->HasTangentsAndBitangents()) {
					/* You can extract tangents and bitangents here              */
					/* Note that you might need to make Assimp generate this     */
					/* data for you. Take a look at the flags that aiImportFile  */
					/* can take.                                                 */
				}
			}
		}

		aiReleaseImport(scene);
	}

	void draw(mat4 transform, GLuint matrix_location, GLuint texture, Shader shaderProgram) {
		glUniformMatrix4fv(matrix_location, 1, GL_FALSE, transform.m);
		glBindVertexArray(vao);
		glDrawArrays(GL_TRIANGLES, 0, modelData.mPointCount);
		glBindVertexArray(0);
	}
private: 
	typedef struct
	{
		size_t mPointCount = 0;
		std::vector<vec3> mVertices;
		std::vector<vec3> mNormals;
		std::vector<vec2> mTextureCoords;
	} ModelData;
};