#include "model.h"

glutWindow win;
GLuint texture[3];
MyModel model;

void loadOBJs();
int init();
void display();
void draw();
void drawParkingLot();
void drawCar();
void drawTires();
void drawChar();

//-----------------------------------------------------------------------------
int LoadGLTextures() {
	texture[0] = SOIL_load_OGL_texture("ParkingLot.bmp", SOIL_LOAD_AUTO,
			SOIL_CREATE_NEW_ID, SOIL_FLAG_INVERT_Y);
	if (texture[0] == 0)
		return false;

	texture[1] = SOIL_load_OGL_texture("car.bmp", SOIL_LOAD_AUTO,
			SOIL_CREATE_NEW_ID, SOIL_FLAG_INVERT_Y);
	if (texture[1] == 0)
		return false;

	texture[2] = SOIL_load_OGL_texture("tire.bmp", SOIL_LOAD_AUTO,
			SOIL_CREATE_NEW_ID, SOIL_FLAG_INVERT_Y);
	if (texture[2] == 0)
		return false;

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	return true;
}

//-----------------------------------------------------------------------------
void pollJoystick(unsigned int buttonmask, int x, int y, int z) {
	GLfloat move = 0.25;

	if (abs(x) > 500) {
		model.rotateCam(abs(x) / x, glm::vec3(0, 1, 0));
	}

	if (abs(y) > 500) {
		model.rotateCam(abs(y) / y, glm::vec3(1, 0, 0));
	}

	if (buttonmask & 0x01) {	// BACK
		model.translateCam(glm::vec3(0, 0, -move));
	}

	if (buttonmask & 0x02) {	// RIGHT
		model.translateCam(glm::vec3(move, 0, 0));
	}

	if (buttonmask & 0x04) {	// LEFT
		model.translateCam(glm::vec3(-move, 0, 0));
	}

	if (buttonmask & 0x08) {	// FORWARD
		model.translateCam(glm::vec3(0, 0, move));
	}
	if (buttonmask & 0x10) {	// TURN LEFT
		model.rotate(model.objs[2].Model, -3.0f);
		model.rotate(model.objs[3].Model, -3.0f);
	}
	if (buttonmask & 0x20) {	// TURN RIGHT
		model.rotate(model.objs[2].Model, 3.0f);
		model.rotate(model.objs[3].Model, 3.0f);
	}

	if (buttonmask & 0x40)
		cout << "40" << endl;

	if (buttonmask & 0x80)
		cout << "80" << endl;

	if (buttonmask != 0 || abs(y) > 100 || abs(x) > 100) {
		//printf("Joystick:{%d,%d}\n", x, y);
		glutPostRedisplay();
	}
}

//-----------------------------------------------------------------------------
void loadOBJs() {
	model.loadObj("ParkingLot.obj");
	model.loadObj("car.obj");
	model.translate(model.objs[1].Model, glm::vec3(-2.0f, 0.0f, -7.0f));
	model.rotate(model.objs[1].Model, 60.0f);
	model.loadObj("tire.obj");

	OBJ tire1 = model.objs[2];
	FOR (i, 3)
	{
		OBJ tire;
		tire.normals = tire1.normals;
		tire.uvs = tire1.uvs;
		tire.vertices = tire1.vertices;
		tire.Model = glm::mat4(1.0f);
		model.objs.push_back(tire);
	}

	model.translate(model.objs[2].Model, glm::vec3(0.4f, 0.22f, -0.5f));
	model.scale(model.objs[2].Model, 0.2f);

	model.translate(model.objs[3].Model, glm::vec3(-0.4f, 0.22f, -0.5f));
	model.rotate(model.objs[3].Model, 180.0f);
	model.scale(model.objs[3].Model, 0.2f);

	model.translate(model.objs[4].Model, glm::vec3(0.4f, 0.22f, 0.5f));
	model.scale(model.objs[4].Model, 0.2f);

	model.translate(model.objs[5].Model, glm::vec3(-0.4f, 0.22f, 0.5f));
	model.rotate(model.objs[5].Model, 180.0f);
	model.scale(model.objs[5].Model, 0.2f);

	model.loadObj("inquisitor.obj");
	model.translate(model.objs[6].Model, glm::vec3(0.0f, 0.4f, 0.0f));
	model.rotate(model.objs[6].Model, 180.0f);
	model.scale(model.objs[6].Model, 0.1f);
}

