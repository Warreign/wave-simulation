#include <iostream>
#include <stdexcept>
#include <AntTweakBar.h>
#include <glm/ext.hpp>

#include "pgr.h"
#include "data.h"
#include "visualization/object.h"
#include "visualization/camera.h"
#include "visualization/shader.h"
#include "visualization/mesh.h"

#include "simulation/amplitude_grid.h"

const unsigned int WIDTH = 1000;
const unsigned int HEIGHT = 1000;
static const char* TITLE = "Water Wave Simulation"; 
int Camera::refreshRate = 30;

TwBar* settingsBar;
bool keys[256];
bool skeys[256];

Shader* commonShader;
WaterShader* waterShader;


AmplitudeGrid* simulationGrid;
WaterMesh* waterMesh;
Water* water;

Skybox* skybox;
Camera* camera;

bool update = true;
bool drawWireframe = false;
bool drawSkybox = true;	

float mult = 0.1f;
float amplMultiplier = 2.0f;

double timeMultiplier = 5.0;
uint32_t frameStart = 0;
uint32_t frameEnd = 0;
uint32_t deltaTime = 0;
uint32_t framecount = 0;
double framerate = Camera::refreshRate;
double averageFrameTime = double(1000) / Camera::refreshRate;

void preRender()
{
	Camera::active->updateMatrices();

	uint32_t lastStart = frameStart;
	frameStart = glutGet(GLUT_ELAPSED_TIME);
	float dt = (frameStart - lastStart) * 0.001 * timeMultiplier;

	if (update)
	{
		simulationGrid->timeStep(dt);
		waterMesh->updateData(amplMultiplier);
		waterMesh->setProfileBuffer(simulationGrid->profileBuffers[0]);
	}
}

void render()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	if (!drawWireframe)
		water->draw(*Camera::active);
	else
		water->draw(*Camera::active, GL_LINE);
	if (drawSkybox)
		skybox->draw(*Camera::active);

	TwDraw();
	glutSwapBuffers();
}

void postRender()
{
	while (glutGet(GLUT_ELAPSED_TIME) - frameStart < 1000 / Camera::refreshRate)
	{
	}

	frameEnd = glutGet(GLUT_ELAPSED_TIME);
	deltaTime += frameEnd - frameStart;
	framecount++;

	if (deltaTime >= 1000)
	{
		framerate = (double)framecount * 0.5 + framerate * 0.5;
		framecount = 0;
		deltaTime = 0;
		averageFrameTime = 1000.0 / (framerate == 0 ? 0.001 : framerate);
	}

	glutPostRedisplay();
}

void displayCallback()
{
	preRender();
	render();
	postRender();
}

void reshapeCallback(int width, int height) 
{
	glViewport(0, 0, width, height);
	TwWindowSize(width, height);
}

void initShaders() 
{
	commonShader = new Shader("shaders/standard.vert", "shaders/standard.frag");
	waterShader = new WaterShader();
}

void TW_CALL FullscreenToggleCB(void* p)
{
	glutFullScreenToggle();
}

void TW_CALL setCB(const void* v, void* c)
{
	mult = *(float*)v;
	waterShader->setFloat("mult", mult);
}

void TW_CALL getCB(void* v, void* c)
{
	*(float*)v = mult;
}

