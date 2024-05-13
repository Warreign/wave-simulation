#include <iostream>
#include <stdexcept>
#include <IL/il.h>
#include <glm/ext.hpp>
#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>

//#include "pgr.h"
#include "application/window.h"
#include "visualization/camera.h"
#include "visualization/meshes/mesh.h"
#include "visualization/objects/object.h"
#include "visualization/objects/water.h"
#include "visualization/objects/skybox.h"
#include "visualization/shaders/shader.h"
#include "visualization/shaders/waterShader.h"

#include "simulation/amplitude_grid.h"

const unsigned int WIDTH = 1000;
const unsigned int HEIGHT = 1000;
static const char* TITLE = "Water Wave Simulation";

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

float timeMultiplier = -0.9;
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
	Camera::get().updateMatrices();

	double lastStart = frameStart;
	frameStart = glfwGetTime();
	//float dt = (frameStart - lastStart) * timeMultiplier;

	double dt = simulationGrid->cflTimeStep() * pow(10, timeMultiplier);

	if (update)
	{
		simulationGrid->timeStep(dt);
		waterMesh->updateData(amplMultiplier, *simulationGrid);
		waterMesh->setProfileBuffer(simulationGrid->profileBuffers[0]);
	}
}

void render()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glClearColor(0.0f, 1.0f, 0.0f, 1.0f);
	if (!drawWireframe)
		water->draw(Camera::get());
	else
		water->draw(Camera::get(), GL_LINE);
	if (drawSkybox)
		skybox->draw(Camera::get());
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

	window->onUpdate();
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
}

void initShaders() 
{
	std::cout << "INFO: Initializing shaders" << std::endl;

	commonShader = new Shader("shaders/standard.vert", "shaders/standard.frag");
	waterShader = new WaterShader();
}

void initImGui()
{
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGui_ImplGlfw_InitForOpenGL(window->getHandle(), true);
	ImGui_ImplOpenGL3_Init();

	ImGui::StyleColorsDark();

	ImGuiIO& io = ImGui::GetIO();
	io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
}

void shutdownImGui()
{
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();
}

void renderGui()
{
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplGlfw_NewFrame();
	ImGui::NewFrame();
	//ImGui::DockSpaceOverViewport();
	{
		ImGui::Begin("Settings");
		ImGui::Separator();
		ImGui::Text("Simulation");
		ImGui::Checkbox("Update", &update);
		ImGui::SliderInt("Direction", &simulationGrid->defaultDirection, 0, 15);
		ImGui::SliderFloat("Wind Speed", &simulationGrid->windSpeed, 0.5, 10);
		ImGui::SliderFloat("Amplitude", &simulationGrid->defaultAmplitudeVal, 0.0f, 4.0f);
		ImGui::SliderFloat("Time Multiplier", static_cast<float*>(&timeMultiplier), -2, 2);
		ImGui::SliderFloat("Amplitude Multiplier", &amplMultiplier, 0, 20);
		ImGui::SliderFloat("Multiplier", &mult, -10, 10);

		ImGui::Separator();
		ImGui::Text("Visualization");
		ImGui::Checkbox("Display Skybox", &drawSkybox);
		ImGui::Checkbox("Display Wireframe", &drawWireframe);
		ImGui::ColorEdit3("Ambient", (float*) & water->ambient);
		ImGui::ColorEdit3("Diffuse", (float*) &water->diffuse);
		ImGui::ColorEdit3("Specular", (float*) &water->specular);
		ImGui::SliderFloat("Shininess", &water->shininess, 0.0f, 100.0f);

		ImGui::Separator();
		ImGui::Text("Camera");
		ImGui::DragFloat("Speed", &Camera::get().speed, 5.0, 0.0);
		ImGui::SliderAngle("FOV", &Camera::get().fovAngle, 10.0f, 179.0f);
		ImGui::DragFloat("Sensitivity", &Camera::get().sensitivity, 0.1f, 3.0f, 0.1f);
		ImGui::DragFloat("Far Plane", &Camera::get().farPlane, 10.0f, 1.0f);
		ImGui::SliderInt("Target FPS", &Camera::refreshRate, 10, 120);
		if (ImGui::Button("Toggle Free Mode")) Camera::get().toggleFreeMode();
		//if (ImGui::Button("Toggle Fullscreen")) 
		ImGui::InputDouble("FPS", &framerate, 0.0, 0.0, "%.2f", ImGuiInputTextFlags_ReadOnly);
		ImGui::InputDouble("Frame Time", &averageFrameTime, 0, 0, "%.6f", ImGuiInputTextFlags_ReadOnly);
		ImGui::End();
	}
	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
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

	waterMesh = new WaterMesh(waterShader, 100, 100.0f);
	water = new Water(waterMesh);

}

void pointDisturbance(double x, double y)
{
	glm::vec3 planePoint = Camera::get().intersectPlane(glm::vec3(0, 1, 0), glm::vec3(0), glm::vec2(x, y));
	simulationGrid->addPointDisturbance(glm::vec2(planePoint.x, planePoint.z), 0.1);
}

void motionCallback(GLFWwindow* w, double x, double y)
{
	//return;
	pointDisturbance(x, y);
}

void buttonCallback(GLFWwindow* w, int button, int action, int mods)
{
	const auto& io = ImGui::GetIO();
	if (io.WantCaptureMouse) return;

	double x, y;
	glfwGetCursorPos(w, &x, &y);

	pointDisturbance(x, y);
}

void keyCallback(GLFWwindow* w, int key, int scancode, int action, int mods)
{

	switch (action) 
	{
		case GLFW_PRESS:
			switch (key)
			{
				case GLFW_KEY_F2:
					Camera::get().toggleFreeMode();
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

	//glfwSetCursorPosCallback(window->getHandle(), motionCallback);
	glfwSetMouseButtonCallback(window->getHandle(), buttonCallback);
	glfwSetKeyCallback(window->getHandle(), keyCallback);




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

	shutdownImGui();
}

int main(int argc, char** argv)
{
	try 
	{
		initApp();
		initShaders();
		initData();
		initImGui();
		//glutMainLoop();

		while (!glfwWindowShouldClose(window->getHandle()))
		{
			preRender();
			render();
			renderGui();
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