//-----------------------------------------------------------------------------
int init() {
	glewExperimental = true;
	if (glewInit() != GLEW_OK) {
		fprintf(stderr, "Failed to initialize GLEW\n");
		return 0;
	}
	loadOBJs();
	LoadGLTextures();

	glShadeModel(GL_SMOOTH);
	glClearColor(0.5f, 0.5f, 0.5f, 0.5f);
	glClearDepth(1.0f);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(45.0f, 4.0f / 3.0f, 0.1f, 1000.0f);
	glMatrixMode(GL_MODELVIEW);

	glEnable(GL_DEPTH_TEST);

	return true;
}

//-----------------------------------------------------------------------------
void draw() {
	drawParkingLot();
	drawCar();
	drawTires();
	//drawChar();
}

//-----------------------------------------------------------------------------
void drawParkingLot() {
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, texture[0]);

	OBJ parkingLot = model.objs[0];
	glm::mat4 MVP = model.getMVP() * parkingLot.Model;
	vector<glm::vec3> vertex = parkingLot.vertices;
	vector<glm::vec2> uv = parkingLot.uvs;

	for (unsigned i = 0; i < vertex.size(); i += 3) {
		glBegin(GL_TRIANGLES);
		for (int j = 2; j >= 0; j--) {
			glm::vec2 tex = uv[i + j];
			glm::vec3 p = vertex[i + j];
			glm::vec4 hp = glm::vec4(p.x, p.y, p.z, 1);
			glm::vec4 tp = MVP * hp;
			glTexCoord2f(tex.x, tex.y);
			glVertex3f(tp.x, tp.y, tp.z);
		}
		glEnd();
	}
}

//-----------------------------------------------------------------------------
void drawCar() {
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, texture[1]);

	OBJ car = model.objs[1];
	glm::mat4 MVP = model.getMVP() * car.Model;
	vector<glm::vec3> vertex = car.vertices;
	vector<glm::vec2> uv = car.uvs;

	/*	// DEBUG
	 glm::vec4 hpt = glm::vec4(vertex[0].x, vertex[0].y, vertex[0].z, 1);
	 glm::vec4 tp = MVP * hpt;
	 printf("Point %i: {%f,%f,%f} --> {%f,%f,%f}\n", 1, hpt.x, hpt.y, hpt.z,
	 tp.x, tp.y, tp.z);
	 */

	for (unsigned i = 0; i < car.vertices.size(); i += 3) {
		glBegin(GL_TRIANGLES);
		for (int j = 2; j >= 0; j--) {
			glm::vec2 tex = uv[i + j];
			glm::vec3 p = vertex[i + j];
			glm::vec4 hp = glm::vec4(p.x, p.y, p.z, 1);
			glm::vec4 tp = MVP * hp;
			glTexCoord2f(tex.x, tex.y);
			glVertex3f(tp.x, tp.y, tp.z);
		}
		glEnd();
	}
}