void initSettingsBar()
{
	settingsBar = TwNewBar("Settings");
	TwDefine(" Settings size='240 420' color='20 20 20' fontsize=3"); 

	TwAddSeparator(settingsBar, "sim.separator", "label='Simulation'");
	TwAddVarRW(settingsBar, "sim.update", TW_TYPE_BOOLCPP, &update, "label='Update'");
	TwAddVarRW(settingsBar, "sim.direction", TW_TYPE_INT32, &simulationGrid->defaultDirection, "min=0 max=15 label='Default direction'");
	TwAddVarCB(settingsBar, "sim.wind", TW_TYPE_FLOAT, AmplitudeGrid::SetWindSpeedCB, AmplitudeGrid::GetWindSpeedCB, simulationGrid, "label='Wind Speed' min=0.5 step=0.5");
	TwAddVarCB(settingsBar, "sim.amplitude", TW_TYPE_FLOAT, AmplitudeGrid::SetAmplitudeCB, AmplitudeGrid::GetAmplitudeCB, simulationGrid, "label='Amplitude' min=0.0 step=0.05 precision=2");
	TwAddVarRW(settingsBar, "sim.timemult", TW_TYPE_DOUBLE, &timeMultiplier, "label='Time Multiplier' min=0.1 max=10.0 precision=3 step=0.1");
	TwAddVarRW(settingsBar, "sim.amplitudemult", TW_TYPE_FLOAT, &amplMultiplier, "label='Amplitude Mult.' min=0.0 max=20.0 step=0.2");
	TwAddVarCB(settingsBar, "sim.mult", TW_TYPE_FLOAT, setCB, getCB, &mult, "label='Multiplier' step=0.1");

	TwAddSeparator(settingsBar, "visual.separator", "label='Visualization'");
	TwAddVarRW(settingsBar, "visual.skybox", TW_TYPE_BOOLCPP, &drawSkybox, "label='Display Skybox'");
	TwAddVarRW(settingsBar, "visual.wireframe", TW_TYPE_BOOLCPP, &drawWireframe, "label='Display Wireframe'");
	TwAddVarRW(settingsBar, "visual.ambient", TW_TYPE_COLOR3F, &water->ambient, "label='Ambient' group='Color'");
	TwAddVarRW(settingsBar, "visual.diffuse", TW_TYPE_COLOR3F, &water->diffuse, "label='Diffuse' group='Color'");
	TwAddVarRW(settingsBar, "visual.specular", TW_TYPE_COLOR3F, &water->specular, "label='Specular' group='Color'");
	TwAddVarRW(settingsBar, "visual.shininess", TW_TYPE_FLOAT, &water->shininess, "label='Shininess' min=0.0 group='Color'");

	TwAddSeparator(settingsBar, "camera.separator", "label='Camera'");
	TwAddVarCB(settingsBar, "camera.speed", TW_TYPE_FLOAT, Camera::SetSpeedCB, Camera::GetSpeedCB, Camera::active, "label='Speed' min=0.0 step=5.0");
	TwAddVarCB(settingsBar, "camera.fov", TW_TYPE_FLOAT, Camera::SetFovCB, Camera::GetFovCB, Camera::active, "label='FOV' min=10.0 max=180.0");
	TwAddVarCB(settingsBar, "camera.sensitivity", TW_TYPE_FLOAT, Camera::SetSensitivityCB, Camera::GetSensitivityCB, Camera::active, "label='Sensitivity' min=0.1 max=3.0 step=0.1");
	TwAddVarCB(settingsBar, "camera.farPlane", TW_TYPE_FLOAT, Camera::SetFarPlaneCB, Camera::GetFarPlaneCB, Camera::active, "label='Far Plane' min=50.0 step=10.0");
	TwAddVarRW(settingsBar, "camera.fps", TW_TYPE_INT32, &Camera::refreshRate, "label='FPS' min=10 max=120 step=1");
	TwAddButton(settingsBar, "camera.freemode", Camera::ToggleFreeModeCB, Camera::active, "label='Toggle Free Mode'");
	TwAddButton(settingsBar, "fullscreen", FullscreenToggleCB, NULL, "label='Toggle Fullscreen'");
	TwAddVarRO(settingsBar, "fps", TW_TYPE_DOUBLE, &framerate, "label='FPS'");
	TwAddVarRO(settingsBar, "frametime", TW_TYPE_DOUBLE, &averageFrameTime, "label='ft'");
}

