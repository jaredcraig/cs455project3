/*
 * model.h
 *
 *  Created on: Sep 16, 2015
 *      Author: jared
 */
#pragma once
#include "common_header.h"
using namespace std;

#define PI 3.14159265359

// libstem gamepad
//-----------------------------------------------------------------------------
typedef struct {
	int width;
	int height;
	char* title;
	float field_of_view_angle;
	float z_near;
	float z_far;
} glutWindow;

//-----------------------------------------------------------------------------
struct OBJ {
	vector<glm::vec3> vertices;
	vector<glm::vec2> uvs;
	vector<glm::vec3> normals;
	glm::mat4 Model;
};

//-----------------------------------------------------------------------------
class MyModel {
public:
	MyModel();
	vector<glm::vec3> temp_vertices;
	vector<glm::vec2> temp_uvs;
	vector<glm::vec3> temp_normals;
	vector<int> vertex_indices, uv_indices, normal_indices;
	vector<OBJ> objs;
	glm::mat4 Projection;
	glm::mat4 View;
	glm::mat4 camera;
	float cameraAngle;

	void loadObj(char*);
	glm::mat4 getMVP();
	glm::mat4 getView();
	void translate(glm::mat4&, glm::vec3);
	void rotate(glm::mat4&, float);
	void scale(glm::mat4&, float);
	void multiply(glm::mat4&, glm::mat4&);
	void translateCam(glm::vec3);
	void rotateCam(float, glm::vec3);
	void parseV(string&);
	void parseF(string&);
	void parseFaceLine(string, vector<int>&);
	void buildTriangles(OBJ &);
	void clear();
	void printm(glm::mat4&);
};