//-----------------------------------------------------------------------------
void drawTires() {
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, texture[2]);

	OBJ car = model.objs[1];

	// TIRE 1
	OBJ tire = model.objs[2];
	glm::mat4 MVP = model.getMVP() * car.Model * tire.Model;

	vector<glm::vec3> vertex = tire.vertices;
	vector<glm::vec2> uv = tire.uvs;

	for (unsigned i = 0; i < vertex.size(); i += 3) {
		glBegin(GL_TRIANGLES);
		for (int j = 2; j >= 0; j--) {
			glm::vec2 tex = uv[i + j];
			glm::vec3 p = vertex[i + j];
			glm::vec4 hp = glm::vec4(p.x, p.y, p.z, 1);
			glm::vec4 tp = MVP * hp;
			glTexCoord2f(tex.x, tex.y);
			glVertex3f(tp.x, tp.y, tp.z);
		}
		glEnd();
	}

	// TIRE 2
	tire = model.objs[3];
	MVP = model.getMVP() * car.Model * tire.Model;

	vertex = tire.vertices;
	uv = tire.uvs;

	for (unsigned i = 0; i < vertex.size(); i += 3) {
		glBegin(GL_TRIANGLES);
		for (int j = 2; j >= 0; j--) {
			glm::vec2 tex = uv[i + j];
			glm::vec3 p = vertex[i + j];
			glm::vec4 hp = glm::vec4(p.x, p.y, p.z, 1);
			glm::vec4 tp = MVP * hp;
			glTexCoord2f(tex.x, tex.y);
			glVertex3f(tp.x, tp.y, tp.z);
		}
		glEnd();
	}

	// TIRE 3
	tire = model.objs[4];
	MVP = model.getMVP() * car.Model * tire.Model;

	vertex = tire.vertices;
	uv = tire.uvs;

	for (unsigned i = 0; i < vertex.size(); i += 3) {
		glBegin(GL_TRIANGLES);
		for (int j = 2; j >= 0; j--) {
			glm::vec2 tex = uv[i + j];
			glm::vec3 p = vertex[i + j];
			glm::vec4 hp = glm::vec4(p.x, p.y, p.z, 1);
			glm::vec4 tp = MVP * hp;
			glTexCoord2f(tex.x, tex.y);
			glVertex3f(tp.x, tp.y, tp.z);
		}
		glEnd();
	}

	// TIRE 4
	tire = model.objs[5];
	MVP = model.getMVP() * car.Model * tire.Model;

	vertex = tire.vertices;
	uv = tire.uvs;

	for (unsigned i = 0; i < vertex.size(); i += 3) {
		glBegin(GL_TRIANGLES);
		for (int j = 2; j >= 0; j--) {
			glm::vec2 tex = uv[i + j];
			glm::vec3 p = vertex[i + j];
			glm::vec4 hp = glm::vec4(p.x, p.y, p.z, 1);
			glm::vec4 tp = MVP * hp;
			glTexCoord2f(tex.x, tex.y);
			glVertex3f(tp.x, tp.y, tp.z);
		}
		glEnd();
	}
}

//-----------------------------------------------------------------------------
void drawChar() {
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, texture[2]);

	OBJ car = model.objs[1];
	OBJ character = model.objs[6];
	glm::mat4 MVP = model.getMVP() * car.Model * character.Model;

	vector<glm::vec3> vertex = character.vertices;
	vector<glm::vec2> uv = character.uvs;

	for (unsigned i = 0; i < vertex.size(); i += 3) {
		glBegin(GL_TRIANGLES);
		for (int j = 2; j >= 0; j--) {
			glm::vec2 tex = uv[i + j];
			glm::vec3 p = vertex[i + j];
			glm::vec4 hp = glm::vec4(p.x, p.y, p.z, 1);
			glm::vec4 tp = MVP * hp;
			glTexCoord2f(tex.x, tex.y);
			glVertex3f(tp.x, tp.y, tp.z);
		}
		glEnd();
	}
}

//-----------------------------------------------------------------------------
void display(void) {

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	draw();

	GLenum err;
	while ((err = glGetError()) != GL_NO_ERROR) {
		cerr << "OpenGL error: " << gluErrorString(err) << endl;
	}
	glutSwapBuffers();
	glFlush();
}

//-----------------------------------------------------------------------------
int main(int argc, char** argv) {
	win.width = 640;
	win.height = 480;
	win.title = "CS455 Program 3";
	win.field_of_view_angle = 45;
	win.z_near = 1.0f;
	win.z_far = 500.0f;

	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
	glutInitWindowSize(win.width, win.height);
	glutCreateWindow(win.title);
	glutDisplayFunc(display);
	init();

	glutJoystickFunc(pollJoystick, 25);
	glutMainLoop();
	return 0;
}

//-----------------------------------------------------------------------------