void initData() 
{
	skybox = new Skybox();

	camera = new Camera(glm::vec3(0, 50, 0), glm::vec3(0, -1, 0), 0.1f, 100.0f, 80.0f, 30.0f);
	camera->makeActive();

	simulationGrid = new AmplitudeGrid(
		100.0f, //size
		0.01f, // min wave length
		10.0f, // max wave length
		10.0f, // wind speed
		128, // number of spatial disc. nodes
		16, // number of wave angle disc. nodes
		1 // number of wave length disc. nodes
	);

	waterMesh = new WaterMesh(waterShader, simulationGrid, 100, 100.0f);
	water = new Water(waterMesh);
}

void motionCallback(int x, int y)
{
	if (TwEventMouseMotionGLUT(x,y))
		return;

	glm::vec3 planePoint = Camera::active->intersectPlane(glm::vec3(0, 1, 0), glm::vec3(0), glm::vec2(x, y));
	simulationGrid->addPointDisturbance(glm::vec2(planePoint.x, planePoint.z), 0.1);
}

void keysTimerCallback(int) 
{
	if (keys['w']) {
		camera->moveForward();
	}
	if (keys['s']) {
		camera->moveBackward();
	}
	if (keys['a']) {
		camera->moveLeft();
	}
	if (keys['d']) {
		camera->moveRight();
	}
	glutTimerFunc(1000U / Camera::refreshRate, keysTimerCallback, 0);
}

void specialCallback(int key, int x, int y)  
{
	if (TwEventSpecialGLUT(key, x, y))
		return;

	skeys[key] = true;
	switch (key) {
	case GLUT_KEY_F2:
		Camera::active->toggleFreeMode();
		break;
	case GLUT_KEY_F1:
		glutLeaveMainLoop();
		break;
	case GLUT_KEY_F11:
		glutFullScreenToggle();
		break;
	}
}

void keyboardCallback(unsigned char key, int x, int y) 
{
	if (TwEventKeyboardGLUT(key, x, y))
		return;

	keys[key] = true;
}

void specialUpCallback(int key, int x, int y)
{
	skeys[key] = false;
}

void keyboardUpCallback(unsigned char key, int x, int y) 
{
	keys[key] = false;
}

void initApp()
{
	glutInitContextVersion(4, 4);
	glutInitContextFlags(GLUT_CORE_PROFILE);
	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);
	glutInitWindowSize(WIDTH, HEIGHT);
	glutCreateWindow(TITLE);

	if (!pgr::initialize(4, 4, pgr::DEBUG_HIGH)) {
		throw std::runtime_error("pgr init error");
	}

	TwInit(TW_OPENGL_CORE, NULL);
	TwWindowSize(glutGet(GLUT_WINDOW_WIDTH), glutGet(GLUT_WINDOW_HEIGHT));

	glutIgnoreKeyRepeat(true);

	glutMouseFunc((GLUTmousebuttonfun)TwEventMouseButtonGLUT);
	//glutPassiveMotionFunc((GLUTmousemotionfun)TwEventMouseMotionGLUT);
	glutMotionFunc(motionCallback);
	glutSpecialFunc(specialCallback);
	glutKeyboardFunc(keyboardCallback);
	TwGLUTModifiersFunc(glutGetModifiers);

	glutKeyboardUpFunc(keyboardUpCallback);
	glutSpecialUpFunc(specialUpCallback);

	glutTimerFunc(averageFrameTime, keysTimerCallback, 0);

	glutDisplayFunc(displayCallback);
	glutReshapeFunc(reshapeCallback);

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_MULTISAMPLE);
}

void cleanup() 
{
	delete commonShader;
	delete skybox;
	delete water;
	delete waterMesh;
	delete simulationGrid;
	delete camera;

	TwDeleteBar(settingsBar);
	TwTerminate();
}

int main(int argc, char** argv)
{
	glutInit(&argc, argv);
	try 
	{
		initApp();
		initShaders();
		initData();
		initSettingsBar();
		glutMainLoop();
		cleanup();
	}
	catch (std::exception& e) 
	{
		pgr::dieWithError(e.what());
	}

	return 0;
}
