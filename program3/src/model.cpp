/*
 * model.cpp
 *
 *  Created on: Sep 16, 2015
 *      Author: jared
 */
#include "model.h"

//-----------------------------------------------------------------------------
MyModel::MyModel() {
	cameraAngle = 0.0f;
	Projection = glm::perspective(45.0f, 4.0f / 3.0f, 0.1f, 1000.f);
	camera = glm::inverse(
			glm::lookAt(glm::vec3(-4.0f, -3.0f, -3.0f),
					glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f)));
	camera = glm::translate(camera, glm::vec3(-1, 4, -15));
}

//-----------------------------------------------------------------------------
void MyModel::translateCam(glm::vec3 t) {
	camera = glm::translate(camera, t);
}

//-----------------------------------------------------------------------------
void MyModel::rotateCam(float f, glm::vec3 v) {
	camera = glm::rotate(camera, f, v);
}

//-----------------------------------------------------------------------------
glm::mat4 MyModel::getView() {
	return glm::inverse(camera);
}

//-----------------------------------------------------------------------------
glm::mat4 MyModel::getMVP() {
	return Projection * getView();
}

//-----------------------------------------------------------------------------
void MyModel::printm(glm::mat4& m) {
	ostringstream oss;
	FOR (i,4)
	{
		oss << m[i][0] << " ";
		oss << m[i][1] << " ";
		oss << m[i][2] << " ";
		oss << m[i][3] << endl;
	}
	cout << oss.str();
}
//-----------------------------------------------------------------------------
void MyModel::translate(glm::mat4& Model, glm::vec3 t) {
	Model = glm::translate(Model, t);
}

//-----------------------------------------------------------------------------
void MyModel::rotate(glm::mat4& Model, float theta) {
	Model = glm::rotate(Model, theta, glm::vec3(0, 1, 0));
}

//-----------------------------------------------------------------------------
void MyModel::scale(glm::mat4& Model, float s) {
	Model = glm::scale(Model, glm::vec3(s, s, s));
}

//-----------------------------------------------------------------------------
void MyModel::multiply(glm::mat4& m1, glm::mat4& m2) {
	m1 *= m2;
}

//-----------------------------------------------------------------------------
void MyModel::clear() {
	temp_vertices.clear();
	temp_uvs.clear();
	temp_normals.clear();
	vertex_indices.clear();
	uv_indices.clear();
	normal_indices.clear();
}
//-----------------------------------------------------------------------------
void MyModel::parseFaceLine(string line, vector<int> &face_line) {
	line[0] = ' ';
	string parsed_line;
	string el;
	istringstream ss(line);

	while (!ss.eof()) {
		ss >> el;
		parsed_line += el;
		parsed_line += '/';
	}

	ss.clear();
	ss.str(parsed_line);

	while (getline(ss, el, '/')) {
		face_line.push_back(atoi(el.c_str()));
	}
}

//-----------------------------------------------------------------------------
void MyModel::parseV(string &line) {
	line[0] = ' ';
	char c = line.c_str()[1];

	if (c == 't') {
		line[1] = ' ';
		glm::vec2 uv;
		sscanf(line.c_str(), "%f %f", &uv.x, &uv.y);
		temp_uvs.push_back(uv);
	} else if (c == 'n') {
		glm::vec3 normal;
		line[1] = ' ';
		sscanf(line.c_str(), "%f %f %f", &normal.x, &normal.y, &normal.z);
		temp_normals.push_back(normal);
	} else {
		glm::vec3 vertex;
		sscanf(line.c_str(), "%f %f %f ", &vertex.x, &vertex.y, &vertex.z);
		temp_vertices.push_back(vertex);
	}
}

//-----------------------------------------------------------------------------
void MyModel::parseF(string &line) {

	vector<int> faces;
	parseFaceLine(line, faces);

	vertex_indices.push_back(faces[0]);
	vertex_indices.push_back(faces[3]);
	vertex_indices.push_back(faces[6]);

	uv_indices.push_back(faces[1]);
	uv_indices.push_back(faces[4]);
	uv_indices.push_back(faces[7]);

	normal_indices.push_back(faces[2]);
	normal_indices.push_back(faces[5]);
	normal_indices.push_back(faces[8]);
}

//-----------------------------------------------------------------------------
void MyModel::buildTriangles(OBJ &obj) {
	for (unsigned int i = 0; i < vertex_indices.size(); i++) {

		// Get the indices of its attributes
		unsigned int vertexIndex = vertex_indices[i];
		unsigned int uvIndex = uv_indices[i];
		unsigned int normalIndex = normal_indices[i];

		// Get the attributes thanks to the index
		glm::vec3 vertex = temp_vertices[vertexIndex - 1];
		glm::vec2 uv = temp_uvs[uvIndex - 1];
		glm::vec3 normal = temp_normals[normalIndex - 1];

		// Put the attributes in buffers
		obj.vertices.push_back(vertex);
		obj.uvs.push_back(uv);
		obj.normals.push_back(normal);
	}
}

//-----------------------------------------------------------------------------
void MyModel::loadObj(char* name) {
	clear();
	string line;
	ifstream file(name);

	struct OBJ obj;
	obj.Model = glm::mat4(1.0f);

	if (file.is_open()) {
		while (!file.eof()) {
			getline(file, line);
			switch (line.c_str()[0]) {
			case 'v':
				parseV(line);
				break;
			case 'f':
				parseF(line);
				break;
			}
		}
		file.close();
		buildTriangles(obj);

	} else {
		cout << "Unable to open file";
	}
	objs.push_back(obj);
}
