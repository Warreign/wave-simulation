#include <iostream>
#include <stdexcept>
#include <AntTweakBar.h>
#include <IL/il.h>
#include <glm/ext.hpp>

//#include "pgr.h"
#include "data.h"
#include "visualization/window.h"
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

Window* window;
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

double timeMultiplier = -0.9;
double frameStart = 0;
double frameEnd = 0;
double deltaTime = 0;
uint32_t framecount = 0;
double framerate = Camera::refreshRate;
double averageFrameTime = double(1000) / Camera::refreshRate;

static void glfw_error_callback(int error, const char* description)
{
	throw std::runtime_error(description);
}

void preRender()
{
	Camera::active->updateMatrices();

	double lastStart = frameStart;
	frameStart = glfwGetTime();
	//float dt = (frameStart - lastStart) * timeMultiplier;

	double dt = simulationGrid->cflTimeStep() * pow(10, timeMultiplier);

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
	glClearColor(0.0f, 1.0f, 0.0f, 1.0f);
	if (!drawWireframe)
		water->draw(*Camera::active);
	else
		water->draw(*Camera::active, GL_LINE);
	if (drawSkybox)
		skybox->draw(*Camera::active);

	TwDraw();
	window->onUpdate();
}

void postRender()
{
	//while (glfwGetTime() - frameStart < 1.0 / Camera::refreshRate)
	//{
	//}

	frameEnd = glfwGetTime();
	deltaTime += frameEnd - frameStart;
	framecount++;

	if (deltaTime >= 1.0)
	{
		framerate = (double)framecount * 0.5 + framerate * 0.5;
		framecount = 0;
		deltaTime = 0;
		averageFrameTime = 1.0 / (framerate == 0 ? 0.001 : framerate);
	}
}

void displayCallback()
{
	preRender();
	render();
	postRender();
}

void reshapeCallback( GLFWwindow* w, int width, int height) 
{
	glViewport(0, 0, width, height);
	TwWindowSize(width, height);
}

void initShaders() 
{
	std::cout << "INFO: Initializing shaders" << std::endl;

	commonShader = new Shader("shaders/standard.vert", "shaders/standard.frag");
	waterShader = new WaterShader();
}

void TW_CALL FullscreenToggleCB(void* p)
{
	//glutFullScreenToggle();
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
	std::cout << "INFO: Initializing AntTweakBar" << std::endl;

	settingsBar = TwNewBar("Settings");
	TwDefine(" Settings size='240 420' color='20 20 20' fontsize=3"); 

	TwAddSeparator(settingsBar, "sim.separator", "label='Simulation'");
	TwAddVarRW(settingsBar, "sim.update", TW_TYPE_BOOLCPP, &update, "label='Update'");
	TwAddVarRW(settingsBar, "sim.direction", TW_TYPE_INT32, &simulationGrid->defaultDirection, "min=0 max=15 label='Default direction'");
	TwAddVarCB(settingsBar, "sim.wind", TW_TYPE_FLOAT, AmplitudeGrid::SetWindSpeedCB, AmplitudeGrid::GetWindSpeedCB, simulationGrid, "label='Wind Speed' min=0.5 step=0.5");
	TwAddVarCB(settingsBar, "sim.amplitude", TW_TYPE_FLOAT, AmplitudeGrid::SetAmplitudeCB, AmplitudeGrid::GetAmplitudeCB, simulationGrid, "label='Amplitude' min=0.0 step=0.05 precision=2");
	TwAddVarRW(settingsBar, "sim.timemult", TW_TYPE_DOUBLE, &timeMultiplier, "label='Time Multiplier' min=-2.0 max=2.0 precision=3 step=0.1");
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
	std::cout << "INFO: Initializing the data" << std::endl;

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

void pointDisturbance(double x, double y)
{
	glm::vec3 planePoint = Camera::active->intersectPlane(glm::vec3(0, 1, 0), glm::vec3(0), glm::vec2(x, y));
	simulationGrid->addPointDisturbance(glm::vec2(planePoint.x, planePoint.z), 0.1);
}

void motionCallback(GLFWwindow* w, double x, double y)
{
	TwEventMousePosGLFW3(w, x, y);
	//return;
	pointDisturbance(x, y);
}

void buttonCallback(GLFWwindow* w, int button, int action, int mods)
{
	TwEventMouseButtonGLFW3(w, button, action, mods);

	double x, y;
	glfwGetCursorPos(w, &x, &y);

	pointDisturbance(x, y);
}

void keyCallback(GLFWwindow* w, int key, int scancode, int action, int mods)
{
	TwEventKeyGLFW3(w, key, scancode, action, mods);

	switch (action) 
	{
		case GLFW_PRESS:
			switch (key)
			{
				case GLFW_KEY_F2:
					Camera::active->toggleFreeMode();
					break;
				case GLFW_KEY_F1:
					break;
				case GLFW_KEY_F11:
					break;
			}
			break;
		case GLFW_RELEASE:
			break;
		case GLFW_REPEAT:
			break;
	}
}

void initApp()
{
	std::cout << "INFO: Initializing the app" << std::endl;

	window = new Window;

	ilInit();
	ilEnable(IL_ORIGIN_SET);
	ilOriginFunc(IL_ORIGIN_LOWER_LEFT);


	TwInit(TW_OPENGL_CORE, NULL);
	TwWindowSize(window->getWidth(), window->getHeight());

	//glfwSetCursorPosCallback(window->getHandle(), motionCallback);
	glfwSetCursorPosCallback(window->getHandle(), (GLFWcursorposfun)TwEventMousePosGLFW3);

	glfwSetMouseButtonCallback(window->getHandle(), buttonCallback);

	glfwSetKeyCallback(window->getHandle(), keyCallback);

	glfwSetCharCallback(window->getHandle(), (GLFWcharfun)TwEventCharGLFW3);
	glfwSetScrollCallback(window->getHandle(), (GLFWscrollfun)TwEventMouseWheelGLFW3);



	glfwSetFramebufferSizeCallback(window->getHandle(), reshapeCallback);
	//glutReshapeFunc(reshapeCallback);

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_MULTISAMPLE);

	glfwSwapInterval(0);

}

void initGLFWApp()
{

}

void cleanup() 
{
	std::cout << "INFO: Finalizing the application" << std::endl;

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
	try 
	{
		initApp();
		initShaders();
		initData();
		initSettingsBar();
		//glutMainLoop();

		while (!glfwWindowShouldClose(window->getHandle()))
		{
			preRender();
			render();
			postRender();
		}

		cleanup();
	}
	catch (std::exception& e) 
	{
		//pgr::dieWithError(e.what());
		std::cout << e.what() << std::endl;
	}

	return 0;
}
